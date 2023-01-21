#include "ThrowRockAttack.h"

#include "CameraControl.h"
#include "EnemyControl.h"
#include "PlayerControl.h"
ThrowRockAttack* ThrowRockAttack::GetInstance()
{
	static ThrowRockAttack ins;
	return &ins;
}
void ThrowRockAttack::Init()
{
	camera = CameraControl::GetInstance()->GetCamera();
	//石オブジェの初期化
	ThrowRockObj = std::make_unique<Object3d>();
	ThrowRockObj->Initialize(camera);
	ThrowRockObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::THROWROCK));
	ThrowRockObj->SetScale({ 2.f, 2.f, 2.f });
	for(int i=0;i<DestRock.size();i++)
	{
		DestRock[i] = std::make_unique<Object3d>();
		DestRock[i]->Initialize(camera);
		DestRock[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::DESTROCK));
		DestRock[i]->SetScale({ 1.2f, 1.2f, 1.2f });

	}
	tmotion = ThrowMotion::END;
}

void ThrowRockAttack::Action()
{
	//石オブジェの更新
	ThrowRockObj->SetPosition(RockPos);
	ThrowRockObj->Update({ 1.f, 1.f, 1.f, 1.f }, camera);

	DestEffect();

	for (int i = 0; i < DestRock.size(); i++)
	{
		DestRock[i]->SetColor({ 1.f,1.f,1.f,DestRockAlpha[i] });
		DestRock[i]->SetPosition(DestRockPos[i]);
		DestRock[i]->Update({ 1.f, 1.f, 1.f, 1.f }, camera);
	}
	ActionUpda();
}

void ThrowRockAttack::Draw()
{
	Object3d::PreDraw();
	if (destF)
	{
		ThrowRockObj->Draw();
	}
	for (int i = 0; i < DestRock.size(); i++)
	{
		if (DestRockAlpha[i] < 0.f)continue;
		DestRock[i]->Draw();
	}
	Object3d::PostDraw();
}
void ThrowRockAttack::DestEffect()
{
	
		for (int i = 0; i < DestRock.size(); i++)
		{
			if (destF) {
				DestRockAcc[i] += 0.002f;
			DestRockPos[i].x += 0.1f * cosf(float(i)*360.f); //360度に広がるようにする
			DestRockPos[i].y += 0.1f * sinf(float(i) *360.f)-DestRockAcc[i];

			DestRockAlpha[i] -= 0.01f;
		}
			else
			{
				DestRockAlpha[i] = 1.f;
				DestRockAcc[i] = 0.f;
				DestRockPos[i] = ThrowRockObj->GetPosition();
			}
			
		}

}


void ThrowRockAttack::ThrowPhase_Set()
{
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();

	if (  boss->GetAnimationTime() > 2.5)
	{
		tmotion = PICK;
	}
}
void ThrowRockAttack::ThrowPhase_Pick()
{
	Enemy*boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	
		if (boss->GetAnimationTime() > 2.5)
		{
			tmotion = THROW;
		}
		RockPos = boss->HandLeftPos();

		destF = false;
}


void ThrowRockAttack::ThrowPhase_Throw()
{
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	//右手とプレイヤーのあたりはんてい
	XMFLOAT3 l_playerpos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();


	if (Collision::GetLength(l_playerpos, RockPos) < 20.f)
	{
		destF = true;
		if (!turnoffdrawF) {
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
		}
	}
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y+180.f));
	move = XMVector3TransformNormal(move, matRot);
	if (!destF)
	{
		//向いた方向に進む
		RockPos.y -= 0.1f;
		RockPos.x += move.m128_f32[0] * 15.f;
		RockPos.z += move.m128_f32[2] * 15.f;
	}
	if (boss->GetAnimationTime() >= boss->GetFbxTimeEnd() - 0.3f)
	{
		tmotion = END;
	}
}
void (ThrowRockAttack::* ThrowRockAttack::actionTable[])() = {
	nullptr,
	& ThrowRockAttack::ThrowPhase_Set,
	& ThrowRockAttack::ThrowPhase_Pick,
	& ThrowRockAttack::ThrowPhase_Throw, 
	& ThrowRockAttack::ThrowPhase_End,
};

#include"SceneManager.h"

void ThrowRockAttack::ActionUpda()
{
	//シーンに応じた更新処理
	(this->*actionTable[tmotion])();
}
void ThrowRockAttack::ThrowPhase_End()
{
	//destF = false;

}






