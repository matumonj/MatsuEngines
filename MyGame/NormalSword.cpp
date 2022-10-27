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

	Rotation = { -18.0f, 18.0f,0.0f + 11.0f };
	
}

void NormalSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 5.0f;
	const float CoolTime_Value = 180.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	m_Object->Setf(FALSE);
	m_Object->SetDestFlag(FALSE);
	m_Object->SetRotation(Rotation);

	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1.0f,1.0f,1.0f,1.0f }, camera);

}

#include"imgui.h"
void NormalSword::Draw()
{
	Draw_Obj();
	ImGui::Begin("rot");
	ImGui::SliderFloat("x", &Rotation.x, -180, 180);
	ImGui::SliderFloat("y", &Rotation.y, -180, 180);
	ImGui::SliderFloat("z", &Rotation.z, -180, 180);
	ImGui::End();

}