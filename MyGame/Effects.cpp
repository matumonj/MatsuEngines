#include "Effects.h"
#include"Player.h"
#include"PlayerAttackState.h"
#include"TargetMarker.h"
#include"Collision.h"
#include"EnemyControl.h"
Effects* Effects::GetInstance()
{
	static Effects instance;

	return &instance;
}
void Effects::Initialize(DebugCamera* camera)
{
	//�G�t�F�N�g�̃C���X�^���X����
	efk = new mEffekseer();
	//efk1 = new mEffekseer();

	//�G�t�F�N�g�̃Z�b�g(3��������)
	efk->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/deadef.efk", (const EFK_CHAR*)L"effect/10");
	//efk1->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/SimpleLaser.efk", (const EFK_CHAR*)L"effect/10");
}

void Effects::Update(DebugCamera* camera)
{
	//�G�t�F�N�g�̃p�����[�^�Z�b�g
	int index = TargetMarker::GetInstance()->GetNearIndex();
	float posx = EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition().x;
		float posy= EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition().y;
		float posz= EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition().z;
	efk->SetPosition(posx,posy,posz);
	efk->SetRotation(0, 0, 0);
	efk->SetScale(0.001, 0.001, 0.001);
	//efk1->SetPosition(-10, 0, 190);
	//if (Input::GetInstance()->PushButton(Input::GetInstance()->Button_RB)) {
		
	//�G�t�F�N�g�̍Đ�
	//if (Input::GetInstance()->Pushkey(DIK_SPACE)) {
	
	if (Collision::GetLength(Player::GetInstance()->GetPosition(), EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition() )<25&& Input::GetInstance()->PushButton(Input::GetInstance()->Button_B)) {
		ff = true;
	}
	if (PlayerAttackState::GetInstance()->GetNowSkill() == PlayerAttackState::GetInstance()->Third) {
		tf = true;
	}
	
	if(ff){
	efk->Load_Effect();
	ff = false;
	}
	//efk1->Load_Effect();

	//�G�t�F�N�g�̃A�j���[�V�����~�߂�
	if (Input::GetInstance()->Pushkey(DIK_C)) {
		efk->Stop_Effect();
	}

	//view,projection�s����G�t�F�N�g�̃e�N�X�`���ɂ�����
	efk->EffekseerUpdate(camera);
	//efk1->EffekseerUpdate(camera);

}

void Effects::Draw()
{
	//�G�t�F�N�g�̉摜
	efk->EffekseerDraw();
	//efk1->EffekseerDraw();
}