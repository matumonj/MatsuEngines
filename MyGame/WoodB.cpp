#include "WoodB.h"
#include "CameraControl.h"
#include"SphereCollider.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"

WoodB::~WoodB()
{
}

WoodB::WoodB()
{
}

void WoodB::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	m_Object = std::make_unique<Object3d>();
	//モデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD2));
	Scale = {9.0f, 18.0f, 9.0f};
	radius_adjustment = 5.0f;
	SetCollider();
	alpha = 1.0f;
}

#include"Input.h"

void WoodB::Update()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	m_Object->SetColor({1.0f, 1.0f, 1.0f, 1.0f});


	Color = {1.0f, 1.0f, 1.0f, alpha};
	ParameterSet_Obj();
	m_Object->Setf(TRUE);
	//m_Object->Setppos(PlayerControl::GetIns()->GetPlayer()->GetPosition());
	//m_Object->setFog(true);

	m_Object->SetDisLen(800);
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		m_Object->setFog(true);
		m_Object->SetFogCenter(camera->GetEye());
		m_Object->Setppos(PlayerControl::GetIns()->GetPlayer()->GetPosition());
	}
	else
	{
		m_Object->setFog(false);
	}

	//m_Object->SetDisLen(800);
	//フィールド
	CollideWood();
}

void WoodB::Draw()
{
	Draw_Obj();
}

void WoodB::CollideWood()
{
	if (PlayerControl::GetIns()->GetPlayer() == nullptr)
	{
		return;
	}
	playerOBB.SetOBBParam_Pos(PlayerControl::GetIns()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({1.0f, 1.0f, 1.0f});
	playerOBB.SetOBBParam_Rot(PlayerControl::GetIns()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	woodOBB.SetOBBParam_Pos(m_Object->GetPosition());
	woodOBB.SetOBBParam_Scl({2.0f, 20.0f, 3.0f});
	woodOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), Position) < 20)
	{
		if (Collision::CheckOBBCollision(playerOBB, woodOBB) == true)
		{
			PlayerControl::GetIns()->GetPlayer()->isOldPos();
		}
	}
}
