#include "NormalSword.h"
#include"PlayerControl.h"
#include"imgui.h"
NormalSword::~NormalSword()
{
	//delete  m_Model;
}
void NormalSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("sword");

	//モデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);

	Scale = { 1.0f,1.0f,1.0f };

	Rotation = { 0.0f,0.0f + 30.0f,0.0f + 100.0f };
	
}

void NormalSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 5.0f;
	const float CoolTime_Value = 180.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;

	m_Object->SetRotation(Rotation);

	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1.0f,1.0f,1.0f,1.0f }, camera);

}

void NormalSword::Draw()
{
	Draw_Obj();
}