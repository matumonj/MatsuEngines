#include "Wood.h"
#include "Wood.h"

#include "CameraControl.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"

Wood::~Wood()
{
}

Wood::Wood()
{
}

void Wood::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object = std::make_unique<Object3d>();
	//モデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD));
	Scale = {12.0f, 9.0f, 12.0f};
	radius_adjustment = 2.0f;
	SetCollider();
	alpha = 1.0f;
}

#include"Input.h"

void Wood::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object->SetColor({1.0f, 1.0f, 1.0f, 1.0f});


	Color = {1.0f, 1.0f, 1.0f, alpha};
	ParameterSet_Obj();
	m_Object->Setf(TRUE);
	m_Object->SetFogCenter(camera->GetEye());
	m_Object->setFog(true);
	m_Object->SetDisLen(800);
	//フィールド
	CollideWood();
}

void Wood::Draw()
{
	Draw_Obj();
}

void Wood::CollideWood()
{
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();
	if (l_player == nullptr)
	{
		return;
	}
	playerOBB.SetOBBParam_Pos(l_player->GetPosition());
	playerOBB.SetOBBParam_Scl({1.0f, 1.0f, 1.0f});
	playerOBB.SetOBBParam_Rot(l_player->GetMatrot());

	//OBB 回転ベクトル
	woodOBB.SetOBBParam_Pos(m_Object->GetPosition());
	woodOBB.SetOBBParam_Scl({6.0f, 20.0f, 6.0f});
	woodOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(l_player->GetPosition(), Position) < 20)
	{
		if (Collision::CheckOBBCollision(playerOBB, woodOBB) == true)
		{
			l_player->isOldPos();
		}
	}
}
