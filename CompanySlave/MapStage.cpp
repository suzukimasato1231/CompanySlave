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

	floor_Tile[0] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile1.png");
	floor_Tile[1] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile2.png");
	floor_Tile[2] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile3.png");
	floor_Tile[3] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile4.png");
	floor_Tile[4] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile13.png");
	floor_Tile[5] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile14.png");
	floor_Tile[6] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile7.png");
	floor_Tile[7] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile8.png");
	floor_Tile[8] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile9.png");
	floor_Tile[9] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile10.png");
	floor_Tile[10] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile11.png");
	floor_Tile[11] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile12.png");
	floor_Tile[12] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile5.png");
	floor_Tile[13] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile6.png");
	floor_Tile[14] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile15.png");
	floor_Tile[15] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile16.png");
	floor_Tile[16] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile17.png");
	floor_Tile[17] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile18.png");
	floor_Tile[18] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile19.png");
	floor_Tile[19] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile20.png");
	floor_Tile[20] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile21.png");
	floor_Tile[21] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile22.png");
	floor_Tile[22] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile24.png");
	floor_Tile[23] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile23.png");
	floor_Tile[24] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile26.png");
	floor_Tile[25] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile25.png");
	floor_Tile[26] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile30.png");
	floor_Tile[27] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile29.png");
	floor_Tile[28] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile28.png");
	floor_Tile[29] = Object::Instance()->LoadTexture(L"Resources/map/MapGraph/Floor_Tile27.png");


	wallBlock = Object::Instance()->CreateOBJ("wall");

	cornerBlock = Object::Instance()->CreateOBJ("wall2");

	okeBlock = Object::Instance()->CreateOBJ("oke");

	sakuBlock = Object::Instance()->CreateOBJ("saku");
	//仮
	smokeGraph = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	particleMan = ParticleManager::Create(L"Resources/particle.jpg");
}

void MapStage::StageInit(int stageNum)
{
	char* Filepath = "";
	char* FilepathOBJ = "";
	switch (stageNum)
	{
	case 1:
		Filepath = (char*)"Resources/map/Floor_Tile1.csv";
		FilepathOBJ = (char*)"Resources/map/Obj_Tile1.csv";
		break;
	case 2:
		Filepath = (char*)"Resources/map/Floor_Tile2.csv";
		FilepathOBJ = (char*)"Resources/map/Obj_Tile2.csv";
		break;
	case 3:
		Filepath = (char*)"Resources/map/Floor_Tile3.csv";
		FilepathOBJ = (char*)"Resources/map/Obj_Tile3.csv";
		break;
	case 4:
		Filepath = (char*)"Resources/map/Floor_Tile4.csv";
		FilepathOBJ = (char*)"Resources/map/Obj_Tile4.csv";
		break;
	default:
		break;
	}
	LoadCSV(map, Filepath);

	LoadCSV(mapOBJ, FilepathOBJ);


	nextFlag = false;
}

void MapStage::Update(Vec3 pPos, Enemy* enemy)
{
	bool isFlag = true;
	int X = pPos.x / mapSize;
	int Z = pPos.z / (-mapSize);
	//敵が全員倒していたら
	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			isFlag = false;
		}
	}
	for (int y = (Z - 7); y < (Z + 6); y++)
	{
		for (int x = (X - 11); x < (X + 13); x++)
		{
			if (y < 0 || x < 0 || y >= MAP_HEIGHT || x >= MAP_WIDTH)
			{
				continue;
			}
			//次のマップへ進める
			if (nextFlag == false && isFlag == true)
			{
				if (mapOBJ[y][x] == SMOKEWALL)
				{
					// 初期化
					mapOBJ[y][x] = NONE;
				}
			}
			//もくもく煙
			if (mapOBJ[y][x] == SMOKEWALL)
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
	for (int j = (Z - 7); j < (Z + 6); j++)
	{
		for (int i = (X - 11); i < (X + 13); i++)
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
					scale, Vec3{}, color, floor_Tile[0]);
				break;
			case FLOOR_TILE2:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[1]);
				break;
			case FLOOR_TILE3:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[2]);
				break;
			case FLOOR_TILE4:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[3]);
				break;
			case FLOOR_TILE5:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[4]);
				break;
			case FLOOR_TILE6:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[5]);
				break;
			case FLOOR_TILE7:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[6]);
				break;
			case FLOOR_TILE8:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[7]);
				break;
			case FLOOR_TILE9:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[8]);
				break;
			case FLOOR_TILE10:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[9]);
				break;
			case FLOOR_TILE11:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[10]);
				break;
			case FLOOR_TILE12:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[11]);
				break;
			case FLOOR_TILE13:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[12]);
				break;
			case FLOOR_TILE14:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[13]);
				break;
			case FLOOR_TILE15:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[14]);
				break;
			case FLOOR_TILE16:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[15]);
				break;
			case FLOOR_TILE17:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[16]);
				break;
			case FLOOR_TILE18:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[17]);
				break;
			case FLOOR_TILE19:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[18]);
				break;
			case FLOOR_TILE20:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[19]);
				break;
			case FLOOR_TILE21:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[20]);
				break;
			case FLOOR_TILE22:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[21]);
			case FLOOR_TILE23:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[22]);
				break;
			case FLOOR_TILE24:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[23]);
				break;
			case FLOOR_TILE25:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[24]);
				break;
			case FLOOR_TILE26:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[25]);
				break;
			case FLOOR_TILE27:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[26]);
				break;
			case FLOOR_TILE28:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[27]);
				break;
			case FLOOR_TILE29:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[28]);
				break;
			case FLOOR_TILE30:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-1,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, floor_Tile[29]);
				break;
			}
			switch (mapOBJ[j][i])
			{
			case NONE:
				break;
			case WALLWIDTH:
				Object::Instance()->Draw(wallBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0,90,0 }, color);
				break;
			case WALLHIGHT:
				Object::Instance()->Draw(wallBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{}, color);
				break;
			case WALL_LU:
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0.0f,0.0f,0.0f }, color);
				break;
			case WALL_RU:
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0.0f,90.0f,0.0f }, color);
				break;
			case WALL_RD:
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0.0f,180.0f,0.0f }, color);
				break;
			case WALL_LD:
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0.0f,270.0f,0.0f }, color);
				break;
			case OKE:
				/*Object::Instance()->Draw(okeBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 3.0f, 3.0f, 3.0f }, Vec3{ 0.0f,45.0f,0.0f }, color);*/
				Object::Instance()->Draw(sakuBlock,
					Vec3{ basePosition.x + i * mapSize - 3.0f,0,basePosition.y + j * (-mapSize) },
					{ 1.1f, 1.0f, 1.0f }, Vec3{ 0.0f,0.0f,0.0f }, color);
				break;
			case SMOKEWALL:
				break;
			case NextStageBlock:
				break;
			default:
				break;

			}
		}
	}


}

void MapStage::DrawParticle(Vec3 pPos)
{
	int X = pPos.x / mapSize;
	int Z = pPos.z / (-mapSize);
	for (int j = (Z - 7); j < (Z + 6); j++)
	{
		for (int i = (X - 11); i < (X + 13); i++)
		{
			if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
			{
				continue;
			}
			if (mapOBJ[j][i] == SMOKEWALL)
			{
				particleMan->Draw();
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
	return mapOBJ[j][i];
}

float MapStage::GetSize()
{
	return mapSize;
}
