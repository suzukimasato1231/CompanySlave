#pragma once
#include<vector>
#include<string>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include "Pipeline.h"
#include"Vec.h"
#include"Singleton.h"
//テクスチャの最大枚数
const int spriteSRVCount = 256;

extern const int window_width;
extern const int window_height;

struct SpriteData
{
	//頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//Z軸回りの回転角
	float rotation = 0.0f;
	//座標
	Vec3 position = { 0,0,0 };
	//ワールド行列
	XMMATRIX matWorld;
	//テクスチャ番号
	UINT texNumber = 0;
	//大きさ
	Vec2 size = { 100,100 };

	Vec2 texLeftTop = { 0,0 };

	Vec2 texSize = { 0,0 };
	//アンカーポイント
	Vec2 anchorpoint = { 0.5f,0.5f };
	//親
	SpriteData* parent = nullptr;
};

class Sprite :public Singleton<Sprite>
{
	friend Singleton<Sprite>;
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
public://構造体

	struct VertexPosUv
	{
		Vec3 pos;//xyz座標
		Vec2 uv;//uv座標
	};


	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vec4 color;
		XMMATRIX mat;//3D変換行列
	};

	//スプライト共通データ
	struct SpriteCommon
	{
		//パイプラインセット
		Pipeline::PipelineSet pipelineSet;
		////射影行列
		XMMATRIX matProjection{};
		//テクスチャ用デスクリプタヒープの生成
		ComPtr<ID3D12DescriptorHeap>descHeap;
		//テクスチャリソース（テクスチャバッファ）の配列
		ComPtr<ID3D12Resource>texBuff[spriteSRVCount];
	};

	struct ConstBuffer
	{
		//GPUリソースの生成1
		ComPtr<ID3D12Resource> constBuff;

	};

public:
	Sprite();
	~Sprite();
public:
	static ID3D12Device *dev;
	static ID3D12GraphicsCommandList *cmdList;
	//スプライト共通データ
	SpriteCommon spriteCommon;
public://静的関数
	//スプライト設定静的初期化
	static void StaticInit(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList);

	static Sprite *Sprite::Create();

	void Init();

	void SpriteAllInitialization(int widow_width, int window_height);

	//スプライト共通データを生成
	SpriteCommon SpriteCommonCreate(int window_width, int window_height);
public:


	//スプライト生成
	SpriteData SpriteCreate(const wchar_t *filename);

	//スプライト共通テクスチャ読み込み
	int SpriteCommonLoadTexture(const wchar_t *filename);

	//スプライト単体頂点バッファの転送 大きさを変えれる
	void SpriteTransferVertexBuffer(const SpriteData &sprite);

	void PreDraw();

	void CreateConstBuffer();

	//スプライト単体更新
	void Update(SpriteData &sprite, Vec2 position, float width, float height, Vec2 anchorpoint, Vec4 color, bool isFlipX, bool isFlipY);

	/// <summary>
	/// //スプライト単体描画
	/// </summary>
	/// <param name="sprite">スプライトデータ</param>
	/// <param name="position">座標</param>
	/// <param name="width">横の長さ</param>
	/// <param name="height">縦の長さ</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="color">色</param>
	/// <param name="isFlipX"></param>
	/// <param name="isFlipY"></param>
	void Draw(SpriteData &sprite, Vec2 position, float width, float height, Vec2 anchorpoint = { 0.0f,0.0f }, Vec4 color = { 1.0f,1.0f,1.0f,1.0f }, bool isFlipX = false, bool isFlipY = false);

	//スプライト共通グラフィックコマンドのセット
	void SpriteCommonBeginDraw();
public://デバックテキスト
	void DebugUpdate(SpriteData &sprite);
	//スプライト単体描画/スプライトデータsprite
	void DebugDraw(SpriteData &sprite);
private:
	std::vector<ConstBuffer *>constBuffer;
public:
	UINT texnumber = 0;
	UINT spriteNum = 0;
};
