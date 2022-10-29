#include "Player.h"
#include"Input.h"
#include"DebugTxt.h"
#include"SphereCollider.h"
#include"TargetMarker.h"
#include"Collision.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"CustomButton.h"
#include"BigSword.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"HUD.h"
#define PI 3.145265
using namespace DirectX;

Player::~Player()
{
	Destroy_unique( m_Object);
	//delete  m_Model;
}
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	return Player::GetInstance();
}

void Player::Initialize(DebugCamera* camera)
{
	StopFlag = false;
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
//	m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	//m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("playerGolem"));
	m_fbxObject->PlayAnimation();

	SetCollider();

	Rotation = { -163.0f,-62.0f,103.0f };
	Position = { 0.0f,0.0f,0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };

	SelectSword::GetInstance()->Initialize();
	
	HP = MaxHP;
	vel /= 5.0f;
	rotate = RotationPrm::FRONT;

}
//80,145
void Player::Jump()
{
	if (onGround) {
		if (CustomButton::GetInstance()->GetJumpAction()) {
			onGround = false;
			const float jumpVYFist = 0.5f;
			fallV = { 0, jumpVYFist, 0, 0 };
		}
	}
}

void Player::ReturnGround()
{
	if (onGround) {
		onGroundPos = Position;
		nogroundtime = 0;
	} else {
		nogroundtime++;
		if (nogroundtime > 120) {
			Position = onGroundPos;
			nogroundtime = 0;
		}
	}
}

void Player::Move()
{
	RotationStatus();

	XMFLOAT3 pos =Position;
	XMFLOAT3 rot = Rotation;

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float pi = 3.14159f;
	const float STICK_MAX = 32768.0f;

	
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f)) {
		if (input->TiltPushStick(Input::L_UP, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,vel,0 }, angle);
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,-vel,0 }, angle);
		}
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel,0,0,0 }, angle);
		}
		if (input->TiltPushStick(Input::L_LEFT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -vel,0,0,0 }, angle);
		}
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		rot.y = angle + atan2f(StickX, StickY) * (180.0f / pi);
		Rotation = { rot.x,rot.y-63.0f,rot.z };
		XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y+63.0f));
		move = XMVector3TransformNormal(move, matRot);

		Position.x += move.m128_f32[0] * movespeed;
		Position.z += move.m128_f32[2] * movespeed;

		
		if (f_time < AttackTime) {
			attackMotion = RUN;
		}
	}
	else {
		if (f_time < AttackTime) {
			attackMotion = NON;
		}
	}
		// ジャンプ操作
		Jump();
	
	//Gmove = move;
	if (input->TriggerButton(input->RB)) {
		evasionF = true;
	}
	Evasion();
}

#include"mHelper.h"
void Player::Evasion()
{
	if (evasionF) {
		if (evaTime < 1.0f) {
			evaTime += 0.03f;
			Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
			Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, 15.0f, 0.0f);

		}
		//m_fbxObject->SetColor({ 0,0,0,0 });
		if (f_time <= EvaTime_Start) {
			f_time = EvaTime_Start;
		}
		if (f_time >= EvaTime_End) {
			evasionF = false;
		}
		f_time += 0.02f;
	}
	else {
		m_fbxObject->SetColor({ 1,1,1,1 });
		evaTime = 0.0f;
	}
}

void Player::Update(DebugCamera* camera)
{
	ReturnGround();

	//１フレーム前の座標を保存
	oldpos = Position;

	RecvDamage_Cool();
	
	Move();

	m_fbxObject->SetHandBoneIndex(hindex);
	//3d_fbx更新
	FbxAnimationControl();

	m_fbxObject->SetFbxTime(f_time);
	//3d更新
	CollisionField(camera);

	ParameterSet_Obj(camera);
	ParameterSet_Fbx(camera);
	
	
	//手のボーン取得
	HandMat = m_fbxObject->GetRot();

	SelectSword::GetInstance()->Update();
}

void Player::RotationStatus()
{
	
}

#include"imgui.h"
void Player::Draw()
{
	Draw_Fbx();
	SelectSword::GetInstance()->SwordDraw();


	ImGui::Begin("we");
	ImGui::SliderFloat("rx", &Rotation.x,-360,360);
	ImGui::SliderFloat("ry", &Rotation.y, -360, 360);
	ImGui::SliderFloat("rz", &angle, -360, 360);

	ImGui::SliderInt("HandBone", &hindex, 0, 30);
	ImGui::SliderFloat("at", &sectime, 0, 20);
	ImGui::End();
}


void Player::FbxAnimationControls(const AttackMotion& motiontype,const float attacktime,const float nextAnimationtime)
{
	//if (evasionF)return;

	if (attackMotion == motiontype) {
		if (f_time <= attacktime) {
			f_time = attacktime;
		}
		if (f_time >= nextAnimationtime) {
			attackMotion = RUN;
		}
	}

	/*歩きと待機モーションどうするかこっちで独自に作るのありかどうか*/
	if (attackMotion == NON) {
		f_time = 0;
	}
	if (attackMotion == RUN) {
		if (f_time >= AttackTime) {
			f_time = 0;
		}

	}
}

void Player::FbxAnimationControl()
{
	if (evasionF)return;
	const float timespeed = 0.02f;

	if (CustomButton::GetInstance()->GetAttackAction() == true && PlayerAttackState::GetInstance()->GetCoolTime() == 0) {
		AttackFlag = true;
		attackMotion = FIRST;
	}
	if (CustomButton::GetInstance()->Get2AttackAction() == true && PlayerAttackState::GetInstance()->GetCoolTime() == 0) {
		SecAttack = true;
		attackMotion = SECOND;
	}

	f_time += timespeed;

	FbxAnimationControls(FIRST, AttackTime, sectime);
	FbxAnimationControls(SECOND, sectime, m_fbxObject->GetEndTime());
	
}


XMMATRIX Player::GetMatrot()
{
	if (m_fbxObject != nullptr) {
		return m_fbxObject->GetMatrot();
	}
}

void Player::RecvDamage(int Damage)
{
	//攻撃受けたあと2秒は無敵
	if (CoolTime != 0||evasionF)return;
	if (!HUD::GetInstance()->GetRecvDamageFlag()) {
		HUD::GetInstance()->SetRecvDamageFlag(true);//プレイヤーHPのHUD用
	}
	if (HP >= 0) {
		HP = HP - Damage;
	}
}

XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle) {
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}

void Player::RecvDamage_Cool()
{
	if (HUD::GetInstance()->GetRecvDamageFlag()) {
		CoolTime = 120;//無敵時間
	}
	//カウント開始
	CoolTime--;

	CoolTime = min(CoolTime, 120);
	CoolTime = max(CoolTime, 0);
}