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
	//m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();

	SetCollider();

	Rotation = { 0.0f,0.0f,0.0f };
	Position = { 0.0f,0.0f,0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };

	SelectSword::GetInstance()->Initialize();
	
	HP = MaxHP;

	rotate = RotationPrm::FRONT;

}

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

	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (!StopFlag) {
		if (input->Pushkey(DIK_W) || input->Pushkey(DIK_A) || input->Pushkey(DIK_D) || input->Pushkey(DIK_S)
			|| (input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Down))) {
			Position.x += move.m128_f32[0] * movespeed;
			Position.z += move.m128_f32[2] * movespeed;
		}
		// ジャンプ操作
		Jump();
	}
	Gmove = move;
}

void Player::Update(DebugCamera* camera)
{
	ReturnGround();

	//１フレーム前の座標を保存
	oldpos = Position;

	RecvDamage_Cool();
	
	Move();

	//3d_fbx更新
	FbxAnimationControl();

	//3d更新
	ParameterSet_Obj(camera);
	ParameterSet_Fbx(camera);
	
	CollisionField(camera);

	//手のボーン取得
	HandMat = m_fbxObject->GetRot();

	SelectSword::GetInstance()->Update();
}

void Player::RotationStatus()
{
	if (StopFlag)return;
	//左方向への移動
	if (rotate != RotationPrm::LEFT && input->LeftTiltStick(input->Left)) {//今向いている方向が左じゃなくAキーが押され、
		if (rotate == RotationPrm::FRONT) {//右以外を向いていたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::BACK) {//右を向いていたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::RIGHT) {
			Rotation.y = Rotation.y - 180;
		}
		//向きの情報を左に
		rotate = RotationPrm::LEFT;
	}

	//右方向への移動
	else if (rotate != RotationPrm::RIGHT && input->LeftTiltStick(input->Right)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::FRONT) {//右以外を向いていたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {//右を向いていたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {
			Rotation.y = Rotation.y + 180;
		}
		//向きの情報を右に
		rotate = RotationPrm::RIGHT;
	}

	//前方向への移動
	if (rotate != RotationPrm::FRONT && input->LeftTiltStick(input->Up)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::RIGHT) {//もし右を向いてたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {//もし左を向いてたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {
			Rotation.y -= 180;
		}
		//向きの情報を前に
		rotate = RotationPrm::FRONT;
	}

	//後ろ方向へ移動
	if (rotate != RotationPrm::BACK && input->LeftTiltStick(input->Down)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::RIGHT) {//もし右を向いてたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::LEFT) {//もし左を向いてたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::FRONT) {
			Rotation.y += 180;
		}
		//向きの情報を前に
		rotate = RotationPrm::BACK;
	}
}


void Player::Draw()
{
	Draw_Fbx();
	SelectSword::GetInstance()->SwordDraw();
}

void Player::FbxAnimationControl()
{
	const float timespeed = 0.02f;

	if (CustomButton::GetInstance()->GetAttackAction() == true) {
			AttackFlag = true;
	}
	if (AttackFlag) {
		f_time = AttackTime;
		AttackFlag = false;
		nowattack = true;
	}


	f_time += timespeed;
	if (nowattack) {
		if (f_time >= DeathTime) {
			f_time = 0;
			nowattack = false;
		}
	} else {//歩くモーション　一つのアニメーションで回してるせいで攻撃モーション行く前にタイムを0に戻す
		if (f_time > AttackTime) {
			f_time = 0;
		}
	}
	m_fbxObject->SetFbxTime(f_time);
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
	if (CoolTime != 0)return;
	if (!HUD::GetInstance()->GetRecvDamageFlag()) {
		HUD::GetInstance()->SetRecvDamageFlag(true);//プレイヤーHPのHUD用
	}
	if (HP >= 0) {
		HP = HP - Damage;
	}
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