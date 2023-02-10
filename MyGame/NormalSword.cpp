#include "NormalSword.h"

#include "CameraControl.h"
#include"PlayerControl.h"
#include"imgui.h"
#include"ModelManager.h"

NormalSword::~NormalSword()
{
		m_Object.reset();
}

void NormalSword::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	Scale = {1.0f, 1.0f, 1.0f};
	SwordObbScl = { 2.5f,2.5f,4.f };

	Rotation = {0.0f, 0.0f, 0.0f + 66.0f};
	
}

void NormalSword::Update()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	if (!LoadF)
	{
		if (m_Object == nullptr) {

			LoadCsv("ND", "NK", "NS", "NM");
			LoadF = true;
		}
	}
	if (m_Object != nullptr) {
		m_Object->Setf(FALSE);
		m_Object->SetDestFlag(FALSE);
		m_Object->SetRotation(Rotation);
		m_Object->SetScale(Scale);
		m_Object->Update(PlayerControl::GetIns()->GetPlayer()->GetHanMat(), { 1.0f, 1.0f, 1.0f, 1.0f }, camera);
	}
}
void NormalSword::Draw()
{
	if (m_Object == nullptr)return;
	Draw_Obj();
}
