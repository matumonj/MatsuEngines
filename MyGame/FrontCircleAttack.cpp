#include "FrontCircleAttack.h"

#include <algorithm>

#include"EnemyControl.h"
#include"CameraControl.h"
#include"BossSpell.h"
#include "PlayerControl.h"

FrontCircleAttack* FrontCircleAttack::GetIns()
{
	static FrontCircleAttack instance;

	return &instance;
}

void FrontCircleAttack::Init()
{
	Texture::LoadTexture(26, L"Resources/2d/icon/missileicon.png");
	
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	for (auto i = 0; i < nail_objses_.size(); i++) {
		//�~�T�C���������̃G�t�F�N�g
		nail_objses_[i] .PireEffect= std::make_unique<Particle>();
		nail_objses_[i].PireEffect->Init(Particle::SMOKE);
		nail_objses_[i].PireEffect->SetParScl({ 4.f, 4.f });
		nail_objses_[i].PireEffect->SetParColor({ 1.f, 1.f, 1.f, 1.f });

		//�e�N�X�`��
		nail_objses_[i].AOETexs.reset(Texture::Create(26));
		nail_objses_[i].AOETexs->CreateTexture();
		nail_objses_[i].AOETexs->SetAnchorPoint({ 0.5f,0.5f });
		//�I�u�W�F
		nail_objses_[i].Obj.reset(new Object3d());
		nail_objses_[i].Obj->Initialize(camera);
		nail_objses_[i].Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
		nail_objses_[i].TexAlpha = 0.f;
	}
	phase = PHASENON;
}
void (FrontCircleAttack::* FrontCircleAttack::actionTable[])() = {
	&FrontCircleAttack::NonInit,
	&FrontCircleAttack::SetDamageArea,
	&FrontCircleAttack::PireNail,
	&FrontCircleAttack::DestNail,
	&FrontCircleAttack::AttackEnd
};

void FrontCircleAttack::Upda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//�t�F�[�Y���Ƃ̏���
	(this->*actionTable[phase])();

	for (auto i = 0; i < nail_objses_.size(); i++) {
		//�e�N�X�`��
		nail_objses_[i].AOETexs->SetBillboard(false);
		nail_objses_[i].AOETexs->SetPosition({ nail_objses_[i].TexPos.x,nail_objses_[i].TexPos.y + 4.f,nail_objses_[i].TexPos.z });
		nail_objses_[i].AOETexs->SetRotation({ 90.f,0.f,0.f });
		nail_objses_[i].AOETexs->SetScale({ 8.f,8.f,2.f });
		nail_objses_[i].AOETexs->SetColor({ 1.f,1.f,1.f,nail_objses_[i].TexAlpha });
		nail_objses_[i].AOETexs->Update(camera);
		//�I�u�W�F
		nail_objses_[i].Obj->SetRotation({ 180.f,0.f,0.f });
		nail_objses_[i].Obj->SetPosition(nail_objses_[i].ObjPos);
		nail_objses_[i].Obj->SetScale({ 4.f,8.f,4.f });
		nail_objses_[i].Obj->Update(camera);

		//pa-texikuru
		nail_objses_[i].PireEffect->Upda_B(false);

		nail_objses_[i].TexAlpha = std::clamp(nail_objses_[i].TexAlpha, 0.f, 1.f);
	}
}
#include "imgui.h"
void FrontCircleAttack::Draw()
{
	Texture::PreDraw();
	for (auto i = 0; i < nail_objses_.size(); i++) {
		//�e�N�X�`��
		nail_objses_[i].AOETexs->Draw();
	}
	Texture::PostDraw();

	Object3d::PreDraw();
	for (auto i = 0; i < nail_objses_.size(); i++) {
		//�I�u�W�F
		nail_objses_[i].Obj->Draw();
	}
	Object3d::PostDraw();
	for (auto i = 0; i < nail_objses_.size(); i++) {
		nail_objses_[i].PireEffect->Draw();
	}
}

void FrontCircleAttack::NonInit()
{
	
}


void FrontCircleAttack::Finalize()
{
	//�J������
	for(auto i=0;i<nail_objses_.size();i++)
	{
		Destroy_unique(nail_objses_[i].PireEffect);
		Destroy_unique(nail_objses_[i].AOETexs);
		Destroy_unique(nail_objses_[i].Obj);
	}
}

void FrontCircleAttack::SetDamageArea()
{
	//�v���C���[���W
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//�{�X���W
	XMFLOAT3 bpos = { EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition() };

	//�A���t�@�����l
	constexpr float AlphaVal = 0.02f;

	//�ŏ���[0]�v�f�Q��
	float prevAlpha = nail_objses_[0].TexAlpha;

	if (nail_objses_[0].TexAlpha == 0.f) {
		//�e�N�X�`���A�I�u�W�F���W�Z�b�g
		nail_objses_[0].TexPos = { ppos.x,bpos.y,ppos.z };
		nail_objses_[0].ObjPos = { nail_objses_[0].TexPos.x,NailMinPosY,nail_objses_[0].TexPos.z };
	}
	
		for (auto i = 1; i < nail_objses_.size(); i++) {
		//�e�N�X�`�������_��
		if (nail_objses_[i].TexAlpha> 0.f)continue;
		nail_objses_[i].TexPos = { float(rand() % 200 - 100),bpos.y, float(rand() % 200 - 100) };
		nail_objses_[i].ObjPos = { nail_objses_[i].TexPos.x,-20.f,nail_objses_[i].TexPos.z};

	}

	//[0]�A���t�@�l���Z
	nail_objses_[0].TexAlpha+= AlphaVal;

	//��O�̃e�N�X�`���A���t�@�l��0.6f�������玟�̃e�N�X�`���̃A���t�@�l�グ��
	for (auto i = 1; i < nail_objses_.size(); i++)
	{
		if (prevAlpha >= 0.6f)
		{
			nail_objses_[i].TexAlpha += AlphaVal;
			//������Ă�l�u������
			prevAlpha = nail_objses_[i].TexAlpha;
		}
	}

	//���̃t�F�[�Y��
	if(nail_objses_[nail_objses_.size()-1].TexAlpha>=1.f)
	{
		for (auto i = 0; i < nail_objses_.size(); i++)
		{
			nail_objses_[i].PireEffect->SetParF(1);
			nail_objses_[i].PireEffect->CreateParticle(true, { nail_objses_[i].ObjPos.x, 10.f,nail_objses_[i].ObjPos.z });
		}
		phase = Phase::PHASETWO;
	}

	
}
#include"mHelper.h"
void FrontCircleAttack::PireNail()
{
	//�A���t�@�l�̑���
	constexpr float l_TexAlphaFeedVal = 0.01f;

	for(auto i = 0; i < nail_objses_.size(); i++)
	{
		//�C�[�W���O�i�߂�
		nail_objses_[i].PosYMovingEaseT += l_TexAlphaFeedVal;
		//�B�˂��o��
		if(nail_objses_[i].PosYMovingEaseT<=1.f)
		nail_objses_[i].ObjPos.y = Easing::EaseOut(nail_objses_[i].PosYMovingEaseT, NailMinPosY, NailMaxPosY);
	}
	
	//���̃t�F�[�Y��
	if (nail_objses_[0].PosYMovingEaseT >= 2.f) {
		phase = PHASETHREE;
	}

}

void FrontCircleAttack::DestNail()
{
	//�B�ʒu������X�s�[�h
	constexpr float l_PosYMovingVal = 0.05f;
	//�e�N�X�`���t�F�[�h�l
	constexpr float l_TexAlphaFeedVal = 0.02f;

	for (auto i = 0; i < nail_objses_.size(); i++)
	{
		nail_objses_[i].PosYMovingEaseT -= l_PosYMovingVal;

		//�B������
		if (nail_objses_[i].PosYMovingEaseT >=0.f)
			nail_objses_[i].ObjPos.y = Easing::EaseOut(nail_objses_[i].PosYMovingEaseT, NailMinPosY, NailMaxPosY);

		//�e�N�X�`������
		nail_objses_[i].TexAlpha = Easing::EaseOut(nail_objses_[i].TexAlpha, 0.f, 1.f);
	}
	//���̃t�F�[�Y��
	if (nail_objses_[0].PosYMovingEaseT <= 0.f)
		phase = PHASEFOUR;

}

void FrontCircleAttack::AttackEnd()
{
	
}




