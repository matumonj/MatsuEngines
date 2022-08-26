#include "SmallSword.h"
#include"PlayerControl.h"
void SmallSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("Sword");

	//�t�B�[���h�Ƀ��f�����蓖��
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);

	Scale = { 1,1,1 };

	Rotation = { 0,0 + 30,0 + 100 };

}

void SmallSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 5.0f;
	const float CoolTime_Value = 60.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	m_Object->SetParent(PlayerControl::GetInstance()->GetPlayer()->GetHanMat());
	//�t�B�[���h
	ParameterSet_Obj(camera);
}

void SmallSword::Draw()
{
	Draw_Obj();
}