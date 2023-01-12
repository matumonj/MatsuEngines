#include "House.h"

#include "CameraControl.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"

House::~House()
{
}

House::House()
{
}

void House::Initialize()
{
}

void House::Update()
{
}

void House::Draw()
{
}

bool House::CollideHouse()
{
	return false;
}

void House_A::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);

	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::HOUSE));
	Scale = {1.5f, 1.5f, 2.2f};
	radius_adjustment = -5.f;
	SetCollider();
	alpha = 1.f;
}

void House_A::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object->SetColor({0.4f, 0.4f, 0.4f, 1.f});


	ParameterSet_Obj();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		m_Object->setFog(TRUE);
		m_Object->SetFogCenter(camera->GetEye());
	}
	//フィールド
	CollideHouse();
}

void House_A::Draw()
{
	Draw_Obj();
}

bool House_A::CollideHouse()
{
	if (PlayerControl::GetInstance()->GetPlayer() == nullptr)
	{
		return false;
	}
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({1.f, 1.f, 1.f});
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	HouseOBB.SetOBBParam_Pos(m_Object->GetPosition());
	HouseOBB.SetOBBParam_Scl({40.f, 30.f, 40.f});
	HouseOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) < 40)
	{
		if (Collision::CheckOBBCollision(playerOBB, HouseOBB) == true)
		{
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			return true;
		}
		return false;
	}
	return false;
}


void House_B::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGROCK_A));
	Scale = {3.f, 3.f, 3.f};
	radius_adjustment = -14.f;
	SetCollider();
	alpha = 1.f;
}

void House_B::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object->SetColor({0.4f, 0.4f, 0.4f, 1.f});


	Color = {1.f, 1.f, 1.f, alpha};

	ParameterSet_Obj();

	//フィールド
	CollideHouse();
}

void House_B::Draw()
{
	Draw_Obj();
}

bool House_B::CollideHouse()
{
	if (PlayerControl::GetInstance()->GetPlayer() == nullptr)
	{
		return false;
	}
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({1.f, 1.f, 1.f});
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	HouseOBB.SetOBBParam_Pos(m_Object->GetPosition());
	HouseOBB.SetOBBParam_Scl({40.f, 50.f, 40.f});
	HouseOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) < 140)
	{
		if (Collision::CheckOBBCollision(playerOBB, HouseOBB) == true)
		{
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			return true;
		}
		return false;
	}
	return false;
}
