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
	Texture::LoadTexture(86, L"Resources/AOE.png");
	TargetMakerTexture = Texture::Create(86, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	TargetMakerTexture->CreateTexture();
	TargetMakerTexture->SetAnchorPoint({ 0.5,0.5 });
	//MarkerPosition = { -10,2,0 };
}

bool TargetMarker::GetEnemyPosition(std::vector<std::unique_ptr<Enemy>>& enemy, int index, float* x, float* y, float* z)
{
	float tempx, tempy, tempz;
	if (enemy[index] == nullptr || enemy[index]->GetHP() < 0) {

		return false;
	}
	//指定された添え字の敵座標を取得
	tempx= enemy[index]->GetPosition().x;
	tempy=enemy[index]->GetPosition().y;
	tempz=enemy[index]->GetPosition().z;
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
void TargetMarker::Update_Tutorial(DebugCamera* camera)
{
	int tindex;
	float ex, ey, ez;
	tindex = NearEnemySearch(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL),PlayerControl::GetInstance()->GetPlayer() );
	if (tindex != -1) {
		GetEnemyPosition(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL), tindex, &ex, &ey, &ez);//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//マーカー位置をnowTargetに合わせる
	MarkerPosition = { ex,ey+1,ez };
	nowTarget = MarkerPosition;

	TargetMakerTexture->SetDisplayRadius(100);
	TargetMakerTexture->SetBillboard(FALSE);
	TargetMakerTexture->Update(camera);
	if (tindex != -1) {
		TargetMakerTexture->SetRotation({ 90,EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[tindex]->GetRotation().y ,0
			});
	}
	TargetMakerTexture->SetScale({ 3,3,3 });
	TargetMakerTexture->SetPosition(MarkerPosition);

}
void TargetMarker::Update_PlayScene(DebugCamera* camera)
{
	int tindex;
	float ex, ey, ez;
	tindex = NearEnemySearch(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE), PlayerControl::GetInstance()->GetPlayer());
	if (tindex != -1) {
		GetEnemyPosition(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE), tindex, &ex, &ey, &ez);//{ enemy[0]->GetPosition().x ,0, enemy[0]->GetPosition().z };
	}
	//マーカー位置をnowTargetに合わせる
	MarkerPosition = { ex,ey + 1,ez };
	nowTarget = MarkerPosition;

	TargetMakerTexture->SetDisplayRadius(100);
	TargetMakerTexture->SetBillboard(FALSE);
	TargetMakerTexture->Update(camera);
	if (tindex != -1) {
		TargetMakerTexture->SetRotation({ 90,EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[tindex]->GetRotation().y ,0
			});
	}
	TargetMakerTexture->SetScale({ 3,3,3 });
	TargetMakerTexture->SetPosition(MarkerPosition);

}

void TargetMarker::Draw()
{
	Texture::PreDraw();
	TargetMakerTexture->Draw();
	Texture::PostDraw();
}
void TargetMarker::Finalize()
{
	delete TargetMakerTexture;

}