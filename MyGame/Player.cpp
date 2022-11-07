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

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("playerGolem"));
	m_fbxObject->PlayAnimation();


	SetCollider();

	Rotation = { -163.0f,-62.0f,103.0f };
	Position = { 0.0f,0.0f,0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };

	
	HP = MaxHP;
	vel /= 5.0f;
	rotate = RotationPrm::FRONT;

	//attackEffect = std::make_unique<AttackEffect>();
	AttackEffect::GetIns()->Init();
}
//80,145
void Player::Jump()
{
	if (onGround) {
		if (CustomButton::GetInstance()->GetJumpAction()) {
			onGround = false;
			const float jumpVYFist = 0.3f;
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
	if (StopFlag || evasionF)return;
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
		Gmove = move;
		
		if (f_time < AttackTime) {
			attackMotion = RUN;
		}
	}
	else {
		if (f_time < AttackTime) {
			attackMotion = NON;
		}
	}
		// �W�����v����
		Jump();
	
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
	if(m_Object==nullptr||m_fbxObject==nullptr)return;
	ReturnGround();

	//�P�t���[���O�̍��W��ۑ�
	oldpos = Position;

	RecvDamage_Cool();
	
	Move();
	//Gmove = move;
	if (input->TriggerButton(input->RB)) {
		evasionF = true;
	}
	Evasion();
	m_fbxObject->SetHandBoneIndex(hindex);
	m_fbxObject->SetFogPos(Position);
	//3d_fbx�X�V
	FbxAnimationControl();

	m_fbxObject->SetFbxTime(f_time);
	//3d�X�V
	CollisionField(camera);

	ParameterSet_Obj(camera);
	ParameterSet_Fbx(camera);
	
		//��̃{�[���擾
	HandMat = m_fbxObject->GetRot();

	SelectSword::GetInstance()->Update();

	AttackEffect::GetIns()->Upda();
}

void Player::RotationStatus()
{
	
}

void Player::Draw()
{
	Draw_Fbx();
	SelectSword::GetInstance()->SwordDraw();

	AttackEffect::GetIns()->Draw();
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

	/*�����Ƒҋ@���[�V�����ǂ����邩�������œƎ��ɍ��̂��肩�ǂ���*/
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
	if (evasionF||noAttack)return;
	float timespeed = 0.02f;

	if (CustomButton::GetInstance()->GetAttackAction() == true ) {
		AttackFlag = true;
		attackMotion = FIRST;
	}
	if (CustomButton::GetInstance()->Get2AttackAction() == true) {
		SecAttack = true;
		attackMotion = SECOND;
	}
	if (PlayerAttackState::GetInstance()->GetHitStopJudg()) {
		timespeed = 0.005f;
	}
	f_time += timespeed;
	FbxAnimationControls(FIRST, AttackTime, sectime+0.3);
	FbxAnimationControls(SECOND, sectime, m_fbxObject->GetEndTime());
	
}


XMMATRIX Player::GetMatrot()
{
	if (m_fbxObject != nullptr) {
		return m_fbxObject->GetMatrot();
	}
}
#include"CameraControl.h"
void Player::RecvDamage(int Damage)
{

	//�U���󂯂�����2�b�͖��G
	if (CoolTime != 0||evasionF)return;
	if (CameraControl::GetInstance()->GetCameraState() != CameraControl::PLAYER) return;

	if (!HUD::GetInstance()->GetRecvDamageFlag()) {
		HUD::GetInstance()->SetRecvDamageFlag(true);//�v���C���[HP��HUD�p
	}
	if (HP >= 0) {
		HP = HP - Damage;
	}
	recvdamage=true;
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
		CoolTime = 120;//���G����
	}
	//�J�E���g�J�n
	CoolTime--;

	CoolTime = min(CoolTime, 120);
	CoolTime = max(CoolTime, 0);
}

XMFLOAT3 Player::GetHandPos()
{
	XMFLOAT3 hPos;
	hPos = { HandMat.r[3].m128_f32[0] ,HandMat.r[3].m128_f32[1] ,HandMat.r[3].m128_f32[2] };
	return hPos;
}