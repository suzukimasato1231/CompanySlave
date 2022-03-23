#pragma once
#include<vector>
#include<string>
#include<d3dx12.h>
#include"Pipeline.h"
#include"Camera.h"
#include<unordered_map>
#include"LightGroup.h"
#include"Vec.h"
#include"Singleton.h"
#pragma comment(lib,"d3d12.lib")

 const int constBufferNum = 256;
class Object:public Singleton<Object>
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	friend Singleton<Object>;
public:
	static ID3D12Device *dev;					//デバイス
	static ID3D12GraphicsCommandList *cmdList;	//コマンドリスト
public://構造体
	////頂点データ構造体
	struct Vertex
	{
		Vec3 pos;//xyz座標
		Vec3 normal;//法線ベクトル
		Vec2 uv;//uv座標
	};

	//マテリアル
	struct Material
	{
		std::string name;			//マテリアル名
		Vec3 ambient;			//アンビエント影響度
		Vec3 diffuse;			//ディフューズ影響度
		Vec3 specular;			//スペキュラー影響度
		float alpha;				//アルファ
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material()
		{
			ambient = { 1.0f,1.0f,1.0f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
	/// <summary>
	/// 位置、大きさ、色情報
	/// </summary>
	struct PSC
	{
		Vec3 position = {};
		Vec3 scale = {};
		Vec4 color = {};
	};

	//オブジェクトデータ構造体
	struct ObjectData
	{
		//頂点データ
		std::vector<Vertex *> vertices;
		//インデックスデータ
		std::vector<unsigned short *> indices;
		//頂点バッファの生成
		ComPtr<ID3D12Resource> vertBuff = nullptr;
		//インデックスバッファ
		ComPtr<ID3D12Resource> indexBuff = nullptr;
		//頂点バッファビューの作成
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		//インデックスバッファビューの作成
		D3D12_INDEX_BUFFER_VIEW ibView{};
		//オブジェ読み込み用
		int OBJTexture = 0;
		//頂点法線スムージング用データ
		std::unordered_map<unsigned short, std::vector<unsigned short>>smoothData;
		//ワールド変換
		XMMATRIX matWorld;
		//親クラス
		ObjectData *parent = nullptr;
		//マテリアル
		Material material;
		//位置、大きさ、色情報
		PSC psc;
	};

	struct ConstBuffer
	{
		//GPUリソースの生成
		ComPtr<ID3D12Resource>constBuff = nullptr;
	};

	//OBJデータ関連
	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;//ビュープロジェクション行列
		XMMATRIX world;//ワールド座標
		Vec3 cameraPos;//カメラ座標(ワールド座標)
		Vec4 color;
	};
	struct ConstBufferDataB1
	{
		Vec3 ambient;	//アンビエント係数
		float pad1;			//パディング
		Vec3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		Vec3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};

	//OBJ定数バッファ
	struct OBJData
	{
		ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
		ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	};
	//テクスチャデータ
	struct TextureData
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		ComPtr<ID3D12Resource> texbuff;
	};
public://パイプライン設定
	Pipeline::PipelineSet objPipelineSet;		//OBJ読み込み
public://変数	
	//テクスチャデータの作成
	std::vector <TextureData *>textureData;
	//OBJデータ
	std::vector<OBJData *> OBJdata;
	//定数バッファ用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap >descHeap = { nullptr };
	//オブジェクトのコンストバッファ
	std::vector <ConstBuffer *>objectBuffer;
public:
	Object() {};
	~Object();
	//初期化
	void Init(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList);
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object *Create();
	/// <summary>
	/// カメラをセット
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera *camera) { Object::camera = camera; }
	/// <summary>
	/// ライトをセット
	/// </summary>
	/// <param name="lightGroup"></param>
	static void SetLight(LightGroup *lightGroup) { Object::lightGroup = lightGroup; }
public:
	//カメラクラス
	static Camera *camera;
	//ライトクラス
	static LightGroup *lightGroup;
public://図形モデル作成
	/// <summary>
	/// OBJオブジェクト作成
	/// </summary>
	/// <param name="filename">filename</param>
	/// <param name="smoothing"></param>
	/// <returns></returns>
	ObjectData CreateOBJ(const std::string filename, bool smoothing = false);
public://オブジェクト関連
	// テクスチャ読み込み
	int LoadTexture(const wchar_t *filename);
	//オブジェクト描画前
	void PreDraw();
public://OBJ関連
	int OBJLoadTexture(const std::string &directoryPath, const std::string &filename);
	//マテリアル読み込み
	int LoadMaterial(const std::string &directoryPath, const std::string &filename, ObjectData &polygon);
	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmoothData(ObjectData &polygon, unsigned short indexPosition, unsigned short indexVertex);
	///<summary>
	///平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals(ObjectData &polygon);

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	inline size_t GetVertexCount(ObjectData &polygon);
	//モデル作成
	void OBJCreateModel(ObjectData &polygon);
	//定数バッファ設定
	void OBJConstantBuffer();
	/// <summary>
	/// オブジェクトのデータ更新
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="matRot">回転</param>
	/// <param name="color">色</param>
	void MatWord(ObjectData &polygon, Vec3 position, Vec3 scale, Vec3 matRot, Vec4 color);
	/// <summary>
	/// OBJ描画
	/// </summary>
	/// <param name="polygon">オブジェクトデータ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="matRot">回転</param>
	/// <param name="color">色</param>
	void Draw(ObjectData &polygon, Vec3 position, Vec3 scale, Vec3 matRot, Vec4 color = { 1,1,1,1 }, int graph = 0);
public:
	size_t OBJNum = 0;//OBJ読み込みの数
	size_t objNum = 0;//オブジェクトの数
	size_t texNum = 0;//テクスチャの数
};