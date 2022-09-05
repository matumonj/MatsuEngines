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
XMFLOAT3 Player::Effect_Pos = { -50,-10,-100 };

Player::~Player()
{
	delete fbxmodel;
	delete  m_Model;
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
	HP = MaxHP;
	rotate = RotationPrm::FRONT;

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(m_fbxModel);
	m_fbxObject->PlayAnimation();

	SetCollider();

	Rotation = { -90,0,0 };
	Position = { 0,-30,0 };
	SelectSword::GetInstance()->Initialize();
}
void Player::Update(DebugCamera* camera)
{
	oldpos = Position;
	//�G�t�F�N�g�̃p�����[�^�Z�b�g
	RotationStatus();
	//Position.y = -2;
	Scale={ 0.02, 0.02f, 0.02f
		};
	//
	// 
	//Position = { 92,-27,-760 };
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (input->Pushkey(DIK_W) || input->Pushkey(DIK_A) || input->Pushkey(DIK_D) || input->Pushkey(DIK_S)
		|| (input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Down))) {
		Position.x += move.m128_f32[0] * movespeed;
		Position.z += move.m128_f32[2] * movespeed;
		
		stopf = false;
	} else {
		stopf = true;
		
	}
	Gmove = move;
	FbxAnimationControl();
	//�t�B�[���h
	ParameterSet_Obj(camera);
	//�t�B�[���h
	ParameterSet_Fbx(camera);

	CollisionField(camera);

	HandMat = m_fbxObject->GetRot();
	SelectSword::GetInstance()->Update();
}

void Player::RotationStatus()
{

	//�������ւ̈ړ�
	if (rotate != RotationPrm::LEFT && input->LeftTiltStick(input->Left)) {//�������Ă��������������Ȃ�A�L�[��������A
		if (rotate == RotationPrm::FRONT) {//�E�ȊO�������Ă�����
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::BACK) {//�E�������Ă�����
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::RIGHT) {
			Rotation.y = Rotation.y - 180;
		}
		//�����̏�������
		rotate = RotationPrm::LEFT;
	}

	//�E�����ւ̈ړ�
	else if (rotate != RotationPrm::RIGHT && input->LeftTiltStick(input->Right)) {//�������Ă���������E����Ȃ��c�L�[��������A
		if (rotate == RotationPrm::FRONT) {//�E�ȊO�������Ă�����
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {//�E�������Ă�����
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {
			Rotation.y = Rotation.y + 180;
		}
		//�����̏����E��
		rotate = RotationPrm::RIGHT;
	}

	//�O�����ւ̈ړ�
	if (rotate != RotationPrm::FRONT && input->LeftTiltStick(input->Up)) {//�������Ă���������E����Ȃ��c�L�[��������A
		if (rotate == RotationPrm::RIGHT) {//�����E�������Ă���
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {//�������������Ă���
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {
			Rotation.y -= 180;
		}
		//�����̏���O��
		rotate = RotationPrm::FRONT;
	}

	//�������ֈړ�
	if (rotate != RotationPrm::BACK && input->LeftTiltStick(input->Down)) {//�������Ă���������E����Ȃ��c�L�[��������A
		if (rotate == RotationPrm::RIGHT) {//�����E�������Ă���
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::LEFT) {//�������������Ă���
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::FRONT) {
			Rotation.y += 180;
		}
		//�����̏���O��
		rotate = RotationPrm::BACK;
	}
}


void Player::Draw()
{
	Draw_Fbx();
	SelectSword::GetInstance()->SwordDraw();
}

void Player::ImguiDraw()
{
	//ImGui::Begin("Player_State");
	//if (ImGui::TreeNode("Position")) {

	//	ImGui::Text(" PositionX   [%5f]", Position.x);
	//	ImGui::Text(" PositionY   [%5f]", Position.y);
	//	ImGui::Text(" PositionZ   [%5f]", Position.z);

	//}
	////Rotation
	//if (ImGui::TreeNode("Rotation")) {
	//	XMMATRIX sub = m_fbxObject->GetPos();

	//	XMFLOAT3 rots = { sub.r[3].m128_f32[0],sub.r[3].m128_f32[1], sub.r[3].m128_f32[2] };
	//	ImGui::Text(" RotationX   [%5f]", rots.x);
	//	ImGui::Text(" RotationY   [%5f]", rots.y);
	//	ImGui::Text(" RotationZ   [%5f]", rots.z);
	//	ImGui::TreePop();
	//}
	//ImGui::SliderInt("HP", &HP, 1, MaxHP);
	//ImGui::SliderFloat("MoveSpeed", &movespeed, 1, 15);
	////�������������Ƃ��n�ʂɖ߂��Ă��� 1�b�O�̐ڒn�ʒu�ɖ߂��Ă���
	//PosSavetime++;
	//if (PosSavetime > 60) {
	//	PosSavetime = 0;
	//}
	//if (ImGui::RadioButton("ReturnGround", &ReturnGround)) {
	//	Position = OldPos_Onground;
	//	ReturnGround = false;
	//}

	ImGui::End();
}

void Player::FbxAnimationControl()
{
	if (HUD::GetInstance()->GetSpriteSize(0) <=10.0f) {//���̂������Ƃŏ���

	if (CustomButton::GetInstance()->GetAttackAction() == true) {
			AttackFlag = true;
		}
		}
	if (AttackFlag) {
		f_time = AttackTime;
		AttackFlag = false;
		nowattack = true;
	}


	f_time += 0.02f;
	if (nowattack) {
		if (f_time >= DeathTime) {
			f_time = 0;
			nowattack = false;
		}
	} else {
		if (f_time > AttackTime) {
			f_time = 0;
		}
	}


	m_fbxObject->SetFbxTime(f_time);
}

void Player::SetColors(XMFLOAT4 color)
{
	m_fbxObject->SetColor(color);
}

XMMATRIX Player::GetMatrot()
{
	if (m_fbxObject != nullptr) {
		return m_fbxObject->GetMatrot();
	}
}