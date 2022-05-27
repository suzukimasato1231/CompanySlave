﻿#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<forward_list>
#include"Vec.h"
#include"Camera.h"
#include"Pipeline.h"
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス

	struct VertexPos
	{
		Vec3 pos;
		float scale;//スケール
		Vec4 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;//ビルボード行列
	};

	struct Particle
	{
		//座標
		Vec3 position = {};
		//速度
		Vec3 velocity = {};
		//加速度
		Vec3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終地
		float e_scale = 0.0f;

		//カラー
		Vec4 color = {};
		//初期値
		Vec4 s_color = {};
		//最終地
		Vec4 e_color = {};

	};
	//テクスチャデータ
	struct TextureData
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		ComPtr<ID3D12Resource> texbuff;
	};

	//パーティクル配列
	std::forward_list<Particle>particles;

private: // 定数
	static const int vertexCount = 1024;//頂点数

	static const int textureMax = 256;
public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <param name="partcleNum">0:add,1:alpha</param>
	/// <returns></returns>
	static ParticleManager* Create(const wchar_t* filename, int partcleNum = 0);

	//カメラをセット
	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name = "life">生存時間</param>
	/// <param name = "position">初期座標</param>
	/// <param name = "velocity">速度</param>
	/// <param name = "accel">加速度</param>
	void Add(int life, Vec3 position, Vec3 velocity, Vec3 accel,
		float start_scale, float end_scale, Vec4 start_color, Vec4 end_color);

	void Add2(int life, Vec3 position, Vec3 velocity, Vec3 accel,
		float start_scale, float end_scale, Vec4 start_color, Vec4 end_color);

	void ParticleManager::ParticleAdd(Vec3 Pos, float md_vel, float md_vel2, float start_scale, float end_scale, Vec4 start_color, Vec4 end_color, int addNum);

	void ParticleAdd2(Vec3 Pos, Vec4 start_color, Vec4 end_color);
	void ParticleAdd3(Vec3 Pos, float md_vel, float size, Vec4 start_color, Vec4 end_color);

	//血が飛び出るパーティクル
	void AddBlood(Vec3 Pos);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;

	static Camera* camera;

	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// デスクリプタヒープ
	//static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	//static ComPtr<ID3D12Resource> vertBuff;

	//static ComPtr<ID3D12Resource> constBuff; // 定数バッファ

	// 頂点バッファビュー
	//static D3D12_VERTEX_BUFFER_VIEW vbView;

	// 頂点データ配列
	//static VertexPos vertices[vertexCount];

	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	//パイプライン設定
	static Pipeline::PipelineSet PartclePipelineSet;

	static Pipeline::PipelineSet ParticlePipelineSetNotAlpha;

private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	bool InitializeDescriptorHeap();

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();
	/// <summary>
	/// ビルボート行列の更新
	/// </summary>
	void MatBillboardUpdate();

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadTexture(const wchar_t* filename);

	int GetParticleDirtection() { return particleDirection; }
	int SetParticleDirection(int particleDirection);
private: // メンバ変数

	// ローカルスケール
	Vec3 scale = { 1,1,1 };
	//ローカルカラー
	Vec4 color = { 1,1,1,1 };

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	int textureNum = 0;
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource>constBuff; // 定数バッファ

	ComPtr<ID3D12DescriptorHeap> descHeap;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ParticleManager::VertexPos vertices[vertexCount];

	int pieplineNum = 0;
	int particleDirection = 0;
};

