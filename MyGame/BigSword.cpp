#include "BigSword.h"

#include "CameraControl.h"
#include"PlayerControl.h"

BigSword::~BigSword()
{
	m_Object.reset();
	//delete  m_Model;
}


void BigSword::Initialize()
{
	
	Scale = {1.0f, 1.0f, 1.0f};

	Rotation = {-20.0f, 30.0f, 15.0f};
}

void BigSword::Update()
{
	if (!LoadF)
	{
		if (m_Object == nullptr) {
			LoadCsv("BD", "BK", "BS", "BM");
			LoadF = true;
		}
	}
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	if (m_Object != nullptr) {
		m_Object->Setf(FALSE);
		//フィールド
		m_Object->SetRotation(Rotation);
		
		SwordObbScl = { 5.f,5.f,4.f };
		m_Object->SetScale(Scale);
		m_Object->Update(PlayerControl::GetIns()->GetPlayer()->GetHanMat(), { 1, 1, 1, 1 }, camera);
	}

}
#include"imgui.h"

void BigSword::Draw()
{
	if (m_Object == nullptr)return;
	Draw_Obj();
}
