#include "FrontCircleAttack.h"

#include <algorithm>

#include"EnemyControl.h"
#include"CameraControl.h"
#include "PlayerControl.h"

#include"mHelper.h"
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
		nail_objses_[i].PireEffect->SetParScl({ 8.f, 8.f });
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
	_phase = PHASE_NON;
}


void (FrontCircleAttack::* FrontCircleAttack::actionTable[])() = {
	&FrontCircleAttack::NonInit,//�ҋ@
	&FrontCircleAttack::SetDamageArea,//�G���A�w��
	&FrontCircleAttack::PireNail,//�B�˂��o��
	&FrontCircleAttack::DestNail,//��������
	&FrontCircleAttack::AttackEnd//�I��
};

void FrontCircleAttack::Upda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//�t�F�[�Y���Ƃ̏���
	(this->*actionTable[_phase])();

	for (auto i = 0; i < nail_objses_.size(); i++) {
		if (nail_objses_[i].AOETexs != nullptr) {
			//�e�N�X�`��
			nail_objses_[i].TexRotY++;
			nail_objses_[i].AOETexs->SetBillboard(false);
			nail_objses_[i].AOETexs->SetPosition({ nail_objses_[i].TexPos.x,nail_objses_[i].TexPos.y + 4.f,nail_objses_[i].TexPos.z });
			nail_objses_[i].AOETexs->SetRotation({ 90.f,0.f,nail_objses_[i].TexRotY});
			nail_objses_[i].AOETexs->SetScale(nail_objses_[i].TexScl);
			nail_objses_[i].AOETexs->SetColor({ 1.f,1.f,1.f,nail_objses_[i].TexAlpha });
			nail_objses_[i].AOETexs->Update(camera);
		}
		//�I�u�W�F
		if (nail_objses_[i].Obj != nullptr) {
			nail_objses_[i].Obj->SetRotation({ 180.f,0.f,0.f });
			nail_objses_[i].Obj->SetPosition(nail_objses_[i].ObjPos);
			nail_objses_[i].Obj->SetScale({ 8.f,8.f,8.f });
			nail_objses_[i].Obj->SetUVf(true);
			nail_objses_[i].Obj->Update(camera);
		}

		//pa-texikuru
		if (nail_objses_[i].PireEffect != nullptr) {
			nail_objses_[i].PireEffect->Upda_B(false);
		}

		nail_objses_[i].TexAlpha = std::clamp(nail_objses_[i].TexAlpha, 0.f, 1.f);
	}
}
#include "imgui.h"
void FrontCircleAttack::Draw()
{
	//�e�N�X�`��
	Texture::PreDraw();
	for (auto i = 0; i < nail_objses_.size(); i++) {
		if (nail_objses_[i].AOETexs == nullptr)continue;
		nail_objses_[i].AOETexs->Draw();
	}
	Texture::PostDraw();

	//�B�I�u�W�F
	Object3d::PreDraw();
	if (_phase != PHASE_FOUR && _phase != PHASE_NON) {
		for (auto i = 0; i < nail_objses_.size(); i++) {
			if (nail_objses_[i].Obj == nullptr)continue;
			nail_objses_[i].Obj->Draw();
		}
	}
	Object3d::PostDraw();

	//�p�[�e�B�N���G�t�F�N�g
	for (auto i = 0; i < nail_objses_.size(); i++) {
		if (nail_objses_[i].PireEffect == nullptr)continue;
		nail_objses_[i].PireEffect->Draw();
	}
}

void FrontCircleAttack::NonInit()
{
	for (auto i = 0; i < nail_objses_.size(); i++)
	{
		nail_objses_[i].TexAlpha = 0.f;
		nail_objses_[i].PosYMovingEaseT = 0.f;
		nail_objses_[i].TexSclingEaseT = 0.f;
	}
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
		nail_objses_[i].TexPos = { float(rand() % 100 - 50)*1.2f,bpos.y, float(rand() % 100 - 50)*1.2f };
		nail_objses_[i].ObjPos = { nail_objses_[i].TexPos.x,-20.f,nail_objses_[i].TexPos.z};

	}
		nail_objses_[0].TexScl = { 8.f,8.f,2.f };
	//[0]�A���t�@�l���Z
	nail_objses_[0].TexAlpha+= AlphaVal;

	//��O�̃e�N�X�`���A���t�@�l��0.6f�������玟�̃e�N�X�`���̃A���t�@�l�グ��
	for (auto i = 1; i < nail_objses_.size(); i++)
	{
		if (prevAlpha >= 0.6f)
		{
			//�e�N�X�`���L����
			nail_objses_[i].TexSclingEaseT += 0.02f;
			nail_objses_[i].TexScl.x = Easing::EaseOut(nail_objses_[i].TexSclingEaseT, 0.f, 8.f);
			nail_objses_[i].TexScl.y = Easing::EaseOut(nail_objses_[i].TexSclingEaseT, 0.f, 8.f);

			//�ő�l�ƍŏ��l
			nail_objses_[i].TexSclingEaseT = std::clamp(nail_objses_[i].TexSclingEaseT, 0.f, 1.f);

			//�A���t�@�l�グ��
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
		_phase = Phase::PHASE_TWO;
	}

	
}
void FrontCircleAttack::PireNail()
{
	//�A���t�@�l�̑���
	constexpr float l_TexAlphaFeedVal = 0.03f;
	
	for(auto i = 0; i < nail_objses_.size(); i++)
	{
		//�C�[�W���O�i�߂�
		nail_objses_[i].PosYMovingEaseT += l_TexAlphaFeedVal;
		//�B�˂��o��
		if(nail_objses_[i].PosYMovingEaseT<=1.f)
		nail_objses_[i].ObjPos.y = Easing::EaseOut(nail_objses_[i].PosYMovingEaseT, NailMinPosY, NailMaxPosY);
	}

	//�B�Ƃ���������_���[�W
	if(ColNail_Player()==true)
	{
		PlayerControl::GetIns()->GetPlayer()->RecvDamage(Damage);
	}

	//���̃t�F�[�Y��
	if (nail_objses_[0].PosYMovingEaseT >= 2.f) {
		_phase = PHASE_THREE;
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
		if (nail_objses_[i].PosYMovingEaseT >=0.f&& nail_objses_[i].PosYMovingEaseT<=1.f)
			nail_objses_[i].ObjPos.y = Easing::EaseOut(nail_objses_[i].PosYMovingEaseT, NailMinPosY, NailMaxPosY);

		//�e�N�X�`������
		nail_objses_[i].TexAlpha = Easing::EaseOut(nail_objses_[i].TexAlpha, 0.f, 1.f);
	}
	//���̃t�F�[�Y��
	if (nail_objses_[0].PosYMovingEaseT <= 0.f)
		_phase = PHASE_FOUR;

}

void FrontCircleAttack::AttackEnd()
{
	for (auto i = 0; i < nail_objses_.size(); i++)
	{
		nail_objses_[i].TexAlpha = 0.f;
		nail_objses_[i].PosYMovingEaseT = 0.f;
		nail_objses_[i].TexSclingEaseT = 0.f;
	}
	_phase = PHASE_NON;
}


bool FrontCircleAttack::ColNail_Player()
{
	//�v���C���[���W
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	for(auto i=0;i<nail_objses_.size();i++)
	{
		bool L_colJudg=Collision::GetLength(ppos, nail_objses_[i].ObjPos) < 20.f;

		if(L_colJudg)
		{
			return true;
		}
	}
	return false;
}


