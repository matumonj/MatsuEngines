#include "BossMap.h"
#include <algorithm>
#include"CameraControl.h"
#include"PlayerControl.h"
#include"imgui.h"
#include"mHelper.h"

BossMap* BossMap::GetIns()
{
	static BossMap ins;
	return &ins;
}

BossMap::~BossMap()
{
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			Destroy_unique(mapobj[i][j]);
		}
	}
}


void BossMap::Init()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			mapSize[i][j] = BLOCK;
			mapobj[i][j] = std::make_unique<Object3d>();
			//フィールドにモデル割り当て
			mapobj[i][j]->Initialize(camera);
			mapobj[i][j]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BOX));
			//フィールドにモデル割り当て
		}
	}
}

#include"EnemyControl.h"
#include"RushAttack.h"

void BossMap::Upda()
{
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
	Player* player = PlayerControl::GetIns()->GetPlayer();

	// 頂点間の長さを測る
	// 頂点間の長さを測る
	float line_length = Collision::CalculationVertexLength(RushArea.start, RushArea.end);


	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapobj[i][j] == nullptr)
			{
				continue;
			}
			if (boss != nullptr) {
				if (Collision::GetLength(boss->GetPosition(), mapobj[i][j]->GetPosition()) < 20.f)
				{
					BlockPosYEase[i][j] += 0.02f;
				} else
				{
					BlockPosYEase[i][j] -= 0.02f;
					BlockPosY[i][j] = 4.f;
				}
			}
			BlockPosY[i][j] = Easing::EaseOut(BlockPosYEase[i][j], 4.f, 3.f);
			BlockPosYEase[i][j] = std::clamp(BlockPosYEase[i][j], 0.f, 1.f);
			//フィールドにモデル割り当て
			mapobj[i][j]->SetPosition({j * BlockSize + cpos.x,cpos.y, i * BlockSize + cpos.z});
			//mapobj[i][j]->SetScale({ bsize.x,BlockPosY[i][j],bsize.z});

			bpoint[i][j].x = mapobj[i][j]->GetPosition().x;
			bpoint[i][j].y = mapobj[i][j]->GetPosition().z;
			mapobj[i][j]->SetFogCenter(camera->GetEye());
			mapobj[i][j]->Update(camera);
		}
	}

	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapobj[i][j] == nullptr)
			{
				continue;
			}
			if (mapSize[i][j] == DAMAGEBLOCK)
			{
				blockColorETime[i][j] += 0.02f;
				//mapobj[i][j]->SetUVf(true);
				mapobj[i][j]->SetShadowF(false);
				mapobj[i][j]->SetColor({blockColor[i][j].x, blockColor[i][j].y, blockColor[i][j].z, 0.8f});
			}
			else
			{
				blockColorETime[i][j] -= 0.02f;

				mapobj[i][j]->SetShadowF(false);
				mapobj[i][j]->SetUVf(false);
				mapobj[i][j]->SetColor({blockColor[i][j].x, blockColor[i][j].y, blockColor[i][j].z, 1.f});
			}

			blockalpha[i][j] = std::clamp(blockalpha[i][j], 0.f, 1.f);
			blockColor[i][j].x = Easing::EaseOut(blockColorETime[i][j], 1.0f, 1.0f);
			blockColor[i][j].y = Easing::EaseOut(blockColorETime[i][j], 1.0f, 0.1f);
			blockColor[i][j].z = Easing::EaseOut(blockColorETime[i][j], 1.0f, 0.1f);

			blockColorETime[i][j] = min(blockColorETime[i][j], 1.0f);
			blockColorETime[i][j] = max(blockColorETime[i][j], 0.0f);
		}
	}
}

void BossMap::DrawDamageLine(bool atckjudg, Line2D line)
{
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapobj[i][j] == nullptr)
			{
				continue;
			}
			if (atckjudg)
			{
				if (Collision::IsCollidingLineAndCircle(line, bpoint[i][j], 40.f) == true)
				{
					mapSize[i][j] = DAMAGEBLOCK;
				}
				else
				{
					mapSize[i][j] = BLOCK;
				}
			}

			else
			{
				mapSize[i][j] = BLOCK;
			}
		}
	}
}

void BossMap::DrawDamageLine(bool atckjudg, Line2D line[4])
{
	if (RushAttack::GetIns()->GetPhase() == RushAttack::PHASE_TWO ||
		RushAttack::GetIns()->GetPhase() == RushAttack::PHASE_THREE)
	{
		return;
	}
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapobj[i][j] == nullptr)
			{
				continue;
			}
			if (atckjudg)
			{
				for (int k = 0; k < 4; k++)
				{
					if (Collision::IsCollidingLineAndCircle(line[k], bpoint[i][j], 20.f) == true)
					{
						mapSize[i][j] = DAMAGEBLOCK;
					}
				}
			}
			else
			{
				mapSize[i][j] = BLOCK;
			}
		}
	}
}


void BossMap::Draw()
{
	Object3d::PreDraw();
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapobj[i][j] == nullptr)
			{
				continue;
			}
			mapobj[i][j]->Draw();
		}
	}

	Object3d::PostDraw();
}
