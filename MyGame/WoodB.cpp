#include "WoodB.h"
#include "WoodB.h"
#include "WoodB.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"

WoodB::~WoodB()
{
}

WoodB::WoodB()
{
}

void WoodB::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	//モデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD2));
	Scale = { 9.0f, 18.0f, 9.0f };
	radius_adjustment = 5.0f;
	SetCollider();
	alpha = 1.0f;
}

#include"Input.h"

void WoodB::Update(DebugCamera* camera)
{
	m_Object->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });


	Color = { 1.0f, 1.0f, 1.0f, alpha };
	ParameterSet_Obj(camera);
	m_Object->Setf(TRUE);
	m_Object->SetFogCenter(camera->GetEye());
	m_Object->setFog(true);

	m_Object->SetDisLen(800);
	//フィールド
	CollideWood();
}

void WoodB::Draw()
{
	Draw_Obj();
}

void WoodB::CollideWood()
{
	if (PlayerControl::GetInstance()->GetPlayer() == nullptr)
	{
		return;
	}
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({ 1.0f, 1.0f, 1.0f });
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	woodOBB.SetOBBParam_Pos(m_Object->GetPosition());
	woodOBB.SetOBBParam_Scl({ 2.0f, 20.0f, 3.0f });
	woodOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) < 20)
	{
		if (Collision::CheckOBBCollision(playerOBB, woodOBB) == true)
		{
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			
		}
	}
}
