#include"MapStage.h"
#include"Shape.h"
#include <Safe_delete.h>
/// <summary>
/// 静的メンバ変数の実体
/// </summary>

MapStage::MapStage()
{}
MapStage::~MapStage()
{
	safe_delete(particleMan);
}

void MapStage::Init()
{
	block = Object::Instance()->CreateOBJ("Ground");

	floor_Tile1 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile1.png");
	floor_Tile2 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile2.png");
	floor_Tile3 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile3.png");
	floor_Tile4 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile4.png");
	floor_Tile5 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile5.png");
	floor_Tile6 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile6.png");
	floor_Tile7 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile7.png");
	floor_Tile8 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile8.png");
	floor_Tile9 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile9.png");
	floor_Tile10 = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile10.png");


	wallBlock = Object::Instance()->CreateOBJ("wall");

	cornerBlock = Object::Instance()->CreateOBJ("wall2");

	okeBlock = Object::Instance()->CreateOBJ("oke");
	//仮
	smokeGraph = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	particleMan = ParticleManager::Create(L"Resources/particle.jpg");
}

void MapStage::StageInit(int stageNum)
{
	char* Filepath = "";
	switch (stageNum)
	{
	case 1:
		Filepath = (char*)"Resources/map/stage01.csv";
		break;
	case 2:
		Filepath = (char*)"Resources/map/stage02.csv";
		break;
	case 3:
		break;
	default:
		break;
	}
	LoadCSV(map, Filepath);
	nextFlag = false;
}

void MapStage::Update(Enemy* enemy)
{
	bool isFlag = true;
	//敵が全員倒していたら
	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			isFlag = false;
		}
	}
	for (size_t y = 0; y < MAP_HEIGHT; y++)
	{
		for (size_t x = 0; x < MAP_WIDTH; x++)
		{
			//次のマップへ進める
			if (nextFlag == false && isFlag == true)
			{
				if (map[y][x] == SMOKEWALL)
				{
					// 初期化
					map[y][x] = NONE;
				}
			}
			//もくもく煙
			if (map[y][x] == SMOKEWALL)
			{
				particleMan->ParticleAdd2(Vec3{ 0 + (float)x * 10,2, 0 + (float)y * (-10) }, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));
				particleMan->ParticleAdd2(Vec3{ 0 + (float)x * 10,2, 0 + (float)y * (-10) }, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));

			}
		}
	}

	particleMan->Update();
}

void MapStage::Draw(Vec3 pPos)
{
	int X = pPos.x / mapSize;
	int Z = pPos.z / (-mapSize);
	for (int j = (Z - 8); j < (Z + 7); j++)
	{
		for (int i = (X - 14); i < (X + 16); i++)
		{
			if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
			{
				continue;
			}
			switch (map[j][i])
			{
			case FLOOR_TILE1:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile1);
				break;
			case FLOOR_TILE2:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile2);
				break;
			case FLOOR_TILE3:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile3);
				break;
			case FLOOR_TILE4:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile4);
				break;
			case FLOOR_TILE5:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile5);
				break;
			case FLOOR_TILE6:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile6);
				break;
			case FLOOR_TILE7:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile7);
				break;
			case FLOOR_TILE8:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile8);
				break;
			case FLOOR_TILE9:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				break;
			case FLOOR_TILE10:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile10);
				break;
			case NONE:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile2);
				break;
			case WALLWIDTH:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				Object::Instance()->Draw(wallBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0,90,0 }, color);
				break;
			case WALLHIGHT:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				Object::Instance()->Draw(wallBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{}, color);
				break;
			case WALL_LD:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{}, color);
				break;
			case WALL_LU:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{0.0f,0.0f,0.0f}, color);
				break; 
			case WALL_RD:
					Object::Instance()->Draw(block,
						Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
						scale, Vec3{}, color, floor_Tile9);
					Object::Instance()->Draw(cornerBlock,
						Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
						{ 1.25f, 1.25f, 1.25f }, Vec3{}, color);
					break;
			case WALL_RU:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{}, color);
				break;


			case OKE:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile9);
				Object::Instance()->Draw(okeBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 3.0f, 3.0f, 3.0f }, Vec3{0.0f,45.0f,0.0f}, color);
				break;
			case SMOKEWALL:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile1);
				particleMan->Draw();
				break;
			case NextStageBlock:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile1);
				break;
			default:
				break;
			}
		}
	}
}

Box MapStage::GetPositionBlock(int i, int j)
{
	Box box;
	box.maxPosition = XMVectorSet(
		basePosition.x + i * mapSize + mapSize / 2,
		0,
		basePosition.z + j * (-mapSize) + mapSize / 2,
		1);
	box.minPosition = XMVectorSet(
		basePosition.x + i * mapSize - mapSize / 2,
		0,
		basePosition.z + j * (-mapSize) - mapSize / 2,
		1);
	return box;
}

Vec3 MapStage::GetPosition(int i, int j)
{
	return Vec3(basePosition.x + i * mapSize, 0, basePosition.z + j * (-mapSize));
}



int MapStage::GetMap(int i, int j)
{
	return map[j][i];
}

float MapStage::GetSize()
{
	return mapSize;
}
