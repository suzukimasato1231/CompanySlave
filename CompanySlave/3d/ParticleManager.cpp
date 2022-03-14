#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device *ParticleManager::device = nullptr;
Camera *ParticleManager::camera = nullptr;
UINT ParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList *ParticleManager::cmdList = nullptr;
Pipeline::PipelineSet ParticleManager::PartclePipelineSet;
ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap;
ComPtr<ID3D12Resource> ParticleManager::vertBuff;
ComPtr<ID3D12Resource>  ParticleManager::constBuff; // 定数バッファ
ComPtr<ID3D12Resource> ParticleManager::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE ParticleManager::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE ParticleManager::gpuDescHandleSRV;
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
ParticleManager::VertexPos ParticleManager::vertices[vertexCount];

XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();


bool ParticleManager::StaticInitialize(ID3D12Device *device,ID3D12GraphicsCommandList *cmdList, Camera *camera, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);
	assert(camera);
	ParticleManager::device = device;

	ParticleManager::camera = camera;

	ParticleManager::cmdList = cmdList;

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// パイプライン初期化
	//InitializeGraphicsPipeline();
	PartclePipelineSet = Pipeline::ParticleCreateGraphicsPipeline(device);

	// テクスチャ読み込み
	LoadTexture();

	// モデル生成
	CreateModel();

	return true;
}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList *cmdList)
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(PartclePipelineSet.pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(PartclePipelineSet.rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw()
{
	// コマンドリストを解除
	ParticleManager::cmdList = nullptr;
}

ParticleManager *ParticleManager::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager *particle = new ParticleManager();
	if (particle == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!particle->Initialize()) {
		delete particle;
		assert(0);
		return nullptr;
	}

	return particle;
}


void ParticleManager::Add(int life, Vec3 position, Vec3 velocity, Vec3 accel, float start_scale, float end_scale, Vec4 start_color, Vec4 end_color)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle &p = particles.front();
	//値のリセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.s_color = start_color;
	p.e_color = end_color;
}

void ParticleManager::ParticleAdd(Vec3 Pos, Vec4 start_color, Vec4 end_color)
{
	for (int i = 0; i < 1; i++)
	{
		//X,Y,Z全て{-5.0f,+5.0f}でランダムに分布
		const float md_pos = 0.5f;
		Vec3 pos = Pos;
		pos.x += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		////X,Y,Z全て{-0.05f,+0.05f}でランダムに分布
		const float md_vel = 0.1f;
		Vec3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ{-0.001f,0}でランダム分布
		Vec3 acc{};
		const float md_acc = 0.001f;
		acc.y = (float)rand() / RAND_MAX * md_acc;

		//	Vec4 start_color = { 1.0f,1.0f,1.0f,1.0f };
		//	Vec4 end_color = { 1.0f,1.0f,1.0f,1.0f };
			//追加
		Add(60, pos, vel, acc, 2.0f, 2.0f, start_color, end_color);
	}
}


bool ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}


bool ParticleManager::LoadTexture()
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		L"Resources/particle.jpg", WIC_FLAGS_NONE,
		&metadata, scratchImg);
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
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		return result;
	}

	// テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return result;
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);

	return true;
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}


	// 頂点バッファへのデータ転送
	VertexPos *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}


	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}


bool ParticleManager::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
}

void ParticleManager::Update()
{
	HRESULT result;

	//寿命が尽きたパーティクルを全削除

	particles.remove_if(
		[](Particle &x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		////速度による移動
		it->position = it->position + it->velocity;

		float f = (float)it->num_frame / it->frame;

		//スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;

		//色
		it->color = (it->e_color - it->s_color) / f;

		it->color = it->color + it->s_color;

	}

	//頂点バッファへデータ転送
	VertexPos *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result))
	{
		//パーティクル情報を１つずつ反映
		for (std::forward_list<Particle>::iterator it = particles.begin();

			it != particles.end();
			it++) {

			//座標
			vertMap->pos = it->position;
			vertMap->scale = it->scale;
			vertMap->color = it->color;
			//次の頂点へ
			vertMap++;
		}

		vertBuff->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->mat = camera->GetMatView() * camera->GetProjection();	// 行列の合成
	constMap->matBillboard = matBillboard;
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw()
{
	// nullptrチェック
	assert(device);
	assert(ParticleManager::cmdList);

	// パイプラインステートの設定
	cmdList->SetPipelineState(PartclePipelineSet.pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(PartclePipelineSet.rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 描画コマンド
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}
