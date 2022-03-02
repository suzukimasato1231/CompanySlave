#include "Object.h"
#include <DirectXTex.h>
#include<fstream>
#include<sstream>
#include<string>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device *Object::dev = nullptr;
ID3D12GraphicsCommandList *Object::cmdList = nullptr;
Camera *Object::camera = nullptr;
LightGroup *Object::lightGroup = nullptr;
Object::~Object()
{
	//OBJデータ削除
	for (int i = (int)OBJdata.size() - 1; i >= 0; i--)
	{
		delete OBJdata[i];
		OBJdata.erase(OBJdata.begin() + i);
	}

	for (int i = (int)objectBuffer.size() - 1; i >= 0; i--)
	{
		delete objectBuffer[i];
		objectBuffer.erase(objectBuffer.begin() + i);
	}

	for (int i = (int)textureData.size() - 1; i >= 0; i--)
	{
		delete textureData[i];
		textureData.erase(textureData.begin() + i);
	}

}

void  Object::Init(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList, Camera *camera, LightGroup *lightGroup)
{
	Object::dev = dev;

	Object::cmdList = cmdList;

	Object::camera = camera;

	Object::lightGroup = lightGroup;

	//パイプライン生成
	objPipelineSet = Pipeline::OBJCreateGraphicsPipeline(Object::dev);
}

Object *Object::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Object *object = new Object();
	if (object == nullptr) {
		return nullptr;
	}

	// 初期化
	return object;
}

void Object::AddSmoothData(ObjectData &polygon, unsigned short indexPosition, unsigned short indexVertex)
{
	polygon.smoothData[indexPosition].emplace_back(indexVertex);
}

void Object::CalculateSmoothedVertexNormals(ObjectData &polygon)
{
	auto itr = polygon.smoothData.begin();
	for (; itr != polygon.smoothData.end(); ++itr)
	{
		//各面用の共通頂点コレクション
		std::vector<unsigned short> &v = itr->second;
		//全頂点の法線を平均する
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(polygon.vertices[index]->normal.x, polygon.vertices[index]->normal.y, polygon.vertices[index]->normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v)
		{
			polygon.vertices[index]->normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
		}
	}
}

inline size_t Object::GetVertexCount(ObjectData &polygon)
{
	return polygon.vertices.size();
}

int  Object::LoadTexture(const wchar_t *filename)
{
	HRESULT result = S_FALSE;

	textureData.push_back(new Object::TextureData);

	if (texNum == 0)
	{
		//設定構造体
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見える
		descHeapDesc.NumDescriptors = constBufferNum;//定数バッファの数
		//デスクリプタヒープの生成
		result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	}


	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image *img = scratchImg.GetImage(0, 0, 0);//生データ抽出

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&textureData[texNum]->texbuff));

	//テクスチャバッファにデータ転送
	result = textureData[texNum]->texbuff->WriteToSubresource(
		0,
		nullptr,//全領域へコピー
		img->pixels,//元データアドレス
		(UINT)img->rowPitch,//1ラインサイズ
		(UINT)img->slicePitch//全サイズ
	);

	//デスクリプタヒープの先頭ハンドルを取得
	textureData[texNum]->cpuDescHandleSRV = descHeap->GetCPUDescriptorHandleForHeapStart();
	//ハンドルのアドレスを進める
	textureData[texNum]->cpuDescHandleSRV.ptr += texNum * dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleStart = descHeap->GetGPUDescriptorHandleForHeapStart();
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//1番SRV
	textureData[texNum]->gpuDescHandleSRV = gpuDescHandleStart;
	textureData[texNum]->gpuDescHandleSRV.ptr += descHandleIncrementSize * texNum;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//２Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープの２番目にシェーダーリソースビューを作成
	dev->CreateShaderResourceView(textureData[texNum]->texbuff.Get(),//ビューと関連付けるバッファ
		&srvDesc,//テクスチャ設定構造
		textureData[texNum]->cpuDescHandleSRV
	);
	texNum++;
	return (int)texNum - 1;
}

void Object::MatWord(ObjectData polygon, Vec3 position, Vec3 scale, Vec3 rotation, Vec4 color)
{
	HRESULT result;
	//ワールド変換：//スケーリング
	XMMATRIX matScale;//スケーリング行列
	//ワールド変換：//回転
	XMMATRIX matRot;//回転行列
	//ワールド変換：//平行移動
	XMMATRIX matTrains;//平行移動行列
	//ワールド変換
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//大きさ
	matTrains = XMMatrixTranslation(position.x, position.y, position.z);//平行移動行列を再計算
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z軸まわりに４５度回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X軸まわりに４５度回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y軸まわりに４５度回転

	polygon.matWorld = XMMatrixIdentity();//ワールド行列は毎フレームリセット
	polygon.matWorld *= matScale;//ワールド行列にスケーリングを反映
	polygon.matWorld *= matRot;//ワールド行列に回転を反映
	polygon.matWorld *= matTrains;//ワールド行列に変更移動を反映

	const XMMATRIX &matViewProjection = camera->GetMatView() * camera->GetProjection();
	const Vec3 &cameraPos = camera->GetEye();
	//GPU上のバッファに対応した仮想メモリを取得
	ConstBufferDataB0 *constMap = nullptr;
	result = Object::OBJdata[OBJNum]->constBuffB0->Map(0, nullptr, (void **)&constMap);
	//行列の合成   ワールド変換行列 ＊ ビュー変換行列 ＊ 射影変換行列
	constMap->viewproj = matViewProjection;
	if (polygon.parent == nullptr)
	{
		constMap->world = polygon.matWorld;
	}
	else
	{
		constMap->world = polygon.matWorld * polygon.parent->matWorld;
	}
	constMap->cameraPos = cameraPos;
	Object::OBJdata[OBJNum]->constBuffB0->Unmap(0, nullptr);


	//定数バッファへデータ転送
	ConstBufferDataB1 *constMap1 = nullptr;
	result = Object::OBJdata[OBJNum]->constBuffB1->Map(0, nullptr, (void **)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	Object::OBJdata[OBJNum]->constBuffB1->Unmap(0, nullptr);
}

void Object::PreDraw()
{
	objNum = 0;
	OBJNum = 0;
}

void Object::OBJConstantBuffer()
{
	HRESULT result;
	Object::OBJdata.push_back(new Object::OBJData);
	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Object::OBJdata[Object::OBJdata.size() - 1]->constBuffB0));

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Object::OBJdata[Object::OBJdata.size() - 1]->constBuffB1));
}


void Object::Draw(ObjectData polygon, Vec3 position, Vec3 scale, Vec3 rotation, Vec4 color, int graph)
{
	//プリミティブ形状の設定コマンド（三角形リスト）
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	if (OBJNum >= Object::OBJdata.size())
	{
		//定数バッファ
		OBJConstantBuffer();
	}
	//更新
	MatWord(polygon, position, scale, rotation, color);

	cmdList->SetPipelineState(objPipelineSet.pipelinestate.Get());
	cmdList->SetGraphicsRootSignature(objPipelineSet.rootsignature.Get());

	//頂点バッファの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////インデックスバッファビューのセットコマンド
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//ヒープの先頭にあるCBVをルートパラメータ０番に設定
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJdata[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJdata[OBJNum]->constBuffB1->GetGPUVirtualAddress());
	if (polygon.OBJTexture == 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, textureData[graph]->gpuDescHandleSRV);
	}
	else
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, textureData[polygon.OBJTexture]->gpuDescHandleSRV);
	}
	//ライトの描画
	lightGroup->Draw(cmdList, 3);
	//描画コマンド          //頂点数				//インスタンス数	//開始頂点番号		//インスタンスごとの加算番号
	cmdList->DrawIndexedInstanced((UINT)polygon.indices.size(), 1, 0, 0, 0);
	OBJNum++;
}


int Object::LoadMaterial(const std::string &directoryPath, const std::string &filename)
{
	int texNumber = 0;
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');
		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());//先頭の文字を削除
		}
		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> material.name;
		}
		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		//先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		//先頭文字列がmap_Kdならテクスチャ名
		if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			line_stream >> material.textureFilename;
			//テクスチャ読み込み
			texNumber = OBJLoadTexture(directoryPath, material.textureFilename);
		}
	}
	//ファイルを閉じる
	file.close();
	return  texNumber;
}

void Object::OBJCreateModel(ObjectData &polygon)
{
	HRESULT result;
	//頂点データ全体のサイズ＝頂点データ一つ分のサイズ＊頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * polygon.vertices.size());

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * polygon.indices.size());

	////頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.vertBuff)
	);

	////インデックスバッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.indexBuff));

	Vertex *vertMap = nullptr;
	//GPU上のバッファに対応した仮想メモリを取得
	result = polygon.vertBuff->Map(0, nullptr, (void **)&vertMap);

	//全頂点に対して
	for (size_t i = 0; i < polygon.vertices.size(); i++)
	{
		vertMap[i] = *polygon.vertices[i];//座標コピー
	}

	//マップを解除
	polygon.vertBuff->Unmap(0, nullptr);

	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short *indexMap = nullptr;
	result = polygon.indexBuff->Map(0, nullptr, (void **)&indexMap);

	//全インデックスに対して
	for (size_t i = 0; i < polygon.indices.size(); i++)
	{
		indexMap[i] = *polygon.indices[i];//インデックスをコピー
	}
	//繋がりを解除
	polygon.indexBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	polygon.vbView.BufferLocation =
		polygon.vertBuff->GetGPUVirtualAddress();
	polygon.vbView.SizeInBytes = sizeVB;
	polygon.vbView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファビューの作成
	polygon.ibView.BufferLocation = polygon.indexBuff->GetGPUVirtualAddress();
	polygon.ibView.Format = DXGI_FORMAT_R16_UINT;
	polygon.ibView.SizeInBytes = sizeIB;
}

int Object::OBJLoadTexture(const std::string &directoryPath, const std::string &filename)
{
	HRESULT result = S_FALSE;
	textureData.push_back(new Object::TextureData);
	if (texNum == 0)
	{
		//設定構造体
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見える
		descHeapDesc.NumDescriptors = constBufferNum;//定数バッファの数
		//デスクリプタヒープの生成
		result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	}
	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//ファイルパスを結合
	string filepath = directoryPath + filename;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	if (FAILED(result)) {
		return result;
	}
	const Image *img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	// テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&textureData[texNum]->texbuff));
	if (FAILED(result)) {
		return result;
	}
	// テクスチャバッファにデータ転送
	result = textureData[texNum]->texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return result;
	}
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// シェーダリソースビュー作成
	textureData[texNum]->cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//ハンドルのアドレスを進める
	textureData[texNum]->cpuDescHandleSRV.ptr += texNum * descHandleIncrementSize;
	textureData[texNum]->gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData[texNum]->gpuDescHandleSRV.ptr += descHandleIncrementSize * texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = textureData[texNum]->texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData[texNum]->texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureData[texNum]->cpuDescHandleSRV
	);
	texNum++;
	return (int)texNum - 1;
}


Object::ObjectData Object::CreateOBJ(const std::string filename, bool smoothing)
{
	Object::ObjectData polygon;

	int index = 0;//インデックスの数調整
	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	std::string directoryPath = "Resources/" + filename + "/";
	file.open(directoryPath + filename + ".obj");
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	vector<Vec3>positions;//頂点座標
	vector<Vec3>normals;//法線ベクトル
	vector<Vec2>texcoords;//テクスチャUV
	//１行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			polygon.OBJTexture = LoadMaterial(directoryPath, filename);
		}

		//先頭文字がvなら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			Vec3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}
		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V成分読み込み
			Vec2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			Vec3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			int faceIndexCount = 0;
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexNormal;

				//頂点データの追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				polygon.vertices.push_back(new Vertex);
				polygon.vertices[polygon.vertices.size() - 1]->pos = vertex.pos;
				polygon.vertices[polygon.vertices.size() - 1]->normal = vertex.normal;
				polygon.vertices[polygon.vertices.size() - 1]->uv = vertex.uv;
				//エッジ円滑化用のデータを追加
				if (smoothing)
				{
					//vキー(座標データ)の番号と、全て合成した頂点のインデックスをセットで登録する
					AddSmoothData(polygon, indexPosition, (unsigned short)GetVertexCount(polygon) - 1);
				}
				// インデックスデータの追加
				if (faceIndexCount >= 3) {
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					polygon.indices.push_back(new unsigned short(polygon.indices.size() - 1 - index));
					polygon.indices.push_back(new unsigned short(polygon.indices.size() - 1 - index));
					polygon.indices.push_back(new unsigned short(polygon.indices.size() - 5 - index));
					index += 2;
				}
				else
				{
					polygon.indices.push_back(new unsigned short(polygon.indices.size() - index));
				}

				faceIndexCount++;

			}
		}
	}
	//ファイルを閉じる
	file.close();
	//頂点法線の平均によるエッジを円滑化
	if (smoothing)
	{
		CalculateSmoothedVertexNormals(polygon);
	}
	OBJCreateModel(polygon);

	return polygon;
}
