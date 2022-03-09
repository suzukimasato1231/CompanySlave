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
	char *Filepath = (char *)"Resources/map/stage01.csv";

	LoadCSV(map, Filepath);

	block = Shape::CreateSquare(10.0f, 10.0f, 10.0f);

	blockGraph = Object::Instance()->LoadTexture(L"Resources/block.png");
}

void MapStage::Update()
{
}

void MapStage::Draw()
{
	for (size_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (size_t i = 0; i < MAP_WIDTH; i++)
		{
			switch (map[j][i])
			{
			case NONE:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,-10,basePosition.y + j * (-mapSize) },
					scale, Vec3{}, color, blockGraph);
				break;
			case BLOCK:
				Object::Instance()->Draw(block,
					Vec3{ basePosition.x + i * mapSize,0,basePosition.y + j * (-mapSize) },
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
