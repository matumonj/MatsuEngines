#include "Stone.h"
#include "Stone.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"
Stone::~Stone()
{

}
void Stone::Initialize(DebugCamera* camera)
{

 }
void Stone::Update(DebugCamera* camera)
{

 }
void Stone::Draw()
{

}
bool Stone::CollideStone()
{
	return false;
 }
void Stone_A::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);

	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGROCK_A));
	Scale = { 3,3,3};
	radius_adjustment = -5;
	SetCollider();
	alpha = 1;
}
void Stone_A::Update(DebugCamera* camera)
{
	m_Object->SetColor({ 1,1,1,1 });


	Color = { 1,1,1,alpha };
	ParameterSet_Obj(camera);
	m_Object->Setf(TRUE);
	
	//フィールド
	CollideStone();

}

void Stone_A::Draw()
{
	Draw_Obj();
}
bool Stone_A::CollideStone()
{
	if (PlayerControl::GetInstance()->GetPlayer() == nullptr)return false;
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({ 1,1,1 });
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	StoneOBB.SetOBBParam_Pos(m_Object->GetPosition());
	StoneOBB.SetOBBParam_Scl({Scale.x+10,Scale.y,Scale.z+10});
	StoneOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) < 40) {
		if (Collision::CheckOBBCollision(playerOBB, StoneOBB) == true) {
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			return true;
		} else {
			return false;
		}
	}

}


void Stone_B::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGROCK_A));
	Scale = { 3,3,3 };
	radius_adjustment = -14;
	SetCollider();
	alpha = 1;
}
void Stone_B::Update(DebugCamera* camera)
{
	m_Object->SetColor({ 1,1,1,1 });


	Color = { 1,1,1,alpha };

	ParameterSet_Obj(camera);
	
	//フィールド
	CollideStone();

}

void Stone_B::Draw()
{
	Draw_Obj();
}
bool Stone_B::CollideStone()
{
	if (PlayerControl::GetInstance()->GetPlayer() == nullptr)return false;
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Scl({ 1,1,1 });
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());

	//OBB 回転ベクトル
	StoneOBB.SetOBBParam_Pos(m_Object->GetPosition());
	StoneOBB.SetOBBParam_Scl({ 40,50,40 });
	StoneOBB.SetOBBParam_Rot(m_Object->GetMatrot());

	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) < 140) {
		if (Collision::CheckOBBCollision(playerOBB, StoneOBB) == true) {
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			return true;
		} else {
			return false;
		}
	}

}
