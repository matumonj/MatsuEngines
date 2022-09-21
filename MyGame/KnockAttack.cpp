#include "KnockAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"BossSpell.h"
KnockAttack::~KnockAttack()
{
	//delete KnockTex;
}
KnockAttack* KnockAttack::GetInstance()
{
	static KnockAttack instance;
	return &instance;
}
void KnockAttack::Initialize()
{
	Texture::LoadTexture(63, L"Resources/Knock.png");

	KnockTex = Texture::Create(63, {0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	KnockTex->CreateTexture();
	KnockTex->SetAnchorPoint({0.5f,0.5f });

}

void KnockAttack::ActionJudg()
{
	if (fase==FASEONE) {
		BossSpell::GetInstance()->SetStartSpell_KA(true);
		if (BossSpell::GetInstance()->GetEndSpell_KA()) {//2�b��������
			fase =FASETWO;//�U���t�F�[�Ysrart
		}
		//������΂����O�ɐ�����΂���̃v���C���[z���W��ݒ�
		AfterPositionZ = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 50.0f;
		BeforePositionZ = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z;

		KnockTime = 0.0f;//�C�[�W���O�p�J�E���^���Z�b�g
		TexAlpha = 0.6f;
	}
	
	if (fase==FASETWO) {
		TexAlpha -= 0.1f;
		if (KnockTime >= 1.0f) {
			fase = FASETHREE;
		}
		KnockTime += 0.04f;//�C�[�W���O�p�J�E���^
		AttackCount = 0;//�U���ҋ@���ԃ��Z�b�g
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({
				PlayerControl::GetInstance()->GetPlayer()->GetPosition().x ,
				PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
					Easing::EaseOut(KnockTime, BeforePositionZ, AfterPositionZ)
			});
	}
	if (CameraControl::GetInstance()->GetCamera() != nullptr) {
		KnockTex->SetUVMove(true);
		KnockTex->SetBillboard(false);

		KnockTex->Update(CameraControl::GetInstance()->GetCamera());
	}
		KnockTex->SetPosition({ 0.0f ,-18.0f ,0.0f });
		KnockTex->SetColor({ 1.0f,1.0f,1.0f,TexAlpha });
		KnockTex->SetRotation({ 90.0f , 0.0f ,0.0f });
		KnockTex->SetScale({ 11.0f ,11.0f ,3.0f });
	AttackCount = min(AttackCount, 180);
	AttackCount = max(AttackCount, 0);

	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}
void KnockAttack::Draw()
{
	Texture::PreDraw();
	if (fase==FASEONE) {
		KnockTex->Draw();
	}
	Texture::PostDraw();
}