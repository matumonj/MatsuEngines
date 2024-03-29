#include "TargetMarker.h"
#include"Input.h"
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include"EnemyControl.h"
#include"PlayerControl.h"

TargetMarker* TargetMarker::GetIns()
{
	static TargetMarker instance;

	return &instance;
}

void TargetMarker::Initialize()
{
	//MarkerPosition = { -10,2,0 };
}


bool TargetMarker::NearGolem(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	float tempx, tempy, tempz;
	if (enemy[index]->GetEnemyNumber() != enemy[index]->GOLEM || enemy[index] == nullptr || enemy[index]->GetHP() < 0)
	{
		return false;
	}
	//指定された添え字の敵座標を取得
	tempx = enemy[index]->GetPosition().x;
	tempy = enemy[index]->GetPosition().y;
	tempz = enemy[index]->GetPosition().z;
	*x = enemy[index]->GetPosition().x;
	*y = enemy[index]->GetPosition().y;
	*z = enemy[index]->GetPosition().z;

	return true;
}

bool TargetMarker::NearMiniGolem(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	float tempx, tempy, tempz;
	if (enemy[index]->GetEnemyNumber() != enemy[index]->MINIGOLEM || enemy[index] == nullptr || enemy[index]->GetHP() <
		0)
	{
		return false;
	}
	//指定された添え字の敵座標を取得
	tempx = enemy[index]->GetPosition().x;
	tempy = enemy[index]->GetPosition().y;
	tempz = enemy[index]->GetPosition().z;
	*x = enemy[index]->GetPosition().x;
	*y = enemy[index]->GetPosition().y;
	*z = enemy[index]->GetPosition().z;

	return true;
}


bool TargetMarker::NearLizard(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	float tempx, tempy, tempz;
	if (enemy[index]->GetEnemyNumber() != enemy[index]->FLOG || enemy[index] == nullptr || enemy[index]->GetHP() < 0)
	{
		return false;
	}
	//指定された添え字の敵座標を取得
	tempx = enemy[index]->GetPosition().x;
	tempy = enemy[index]->GetPosition().y;
	tempz = enemy[index]->GetPosition().z;
	*x = enemy[index]->GetPosition().x;
	*y = enemy[index]->GetPosition().y;
	*z = enemy[index]->GetPosition().z;

	return true;
}

bool TargetMarker::GetEnemyPosition(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	return true;
}

int TargetMarker::NearGolemSearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{
	int nearindex = -1;
	float nearresult = 0.f;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr || enemy[i]->getdeath())
		{
			continue;
		}
		if (!NearGolem(enemy, i, &ex, &ey, &ez))
		{
			continue;
		}
		tx = ex - player->GetPosition().x;
		ty = ey - player->GetPosition().y;
		tz = ez - player->GetPosition().z;

		if (nearindex == -1)
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			continue;
		}

		//比較して小さければそれを最小値に
		if (nearresult > sqrt(tx * tx + ty * ty + tz * tz))
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			//	nearresult2 = nearresult;
		}
		//nearindex2 = nearresult;
		//	if(nearresult)
	}


	this->nearGolem = nearindex;
	return nearGolem;
}

int TargetMarker::NearMiniGolemSearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{
	int nearindex = -1;
	float nearresult = 0.f;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr || enemy[i]->getdeath())
		{
			continue;
		}
		if (!NearMiniGolem(enemy, i, &ex, &ey, &ez))
		{
			continue;
		}
		tx = ex - player->GetPosition().x;
		ty = ey - player->GetPosition().y;
		tz = ez - player->GetPosition().z;

		if (nearindex == -1)
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			continue;
		}

		//比較して小さければそれを最小値に
		if (nearresult > sqrt(tx * tx + ty * ty + tz * tz))
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			//	nearresult2 = nearresult;
		}
		//nearindex2 = nearresult;
		//	if(nearresult)
	}


	this->nearMiniGolem = nearindex;
	return nearMiniGolem;
}


int TargetMarker::NearLizardSearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{
	int nearindex = -1;
	float nearresult = 0;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr || enemy[i]->getdeath())
		{
			continue;
		}
		if (!NearLizard(enemy, i, &ex, &ey, &ez))
		{
			continue;
		}
		tx = ex - player->GetPosition().x;
		ty = ey - player->GetPosition().y;
		tz = ez - player->GetPosition().z;

		if (nearindex == -1)
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			continue;
		}

		//比較して小さければそれを最小値に
		if (nearresult > sqrt(tx * tx + ty * ty + tz * tz))
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			//	nearresult2 = nearresult;
		}
		//nearindex2 = nearresult;
		//	if(nearresult)
	}


	this->nearLizard = nearindex;
	return nearLizard;
}

int TargetMarker::NearEnemySearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{
	int nearindex = -1;
	float nearresult = 0.f;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr || enemy[i]->getdeath())
		{
			continue;
		}
		if (!GetEnemyPosition(enemy, i, &ex, &ey, &ez))
		{
			continue;
		}
		tx = ex - player->GetPosition().x;
		ty = ey - player->GetPosition().y;
		tz = ez - player->GetPosition().z;

		if (nearindex == -1)
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			continue;
		}

		//比較して小さければそれを最小値に
		if (nearresult > sqrt(tx * tx + ty * ty + tz * tz))
		{
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			//	nearresult2 = nearresult;
		}
		//nearindex2 = nearresult;
		//	if(nearresult)
	}


	this->nearindex = nearindex;
	return nearindex;
}

void TargetMarker::Update_Tutorial(DebugCamera* camera)
{
	int tindex;
	float ex, ey, ez;
	tindex = NearEnemySearch(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL),
	                         PlayerControl::GetIns()->GetPlayer());
	if (tindex != -1)
	{
		GetEnemyPosition(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL), tindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//マーカー位置をnowTargetに合わせる
	MarkerPosition = {ex, ey + 1, ez};
	nowTarget = MarkerPosition;
	//if (tindex != -1) {
	float rotY = EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetRotRadians();
}

void TargetMarker::Update_PlayScene(DebugCamera* camera)
{
	int tindex;
	int lindex;
	int gindex;

	int mindex;
	float ex, ey, ez;
	tindex = NearEnemySearch(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE),
	                         PlayerControl::GetIns()->GetPlayer());

	lindex = NearLizardSearch(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE),
	                          PlayerControl::GetIns()->GetPlayer());

	gindex = NearGolemSearch(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE),
	                         PlayerControl::GetIns()->GetPlayer());
	mindex = NearMiniGolemSearch(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE),
	                             PlayerControl::GetIns()->GetPlayer());

	if (tindex != -1)
	{
		GetEnemyPosition(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE), tindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	if (lindex != -1)
	{
		NearLizard(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE), lindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	if (gindex != -1)
	{
		NearGolem(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE), gindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	if (mindex != -1)
	{
		NearMiniGolem(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE), mindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//マーカー位置をnowTargetに合わせる
	MarkerPosition = {ex, ey + 1, ez};
	nowTarget = MarkerPosition;

	if (tindex != -1)
	{
		float rotY = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[tindex]->GetRotRadians();
	}
}

void TargetMarker::Draw()
{
}

void TargetMarker::Finalize()
{
	delete TargetMakerTexture;
}
