#include "TargetMarker.h"
#include"Input.h"
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include"EnemyControl.h"
TargetMarker::~TargetMarker()
{
	delete TargetMakerTexture;
}
TargetMarker* TargetMarker::GetInstance()
{
	static TargetMarker instance;

	return &instance;
}
void TargetMarker::Initialize()
{
	Texture::LoadTexture(86, L"Resources/target.png");
	TargetMakerTexture = Texture::Create(86, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	TargetMakerTexture->CreateTexture();
	TargetMakerTexture->SetAnchorPoint({ 0.5,0.5});
	//MarkerPosition = { -10,2,0 };
}

bool TargetMarker::GetEnemyPosition(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	float tempx, tempy, tempz;
	if (enemy[index] == nullptr||enemy[index]->GetHP()<0) {
	
		return false;
	}
		//Žw’è‚³‚ê‚½“Y‚¦Žš‚Ì“GÀ•W‚ðŽæ“¾
		enemy[index]->Getposition(&tempx, &tempy, &tempz);
		*x = enemy[index]->GetPosition().x;
		*y = enemy[index]->GetPosition().y;
		*z = enemy[index]->GetPosition().z;

		return true;
}
int TargetMarker::NearEnemySearch(std::vector<std::unique_ptr<Enemy>>& enemy, Player* player)
{

	int nearindex = -1;
	float nearresult = 0;
	float ex, ey, ez;
	float tx, ty, tz;
	//float distance;
	for (int i = 0; i < enemy.size()/*enemynum*/; ++i) {

		if (!GetEnemyPosition(enemy, i, &ex, &ey, &ez)) {
			//nearindex = -1;
			continue;
		}
		tx = ex - player->GetPosition().x;
		ty = ey - player->GetPosition().y;
		tz = ez - player->GetPosition().z;

		if (nearindex == -1) {
			nearindex = i;
			nearresult = sqrt(tx * tx + ty * ty + tz * tz);
			continue;
		}
		
		//”äŠr‚µ‚Ä¬‚³‚¯‚ê‚Î‚»‚ê‚ðÅ¬’l‚É
		if (nearresult > sqrt(tx * tx + ty * ty + tz * tz)) {
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
void TargetMarker::Update(DebugCamera* camera, Player* player)
{
	
}

void TargetMarker::Draw()
{
	if (EnemyTargetDrawFlag) {
		TargetMakerTexture->Draw();
	}
}
void TargetMarker::Finalize()
{
	delete TargetMakerTexture;

}