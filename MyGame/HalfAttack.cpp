#include "HalfAttack.h"
#include"EnemyControl.h"
#include"PlayerControl.h"
#include"Collision.h"
#include"CameraControl.h"
#include"Nail.h"
#include"BossSpell.h"
HalfAttack::~HalfAttack()
{
	delete HalfAreaTex;
}
HalfAttack* HalfAttack::GetInstance()
{
	static HalfAttack instance;	
	return&instance;
}
void HalfAttack::Initialize()
{
	
	Texture::LoadTexture(21, L"Resources/damage.png");

	HalfAreaTex = Texture::Create(21, { 0,0,0 }, { 100,100,1 }, { 1,1,1,1 });
	HalfAreaTex->CreateTexture();
	HalfAreaTex->SetAnchorPoint({ 0.5,0.5 });

}

void HalfAttack::ActionJudg()
{
	if (Input::GetInstance()->TriggerButton(Input::GetInstance()->Button_A)) {
		fase1 = true;
	}
//fase1�@�J�E���g�_�E���ƒ����ɖ߂鏈��
	PlayerPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	BossEnemyPos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition();
	CenterPos = { 0,-18,0 };
	if (fase1) {
		TurnCenter();
		if (Collision::GetLength(BossEnemyPos, CenterPos) < 10) {
			if (PlayerPos.x>0) {
				Area = LEFT;
			} else if(PlayerPos.x <=0) {
				Area = RIGHT;
			}
			fase2 = true;
			fase1 = false;
		}
	}

	if (fase2) {
		if (Area == LEFT) {
			BossSpell::GetInstance()->SetStartSpell_HR(true);
		} else if (Area == RIGHT) {
			BossSpell::GetInstance()->SetStartSpell_HL(true);
		}
		if (BossSpell::GetInstance()->GetEndSpell_HL()|| BossSpell::GetInstance()->GetEndSpell_HR()) {
			fase3 = true;
			fase2 = false;
		}
	}

	if (fase3) {
		if ( Area == LEFT) {
			Nail::GetInstance()->HalfAttack(Nail::RIGHT);
		}
		else if (Area == RIGHT) {
			Nail::GetInstance()->HalfAttack(Nail::LEFT);
		}
		TexAlpha -= 0.02f;
		AttackCount = 0;
	}
	HalfAreaTex->SetUVMove(true);
	HalfAreaTex->SetBillboard(false);
	HalfAreaTex->SetColor({ 1,1,1,TexAlpha });

	HalfAreaTex->Update(CameraControl::GetInstance()->GetCamera());
	HalfAreaTex->SetPosition({ 0,-18,0 });
	HalfAreaTex->SetRotation({ 90,0,0 });
	switch (Area)
	{
	case HalfAttack::LEFT:

		HalfAreaTex->SetPosition({ 30,-18,0 });
		HalfAreaTex->SetScale({ 8,11.5,3 });
		break;
	case HalfAttack::RIGHT:
		HalfAreaTex->SetPosition({ -30,-18,0 });
		HalfAreaTex->SetScale({ 8,11.5,3 });
		break;
	default:
		break;
	}
	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void HalfAttack::Draw()
{
	Texture::PreDraw();
	HalfAreaTex->Draw();
	
	Texture::PostDraw();
}


void HalfAttack::TurnCenter()
{
		float angleX, angleZ, dis;
		//�ǐՃX�s�[�h
		float centerSpeed = 0.2f;

		angleX = (CenterPos.x - BossEnemyPos.x);
		angleZ = (CenterPos.z - BossEnemyPos.z);

		//�G�ƃv���C���[�̋�������
		dis = sqrtf((BossEnemyPos.x - CenterPos.x) * (BossEnemyPos.x - CenterPos.x)
			+ (BossEnemyPos.z - CenterPos.z) * (BossEnemyPos.z - CenterPos.z));

		//�G���v�G���C���[�̕���������
		XMVECTOR positionA = { CenterPos.x,CenterPos.y, CenterPos.z };
		XMVECTOR positionB = { BossEnemyPos.x,BossEnemyPos.y,BossEnemyPos.z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		EnemyControl::GetInstance()->GetBossEnemyindex()[0]->SetRotation({
			EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetRotation().x,
			RotY * 60 + 180,
			EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetRotation().z });
}