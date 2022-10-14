#include "BigSword.h"
#include"PlayerControl.h"
BigSword::~BigSword()
{
	//delete  m_Model;
}
void BigSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("BigSword");

	//���f�����蓖��
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);
	
	Scale = { 1.0f,1.0f,1.0f };

	Rotation={ 0.0f,30.0f,100.0f };

}

void BigSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 20.0f;
	const float CoolTime_Value = 180.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	//�t�B�[���h
	m_Object->SetRotation(Rotation);
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1,1,1,1 }, camera);
}

void BigSword::Draw()
{
	Draw_Obj();
}
