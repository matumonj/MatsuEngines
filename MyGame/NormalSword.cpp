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
	Damage = 200;

	animetiontime = 1.6f;
	KnockPower = 18.0f;
	SwordObbScl = { 2.5f,2.5f,4.f };

	Rotation = {0.0f, 0.0f, 0.0f + 66.0f};
	m_Object->SetScale(Scale);
	const int Damage_Value = 10;
	const int CoolTime_Value = 180;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
}

void NormalSword::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	
	m_Object->Setf(FALSE);
	m_Object->SetDestFlag(FALSE);
	m_Object->SetRotation(Rotation);


	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);
}

void NormalSword::Draw()
{
	ImGui::Begin("rot");
	ImGui::SliderInt("Damage", &Damage, 0, 60);

	ImGui::End();
	Draw_Obj();
}
