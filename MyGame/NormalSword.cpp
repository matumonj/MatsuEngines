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

	//ƒ‚ƒfƒ‹Š„‚è“–‚Ä
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);

	Scale = { 1.0f,1.0f,1.0f };

	Rotation = { 0.0f,0.0f + 30.0f,0.0f + 100.0f };
	
}

void NormalSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 5.0f;
	const float CoolTime_Value = 90.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;

	m_Object->SetRotation(Rotation);

	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1,1,1,1 }, camera);

}

void NormalSword::Draw()
{
	ImGui::Begin("SROT");
	ImGui::Text("%f", m_Object->ExtractPositionMat().r[2].m128_f32[2]);
	ImGui::End();
	Draw_Obj();
	
}