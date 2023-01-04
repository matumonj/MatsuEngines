#include "NormalSword.h"

#include "CameraControl.h"
#include"PlayerControl.h"
#include"imgui.h"
#include"ModelManager.h"

NormalSword::~NormalSword()
{
	//	m_Object.reset();
}

void NormalSword::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//ƒ‚ƒfƒ‹Š„‚è“–‚Ä
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NORMALSWORD));

	Scale = {1.0f, 1.0f, 1.0f};
	Damage = 20;

	animetiontime = 1.6f;
	KnockPower = 20.0f;

	Rotation = {0.0f, 0.0f, 0.0f + 66.0f};
	m_Object->SetScale(Scale);
}

void NormalSword::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	const int Damage_Value = 10;
	const int CoolTime_Value = 180;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	m_Object->Setf(FALSE);
	m_Object->SetDestFlag(FALSE);
	m_Object->SetRotation(Rotation);

	
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);
}

void NormalSword::Draw()
{
	ImGui::Begin("sr");

	ImGui::SliderFloat("x", &Rotation.x, 0, 360);
	ImGui::SliderFloat("y", &Rotation.y, 0, 360);
	ImGui::SliderFloat("z", &Rotation.z, 0, 360);
	ImGui::End();
	Draw_Obj();
}
