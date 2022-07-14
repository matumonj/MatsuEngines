#include "TargetMarker.h"
#include"Input.h"
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>

TargetMarker* TargetMarker::GetInstance()
{
	static TargetMarker instance;

	return &instance;
}
void TargetMarker::Initialize()
{
	Texture::LoadTexture(8, L"Resources/targetcircle.png");
	TargetMakerTexture = Texture::Create(8, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	TargetMakerTexture->CreateTexture();

	//MarkerPosition = { -10,2,0 };
}

bool TargetMarker::GetEnemyPosition(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	float tempx, tempy, tempz;
	if (enemy[index] == nullptr||enemy[index]->State_Dead()) {
	
		return false;
	}
		//指定された添え字の敵座標を取得
		enemy[index]->Getposition(&tempx, &tempy, &tempz);
		*x = tempx;
		*y = tempy;
		*z = tempz;

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
		
		//比較して小さければそれを最小値に
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
void TargetMarker::Update(std::vector<std::unique_ptr<Enemy>>& enemy, DebugCamera* camera, Player* player)
{
	int tindex;
	float ex, ey, ez;
	tindex = NearEnemySearch(enemy, player);
	if (tindex != -1) {
		GetEnemyPosition(enemy, tindex, &ex, &ey, &ez);//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//マーカー位置をnowTargetに合わせる
	MarkerPosition = { ex,ey,ez };
	nowTarget = MarkerPosition;

	if (Input::GetInstance()->Pushkey(DIK_L)) {
		MarkerPosition.y += 0.2f;
	}
	if (Input::GetInstance()->Pushkey(DIK_K)) {
		MarkerPosition.y -= 0.2f;
	}

	TargetMakerTexture->SetRotation({ rot, 0, 0 });

	TargetMakerTexture->SetPosition(MarkerPosition);
	TargetMakerTexture->Update(camera);
}

void TargetMarker::Draw()
{
	//Texture::PreDraw(dxcomn->GetCmdList());
	TargetMakerTexture->Draw();
	//Texture::PostDraw();
}
void TargetMarker::Finalize()
{
	delete TargetMakerTexture;

}