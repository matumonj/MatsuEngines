#include "BigSword.h"
#include"PlayerControl.h"

BigSword::~BigSword()
{
	//m_Object.reset();
	//delete  m_Model;
}

void BigSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	
	//モデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));

	Scale = {1.0f, 1.0f, 1.0f};

	Rotation = {-20.0f, 30.0f, 15.0f};
}

void BigSword::Update(DebugCamera* camera)
{
	const int Damage_Value = 30;
	const int CoolTime_Value = 180;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	m_Object->Setf(FALSE);
	//フィールド
	m_Object->SetRotation(Rotation);
	animetiontime = 0.02f;
	KnockPower = 40.0f;
	m_Object->SetScale(Scale);
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), {1, 1, 1, 1}, camera);
}

#include"imgui.h"
void BigSword::Draw()
{
	ImGui::Begin("Rot");
	ImGui::SliderFloat("X", &Rotation.x, 0, 360);
	ImGui::SliderFloat("Y", &Rotation.y, 0, 360);

	ImGui::SliderFloat("Z", &Rotation.z, 0, 360);

	
	Draw_Obj();
}
