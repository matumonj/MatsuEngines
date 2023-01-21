#include "BossMap.h"
#include"CameraControl.h"
#include "Field.h"
#include"PlayerControl.h"
#include"imgui.h"
#include"mHelper.h"
#include "UltAttack.h"

BossMap* BossMap::GetInstance()
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

	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
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
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	Player* player = PlayerControl::GetInstance()->GetPlayer();

	// 頂点間の長さを測る
	// 頂点間の長さを測る
	float line_length = Collision::CalculationVertexLength(RushArea.start, RushArea.end);


	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			//フィールドにモデル割り当て
			mapobj[i][j]->SetPosition({j * BlockSize + cpos.x, cpos.y, i * BlockSize + cpos.z});
			mapobj[i][j]->SetScale(bsize);

			bpoint[i][j].x = mapobj[i][j]->GetPosition().x;
			bpoint[i][j].y = mapobj[i][j]->GetPosition().z;
			mapobj[i][j]->Update({1.f, 0.f, 0.f, 1.f}, camera);
		}
	}

	if (Input::GetInstance()->TriggerButton(Input::X))
	{
		p = INI;
	}
	DamageBlockMove();
	SetDamageArea();

	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapSize[i][j] == DAMAGEBLOCK)
			{
				blockColorETime[i][j] += 0.02f;
				//mapobj[i][j]->SetUVf(true);
				mapobj[i][j]->SetShadowF(false);
				mapobj[i][j]->SetColor({blockColor[i][j].x, blockColor[i][j].y, blockColor[i][j].z, 0.5});
			}
			else
			{
				blockColorETime[i][j] -= 0.02f;

				mapobj[i][j]->SetShadowF(false);
				mapobj[i][j]->SetUVf(false);
				mapobj[i][j]->SetColor({blockColor[i][j].x, blockColor[i][j].y, blockColor[i][j].z,1.f});
			}

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
			if (atckjudg)
			{
				if (Collision::IsCollidingLineAndCircle(line, bpoint[i][j],40.f) == true)
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
	if (RushAttack::GetInstance()->GetPhaseEnd() == RushAttack::PHASETWO ||
		RushAttack::GetInstance()->GetPhaseEnd() == RushAttack::PHASETHREE)return;
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (atckjudg)
			{
				for (int k = 0; k < 4; k++)
				{
					if (Collision::IsCollidingLineAndCircle(line[k], bpoint[i][j],20.f) == true)
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

void BossMap::DamageBlockMove()
{
	if (p == INI)
	{
		for (int i = 0; i < mapHight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				if (mapSize[i][j] == DAMAGEBLOCK)
				{
					nailPos[i][j] = mapobj[i][j]->GetPosition();
					nailalpha[i][j] = 1.05f;
				}
			}
		}
		p = UPDA;
	}

	if (p == UPDA)
	{
		for (int i = 0; i < mapHight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				if (mapSize[i][j] == DAMAGEBLOCK)
				{
					nailPos[i][j].y += 0.5f;
				}

				nailPos[i][j].y = min(nailPos[i][j].y, 15);
				nailPos[i][j].y = max(nailPos[i][j].y, -6);
			}
		}
		rockappertime++;
		if (rockappertime > 240)
		{
			p = END;
		}
	}

	if (p == END)
	{
		for (int i = 0; i < mapHight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				nailPos[i][j].y = min(nailPos[i][j].y, 15);
				nailPos[i][j].y = max(nailPos[i][j].y, -16);
			}
		}
	}
}

void BossMap::SetDamageArea()
{
}

void BossMap::Draw()
{
	Object3d::PreDraw();
	for (int i = 0; i < mapHight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			mapobj[i][j]->Draw();
		}
	}
	
	Object3d::PostDraw();
	
}
