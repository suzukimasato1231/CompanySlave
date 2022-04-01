#include"MapStage.h"
#include"Shape.h"
/// <summary>
/// ê√ìIÉÅÉìÉoïœêîÇÃé¿ëÃ
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
	//âº
	smokeGraph = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	nextGraph = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");
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
}

void MapStage::Update(Enemy *enemy)
{
	if (enemy->GetEnemySize() == 0)
	{
		for (size_t y = 0; y < MAP_HEIGHT; y++)
		{
			for (size_t x = 0; x < MAP_WIDTH; x++)
			{
				if (map[y][x] == SMOKEWALL)
					// èâä˙âª
					map[y][x] = NONE;
			}
		}
	}

}

void MapStage::Draw(Vec3 pPos)
{
	int X = pPos.x / mapSize;
	int Z = pPos.z / (-mapSize);
	for (int j = (Z - 6); j < (Z + 8); j++)
	{
		for (int i = (X - 11); i < (X + 13); i++)
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
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, smokeGraph);
				break;
			case NextStageBlock:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, nextGraph);
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
