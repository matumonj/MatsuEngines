#include "AttackCollision.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include"Collision.h"
void AttackCollision::Init()
{
	cModel = Model::CreateFromOBJ("chest");
	cObj = new Object3d();
	cObj->Initialize(CameraControl::GetInstance()->GetCamera());
	cObj->SetModel(cModel);
	Position = { PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
	PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
	PlayerControl::GetInstance()->GetPlayer()->GetPosition().z };
	input = Input::GetInstance();
}

void AttackCollision::Update()
{
	
	if (input->Pushkey(DIK_W) || input->Pushkey(DIK_A) || input->Pushkey(DIK_D) || input->Pushkey(DIK_S)
		|| (input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Down))) {
		Position.x += PlayerControl::GetInstance()->GetPlayer()->GetMove().m128_f32[0]*3;
		Position.z += PlayerControl::GetInstance()->GetPlayer()->GetMove().m128_f32[2]*3;
	}
	AttackArea.position = { Position.x,Position.z };
	AttackArea.scale = { 30,30 };
	EnemyArea.position = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().x,
		EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().z };
	EnemyArea.scale = { 10,10 };
	if (Collision::CheckBox2Box(AttackArea, EnemyArea)==true) {
		EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->Setcol({ 0,1,0,1 });
	}
else if (Collision::CheckBox2Box(AttackArea, EnemyArea) == false) {

		EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->Setcol({ 1,1,1,1 });
	}
	
	cObj->SetScale({ 5,5,5 });
	cObj->SetPosition(Position);
	cObj->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
}

void AttackCollision::Draw()
{
	cObj->PreDraw();
	cObj->Draw();
	cObj->PostDraw();
}