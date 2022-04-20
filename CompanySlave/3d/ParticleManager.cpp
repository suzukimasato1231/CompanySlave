#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* ParticleManager::device = nullptr;
Camera* ParticleManager::camera = nullptr;
UINT ParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
Pipeline::PipelineSet ParticleManager::PartclePipelineSet;
Pipeline::PipelineSet ParticleManager::ParticlePipelineSetNotAlpha;
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();


bool ParticleManager::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);
	assert(camera);
	ParticleManager::device = device;

	ParticleManager::cmdList = cmdList;

	// パイプライン初期化
	PartclePipelineSet = Pipeline::ParticleCreateGraphicsPipeline(device);

	ParticlePipelineSetNotAlpha = Pipeline::ParticleCreateGraphicsNoAplaha(device);

	// モデル生成
	//CreateModel();

	return true;
}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
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

ParticleManager* ParticleManager::Create(const wchar_t* filename, int partcleNum)
{
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager* particle = new ParticleManager();



	particle->InitializeDescriptorHeap();

	particle->CreateModel();

	if (particle == nullptr) {
		return nullptr;
	}

	if (!particle->LoadTexture(filename))
	{
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!particle->Initialize()) {
		delete particle;
		assert(0);
		return nullptr;
	}

	particle->pieplineNum = partcleNum;

	return particle;
}


void ParticleManager::Add(int life, Vec3 position, Vec3 velocity, Vec3 accel, float start_scale, float end_scale, Vec4 start_color, Vec4 end_color)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
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

void ParticleManager::Add2(int life, Vec3 position, Vec3 velocity, Vec3 accel, float start_scale, float end_scale, Vec4 start_color, Vec4 end_color)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
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

void ParticleManager::ParticleAdd(Vec3 Pos, float md_vel, float md_vel2, Vec4 start_color, Vec4 end_color)
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
		Vec3 vel{};
		//左右
		if (particleDirection == 0 || particleDirection == 1) {

			vel.x += (float)rand() / RAND_MAX * md_vel;
			vel.y += (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z += (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		}
		//上下
		if (particleDirection == 2 || particleDirection == 3) {

			vel.x += (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y += (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z += (float)rand() / RAND_MAX * md_vel;
		}
		//斜め
		if (particleDirection == 4 || particleDirection == 5 || particleDirection == 6 || particleDirection == 7) {

			vel.x += (float)rand() / RAND_MAX * md_vel;
			vel.y += (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z += (float)rand() / RAND_MAX * md_vel2;
		}
		//重力に見立ててYのみ{-0.001f,0}でランダム分布
		Vec3 acc{};
		const float md_acc = 0.011f;
		acc.z = (float)rand() / RAND_MAX * md_acc;

		//	Vec4 start_color = { 1.0f,1.0f,1.0f,1.0f };
		//	Vec4 end_color = { 1.0f,1.0f,1.0f,1.0f };
			//追加
		Add(15, pos, vel, acc, 4.0f, 0.3f, start_color, end_color);
	}
}

void ParticleManager::ParticleAdd2(Vec3 Pos, Vec4 start_color, Vec4 end_color)
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
		const float md_acc = -0.001f;
		acc.y = (float)rand() / RAND_MAX * md_acc;

		//	Vec4 start_color = { 1.0f,1.0f,1.0f,1.0f };
		//	Vec4 end_color = { 1.0f,1.0f,1.0f,1.0f };
			//追加
		Add2(90, pos, vel, acc, 4.0f, 8.0f, start_color, end_color);
	}

}
void ParticleManager::ParticleAdd3(Vec3 Pos, float md_vel, Vec4 start_color, Vec4 end_color)
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
		Add(20, pos, vel, acc, 1.0f, 0.0f, start_color, end_color);
	}
}
void ParticleManager::AddBlood(Vec3 Pos)
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
		float md_vel = 2.0f;
		Vec3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel;
		vel.z = (float)rand() / RAND_MAX * md_vel;
		//重力に見立ててYのみ{-0.001f,0}でランダム分布
		Vec3 acc{};
		const float md_acc = 0.1f;
		acc.z = -md_acc;

		Vec4 start_color = { 1.0f,1.0f,1.0f,1.0f };
		Vec4 end_color = { 1.0f,1.0f,1.0f,1.0f };
		//追加
		Add(10, pos, vel, acc, 1.0f, 1.0f, start_color, end_color);
	}
}
bool ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = textureMax; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}


bool ParticleManager::LoadTexture(const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return result;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

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

	//// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);


	//UINT descHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	////デスクリプタヒープの先頭ハンドルを取得
	//cpuDescHandleSRV = descHeap->GetCPUDescriptorHandleForHeapStart();
	////ハンドルのアドレスを進める
	//cpuDescHandleSRV.ptr += textureNum * descHandleIncrementSize;

	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleStart = descHeap->GetGPUDescriptorHandleForHeapStart();
	////1番SRV
	//gpuDescHandleSRV = gpuDescHandleStart;
	//gpuDescHandleSRV.ptr += textureNum * descHandleIncrementSize;


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
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}


	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}


void ParticleManager::MatBillboardUpdate()
{
	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&camera->GetEye());
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&camera->GetTarget());
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&camera->GetUp());

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）	

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	XMMATRIX matView = camera->GetMatView();
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y軸回りビルボード行列の計算
	// カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = cameraAxisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	// Y軸回りビルボード行列
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
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
		[](Particle& x)
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
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
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
	//ビルボード行列の更新
	MatBillboardUpdate();

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = camera->GetMatView() * camera->GetProjection();	// 行列の合成
	constMap->matBillboard = matBillboard;
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw()
{
	// nullptrチェック
	assert(device);
	assert(ParticleManager::cmdList);
	if (pieplineNum == 1)
	{
		// パイプラインステートの設定
		cmdList->SetPipelineState(ParticlePipelineSetNotAlpha.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(ParticlePipelineSetNotAlpha.rootsignature.Get());
	}
	else
	{
		// パイプラインステートの設定
		cmdList->SetPipelineState(PartclePipelineSet.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(PartclePipelineSet.rootsignature.Get());
	}

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 描画コマンド
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}
int ParticleManager::SetParticleDirection(int particleDirection)
{
	return this->particleDirection = particleDirection;
}
