#include "TargetMarker.h"
#include"Input.h"
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include"EnemyControl.h"
#include"PlayerControl.h"

TargetMarker* TargetMarker::GetInstance()
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
	if (enemy[index]->GetEnemyNumber() == enemy[index]->FLOG || enemy[index] == nullptr || enemy[index]->GetHP() < 0)
	{
		return false;
	}
	//Žw’è‚³‚ê‚½“Y‚¦Žš‚Ì“GÀ•W‚ðŽæ“¾
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
	if (enemy[index]->GetEnemyNumber() == enemy[index]->GOLEM || enemy[index] == nullptr || enemy[index]->GetHP() < 0)
	{
		return false;
	}
	//Žw’è‚³‚ê‚½“Y‚¦Žš‚Ì“GÀ•W‚ðŽæ“¾
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
	float tempx, tempy, tempz;
	if (enemy[index] == nullptr || enemy[index]->GetHP() < 0)
	{
		return false;
	}
	//Žw’è‚³‚ê‚½“Y‚¦Žš‚Ì“GÀ•W‚ðŽæ“¾
	tempx = enemy[index]->GetPosition().x;
	tempy = enemy[index]->GetPosition().y;
	tempz = enemy[index]->GetPosition().z;
	*x = enemy[index]->GetPosition().x;
	*y = enemy[index]->GetPosition().y;
	*z = enemy[index]->GetPosition().z;

	return true;
}

int TargetMarker::NearGolemSearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{
	int nearindex = -1;
	float nearresult = 0;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr)
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

		//”äŠr‚µ‚Ä¬‚³‚¯‚ê‚Î‚»‚ê‚ðÅ¬’l‚É
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


int TargetMarker::NearLizardSearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{
	int nearindex = -1;
	float nearresult = 0;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr)
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

		//”äŠr‚µ‚Ä¬‚³‚¯‚ê‚Î‚»‚ê‚ðÅ¬’l‚É
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
	float nearresult = 0;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i)
	{
		if (enemy[i] == nullptr)
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

		//”äŠr‚µ‚Ä¬‚³‚¯‚ê‚Î‚»‚ê‚ðÅ¬’l‚É
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
	tindex = NearEnemySearch(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL),
	                         PlayerControl::GetInstance()->GetPlayer());
	if (tindex != -1)
	{
		GetEnemyPosition(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL), tindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//ƒ}[ƒJ[ˆÊ’u‚ðnowTarget‚É‡‚í‚¹‚é
	MarkerPosition = {ex, ey + 1, ez};
	nowTarget = MarkerPosition;
	//if (tindex != -1) {
	float rotY = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetRotRadians();
}

void TargetMarker::Update_PlayScene(DebugCamera* camera)
{
	int tindex;
	int lindex;
	int gindex;
	float ex, ey, ez;
	tindex = NearEnemySearch(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE),
	                         PlayerControl::GetInstance()->GetPlayer());

	lindex = NearLizardSearch(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE),
	                          PlayerControl::GetInstance()->GetPlayer());

	gindex = NearGolemSearch(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE),
	                         PlayerControl::GetInstance()->GetPlayer());

	if (tindex != -1)
	{
		GetEnemyPosition(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE), tindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	if (lindex != -1)
	{
		NearLizard(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE), tindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	if (gindex != -1)
	{
		NearGolem(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE), tindex, &ex, &ey, &ez);
		//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//ƒ}[ƒJ[ˆÊ’u‚ðnowTarget‚É‡‚í‚¹‚é
	MarkerPosition = {ex, ey + 1, ez};
	nowTarget = MarkerPosition;

	if (tindex != -1)
	{
		float rotY = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[tindex]->GetRotRadians();
	}
}

void TargetMarker::Draw()
{
}

void TargetMarker::Finalize()
{
	delete TargetMakerTexture;
}
