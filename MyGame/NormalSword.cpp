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
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	//ƒ‚ƒfƒ‹Š„‚è“–‚Ä
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);

	Scale = { 1.0f,1.0f,1.0f };

	Rotation = { -18.0f, 18.0f,0.0f + 11.0f };
	
}

void NormalSword::Update(DebugCamera* camera)
{
	const int Damage_Value = 10;
	const int CoolTime_Value = 180;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	m_Object->Setf(FALSE);
	m_Object->SetDestFlag(FALSE);
	m_Object->SetRotation(Rotation);

	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1.0f,1.0f,1.0f,1.0f }, camera);

}

void NormalSword::Draw()
{
	Draw_Obj();
	
}