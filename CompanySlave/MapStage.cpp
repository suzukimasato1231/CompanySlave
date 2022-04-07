#include"MapStage.h"
#include"Shape.h"
/// <summary>
/// 静的メンバ変数の実体
/// </summary>

MapStage::MapStage()
{
}

MapStage::~MapStage()
{
}

void MapStage::Init()
{
	block = Shape::CreateSquare(10.0f, 10.0f, 10.0f);

	blockGraph = Object::Instance()->LoadTexture(L"Resources/jimen.png");

	wallBlock = Object::Instance()->CreateOBJ("wall");

	cornerBlock = Object::Instance()->CreateOBJ("wall2");

	strawBlock = Object::Instance()->CreateOBJ("wara2");
	//仮
	smokeGraph = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");
	//nextGraph = Object::Instance()->LoadTexture(L"Resources/color/.png");
}

void MapStage::StageInit(int stageNum)
{
	char *Filepath;
	switch (stageNum)
	{
	case 1:
		Filepath = (char *)"Resources/map/stage01.csv";
		LoadCSV(map, Filepath);
		break;
	case 2:
		Filepath = (char *)"Resources/map/stage01.csv";
		LoadCSV(map, Filepath);
		break;
	default:
		break;
	}
	nextFlag = false;
}

void MapStage::Update(Enemy *enemy)
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
	//次のマップへ進める
	if (nextFlag == false && isFlag == true)
	{
		for (size_t y = 0; y < MAP_HEIGHT; y++)
		{
			for (size_t x = 0; x < MAP_WIDTH; x++)
			{
				if (map[y][x] == SMOKEWALL)
				{
					// 初期化
					map[y][x] = NONE;
				}
			}
		}
		nextFlag = true;
	}

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
			case NONE:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				break;
			case WALLWIDTH:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				Object::Instance()->Draw(wallBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{ 0,90,0 }, color, blockGraph);
				break;
			case WALLHIGHT:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				Object::Instance()->Draw(wallBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{}, color, blockGraph);
				break;
			case WALLCORNER:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				Object::Instance()->Draw(cornerBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 1.25f, 1.25f, 1.25f }, Vec3{}, color, blockGraph);
				break;
			case STRAW:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				Object::Instance()->Draw(strawBlock,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					{ 3.0f, 3.0f, 3.0f }, Vec3{}, color, blockGraph);
				break;
			case SMOKEWALL:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				/*Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, smokeGraph);*/
				break;
			case NextStageBlock:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
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
