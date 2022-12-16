#include "House.h"
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

void House::Initialize(DebugCamera* camera)
{
}

void House::Update(DebugCamera* camera)
{
}

void House::Draw()
{
}

bool House::CollideHouse()
{
	return false;
}

void House_A::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);

	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::HOUSE));
	Scale = {1.5, 1.5, 2.2};
	radius_adjustment = -5;
	SetCollider();
	alpha = 1;
}

void House_A::Update(DebugCamera* camera)
{
	m_Object->SetColor({0.4, 0.4, 0.4, 1});


	ParameterSet_Obj(camera);
	m_Object->setFog(TRUE);
	m_Object->SetFogCenter(camera->GetEye());

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
	playerOBB.SetOBBParam_Scl({1, 1, 1});
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	HouseOBB.SetOBBParam_Pos(m_Object->GetPosition());
	HouseOBB.SetOBBParam_Scl({40, 30, 40});
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


void House_B::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGROCK_A));
	Scale = {3, 3, 3};
	radius_adjustment = -14;
	SetCollider();
	alpha = 1;
}

void House_B::Update(DebugCamera* camera)
{
	m_Object->SetColor({0.4, 0.4, 0.4, 1});


	Color = {1, 1, 1, alpha};

	ParameterSet_Obj(camera);

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
	playerOBB.SetOBBParam_Scl({1, 1, 1});
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	HouseOBB.SetOBBParam_Pos(m_Object->GetPosition());
	HouseOBB.SetOBBParam_Scl({40, 50, 40});
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
