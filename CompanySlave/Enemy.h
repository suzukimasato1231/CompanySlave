#pragma once
#include<vector>
#include"LoadCSV.h"
#include"Player.h"
#include"EnemyHelper.h"
#include"OniType.h"
#include"OniBow.h"


class Enemy
{
private:
	//敵最大数
	static const int eNumMax = 20;
	//敵配列
	std::vector<EnemyData *>eData;

public:
	Enemy();//コンストラクタ

	~Enemy();//ですコンストラクタ

	void Init();//初期化

	//ステージごとの初期化
	void StageInit(int stageNum);

	void Update(class Player *player);//更新

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
	void DamegeNormal(int i);

	void DamegeSword(int i);
private:
	//削除
	void Delete();
	//敵が向いている方向
	int Direction(int i, class Player *player);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);
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

	bool GetParticleFlag(int i) { return particleFlag[i]; }
	int GetParticleTime(int i) { return particleTime[i]; }

	bool GetDamegeFlag(int i) { return eData[i]->DamegeFlag; }
	bool SetDamegeFlag(int i, bool DamegeFlag);

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//沸き地のマップ

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//マップチップの初期位置
private://エネミー１
	class OniType oniType;

	void UpdateOni(int i, class Player *player);
	//エネミー弓
	class OniBow oniBow;

	void UpdateBow(int i, class Player *player);
private:
	//HPUI
	Object::ObjectData hpOBJ;
	Object::ObjectData hpGaugeOBJ;
	int hpGaugeGraph;
	int hpGraph;

	int redColor;
	//血関係の変数
	Object::ObjectData Blood;
	int BloodGraph;
	bool BloodFlag[eNumMax] = { false };
	Vec3 BloodPosition[eNumMax] = { Vec3(0,0,0) };

	int particleTime[eNumMax];
	bool particleFlag[eNumMax];
};