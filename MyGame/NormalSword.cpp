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

	animetiontime = 0.05f;
	KnockPower = 20.0f;

	Rotation = {-18.0f, 18.0f, 0.0f + 11.0f};
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

	m_Object->SetScale(Scale);
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);
}

void NormalSword::Draw()
{
	Draw_Obj();
}
