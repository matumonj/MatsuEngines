#include "CircleAttack.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"Nail.h"
#include"BossSpell.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"imgui.h"
CircleAttack::~CircleAttack()
{
	//delete ImpactAreaTex;
}
CircleAttack*CircleAttack::GetInstance()
{
	static CircleAttack instance;
	return &instance;
}
void CircleAttack::Initialize()
{
	Texture::LoadTexture(23, L"Resources/AOE.png");

	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i] = Texture::Create(23, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
		ImpactAreaTex[i]->CreateTexture();
		ImpactAreaTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	
	Direction[NORTH] = { 0.0f ,0.0f ,30.0f };
	Direction[SOUTH] = { 0.0f ,0.0f ,-30.0f };
	Direction[EAST] = { 30.0f ,0.0f ,0.0f };
	Direction[WEST] = { -30.0f ,0.0f ,0.0f };
}
void CircleAttack::ActionJudg()
{
	switch (fase)
	{
	case FASENON:
		break;
	case FASEONE:
		//�B�h����܂�
		PierceNail();
		break;
	case FASETWO:
		//�_���[�W�G���A�̉~�L����܂�
		DamageAreaTexSet();
		break;
	case FASETHREE:
		//�n�ʂ���B�o�Ă��܂�
		ProtrudeNail();
		break;
	case FASEFOUR:
		//�U���I���ł�
		EndAttackAction();
		break;
	default:
		break;
	}
	rotY += 1.0f;
	if (rotY > 360.0f) {
		rotY = 0.0f;
	}
	//�_���[�W�G���A�e�N�X�`���̊e��p�����[�^
	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		ImpactAreaTex[i]->SetScale({ CircleSize.x,CircleSize.y,3.0f });
		ImpactAreaTex[i]->SetRotation({ 90.0f,0.0f,rotY });
		ImpactAreaTex[i]->SetColor({ 1.0f,1.0f,1.0f,TexAlpha});
	}
	ImpactAreaTex[0]->SetPosition({ Direction[Area1].x ,-18,Direction[Area1].z });
	ImpactAreaTex[1]->SetPosition({ Direction[Area2].x ,-18,Direction[Area2].z });

	//�B�I�u�W�F�̍X�V
	for (int i = 0; i < NailObj.size(); i++) {
		NailObj[i]->SetScale({ 3.0f,3.0f,3.0f });
		NailObj[i]->Update({ 1.0f,1.0f,1.0f,1.0f }, CameraControl::GetInstance()->GetCamera());
	}
	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void CircleAttack::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < 2; i++) {
		//ImpactAreaTex[i]->Draw();
	}
	Texture::PostDraw();
	for (int i = 0; i < NailObj.size(); i++) {
		Object3d::PreDraw();
		NailObj[i]->Draw();
		Object3d::PostDraw();
	}
	if (fase == FASETHREE) {
		Nail::GetInstance()->Draw();
	}
}

void CircleAttack::CollisonNailPlayer()
{
	const int Damage = 20;

	if(Collision::GetLength(NailObj[0]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
	}
	if (Collision::GetLength(NailObj[1]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
	}
}
void CircleAttack::PierceNail()
{
	TexAlpha = 1.0f;
	BossSpell::GetInstance()->SetStartSpell(BossSpell::CIRCLE,true);
	if (BossSpell::GetInstance()->GetEndSpell(BossSpell::CIRCLE)) {
		fase = FASETWO;
	}
	//�B����
	NailObj.resize(2);
	for (int i = 0; i < NailObj.size(); i++) {
		NailObj[i] = std::make_unique<Object3d>();
		//�t�B�[���h�Ƀ��f�����蓖��
		NailObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		NailObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
	}
	//�r���{�[�h�؂�(�W����true�Ȃ̂ŁA�A)
	ImpactAreaTex[0]->SetBillboard(false);
	ImpactAreaTex[1]->SetBillboard(false);

	//Direction.y->�B��Y���W�Ɠ���
	if (Direction[Area1].y > -17.0f) {
		Direction[Area1].y--;
		Direction[Area2].y--;
	}
	//���W���킹��
	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);
}

void CircleAttack::DamageAreaTexSet()
{
	const float EaseC = 0.01f;
	const XMFLOAT2 DamageAreaTex_Max = { 8.0f,8.0f };
	if (Nail::GetInstance()->GetEndAction_Circle()) {
		Nail::GetInstance()->SetEndAction_Circle(false);
	}
	//�_���[�W�G���A�̉~�Ђ낪��
	CircleAreaTime += EaseC;
	CircleSize.x = Easing::EaseOut(CircleAreaTime, 0.0f, DamageAreaTex_Max.x);
	CircleSize.y = Easing::EaseOut(CircleAreaTime, 0.0f, DamageAreaTex_Max.y);
	AttackCount = 0;

	if (CircleAreaTime >= 1.0f) {
		fase = FASETHREE;//�~�̑傫�����ő咴�����玟��
	}
}

void CircleAttack::ProtrudeNail()
{
	Nail::GetInstance()->CircleAttack(Area1, Area2);
	
	CollisonNailPlayer();

	CircleAreaTime = 0;
	Direction[Area1].y++;
	Direction[Area2].y++;

	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);
	TexAlpha -= 0.01f;
	if (TexAlpha <= 0.0f) {
		fase = FASEFOUR;
	}
}

void CircleAttack::EndAttackAction()
{
	TexAlpha = 0.5f;
	BossSpell::GetInstance()->SetEndSpell(BossSpell::CIRCLE,false);

	Direction[NORTH] = { 0.0f ,0.0f ,30.0f };
	Direction[SOUTH] = { 0.0f ,0.0f ,-30.0f };
	Direction[EAST] = { 30.0f ,0.0f ,0.0f };
	Direction[WEST] = { -30.0f ,0.0f ,0.0f };

	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);

	CircleSize = { 0.0f,0.0f };
	NailObj.clear();
}