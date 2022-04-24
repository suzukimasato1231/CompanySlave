#pragma once
#include<vector>
#include"LoadCSV.h"
#include"Player.h"
#include"EnemyHelper.h"
#include"OniType.h"
#include"OniBow.h"
#include"WOLF.h"

class Enemy
{
private:
	//敵最大数
	static const int eNumMax = 20;
	//敵配列
	std::vector<EnemyData*>eData;

public:
	Enemy();//コンストラクタ

	~Enemy();//ですコンストラクタ

	void Init();//初期化

	//ステージごとの初期化
	void StageInit(int stageNum);

	void Update(class Player* player);//更新

	void Draw();//描画
	//血痕描画
	void BloodDraw();

	//TRUEにする
	void WasAttack(int i);
	//敵の位置を設定
	void SetPosition(int i, Vec3 position);

	/// <summary>
	/// プレイヤーの通常攻撃ダメージ
	/// </summary>
	void DamegeNormal(int i, int pAttackDirection);

	/// <summary>
	/// 剣のダメージ投げ
	/// </summary>
	/// <param name="i"></param>
	void DamegeThrowSword(int i);
	/// <summary>
	/// 剣のダメージ戻し
	/// </summary>
	/// <param name="i"></param>
	void DamegeSword(int i);
private:
	//削除
	void Delete();
	//敵が向いている方向
	int Direction(int i, class Player* player);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);
	//ノックバック処理
	void NockBack(int i);
public://取得系
	// 座標取得
	Vec3 GetPosition(int i) { return eData[i]->position; }

	float GetHP(int i) { return eData[i]->HP; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//エネミー数取得
	int GetEnemySize() { return eData.size(); }
	//攻撃されたかを取得
	//bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }

	float GetEnemyR(int i) { return eData[i]->r; }

	int GetTime(int i) { return BloodTime[i]; }

	bool GetDamegeFlag(int i) { return eData[i]->DamegeFlag; }
	bool SetDamegeFlag(int i, bool DamegeFlag);

	bool GetExplosionFlag(int i) { return eData[i]->explosionFlag; }
	bool SetExplosionFlag(int i);

	bool GetExplosionCount(int i) { return eData[i]->explosionCount; }
	bool SetExplosionCount(int i);

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//沸き地のマップ

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//マップチップの初期位置
private:
	//エネミー１
	class OniType oniType;

	void UpdateOni(int i, class Player* player);
	//エネミー弓
	class OniBow oniBow;

	void UpdateBow(int i, class Player* player);

	//狼
	class Wolf wolf;

	void UpdateWolf(int i, class Player* player);

private:
	//ノックバックステータス
	const float nockPower = 0.8f;
	const int nockBackTimeMax = 5;
	//HPUI
	Object::ObjectData hpOBJ;
	Object::ObjectData hpGaugeOBJ;
	Object::ObjectData explosionOBJ;
	int hpGaugeGraph;
	int hpGraph;
	int explosionGraph;

	int redColor;
	//血関係の変数

	Object::ObjectData Blood;	//血痕
	int BloodGraph;
	bool BloodFlag[eNumMax] = { false };
	Vec3 BloodPosition[eNumMax] = { Vec3(0,0,0) };

	Object::ObjectData BloodFrount;	//飛んでいる血
	Object::ObjectData BloodBack;

	int Blood2Graph[3];
	int Blood2GraphBack[3];
	int Blood2Flag[eNumMax];
	int BloodTime[eNumMax];
	const int bloodTimeMax = 30;//飛び散る血の画像が出ている時間


	//破
	int delayCount[eNumMax];
	int explosionGraphCnt[eNumMax];
	bool explosionFlag[eNumMax] = { false };
	Vec3 explosionPosition[eNumMax] = { Vec3(0,0,0) };
};