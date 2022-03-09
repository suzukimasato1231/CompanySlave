#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include<vector>
class Enemy
{
public:
	Enemy();//コンストラクタ

	~Enemy();//ですコンストラクタ

	void Init();//初期化

	void Update(Vec3 pPos);//更新


	void Draw();//描画

	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="BPos">ブロックの位置</param>
	/// <param name="blockSize">ブロックサイズ</param>
	/// <param name="up">上のブロック</param>
	/// <param name="down">下のブロック</param>
	void PushBlock(int num,Vec3 BPos, float blockSize, const int up, const int down);

	Vec3 GetPosition(int i);//座標取得

	Box GetBox(int i);
private:
	struct EnemyData
	{
		Object::ObjectData enemyObject;//プレイヤーオブジェクト
		Box eBox;//プレイヤーの球
		Vec3 position{ 50.0f,0.0f,0.0f };//座標
		Vec3 oldPosition{};
		Vec3 speed{ 2.0f,2.0f,2.0f };//プレイヤースピード
		Vec3 scale{ 1.0f,1.0f,1.0f };//大きさ
		Vec3 angle{ 0.0f,0.0f,0.0f };//角度
		Vec4 color{ 1.0f,1.0f,1.0f,1.0f };//色
		float r = 5;
		//ジャンプ
		const float jumpPowerMax = 5.0f;
		float jumpPower = 5.0f;//ジャンプパワー
		float jumpPowerDelay = 0.2;
		float gravity = 2.0f;//重力
		bool jumpFlag = false;//ジャンプをしたか
		bool groundFlag = false;//地面に接しているかどうか
	};

	std::vector<EnemyData *>enemyData;
};