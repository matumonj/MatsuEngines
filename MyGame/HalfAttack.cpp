#include "HalfAttack.h"
#include"EnemyControl.h"
#include"PlayerControl.h"
#include"Collision.h"
#include"CameraControl.h"
#include"Nail.h"
#include"BossSpell.h"

HalfAttack::~HalfAttack()
{
	//delete HalfAreaTex;
}

HalfAttack* HalfAttack::GetInstance()
{
	static HalfAttack instance;
	return &instance;
}

void HalfAttack::Initialize()
{
	fase = FASENON;
	Texture::LoadTexture(21, L"Resources/2d/BossAttackEffect/DamageArea.png");

	Texture* l_Tex[EnemySize];
	for (int i = 0; i < EnemySize; i++) {
	l_Tex[i]= Texture::Create(21, { 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	MagicTex[i].reset(l_Tex[i]);
	MagicTex[i]->CreateTexture();
	MagicTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
}

bool HalfAttack::SummonEnemy()
{
	if (fase==FASETWO) {
		return true;
	}
	return false;
}
void HalfAttack::ActionJudg()
{
	Enemy* Boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	//fase1　カウントダウンと中央に戻る処理
	PlayerPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	BossEnemyPos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	CenterPos = {0.0f, -18.0f, 0.0f};
	if (fase == FASEONE)
	{
		TexAlpha = 1.0f;
		BossSpell::GetInstance()->SetEndSpell(BossSpell::HALF_RIGHT, false);
		BossSpell::GetInstance()->SetEndSpell(BossSpell::HALF_LEFT, false);

		for (int i = 0; i < EnemySize; i++) {
			TexScl[i].x += 0.1f;
			TexScl[i].y += 0.1f;
		}
		RotY += 2.0f;
		
		if (TexScl[0].x > 5.0f) {
			fase = FASETWO;
		}
	}

	if (fase == FASETWO)
	{
		TexAlpha -= 0.01f;
	}

	if (TexAlpha <= 0.0f) {
		fase = FASETHREE;
	}

	if (fase == FASETHREE)
	{
		fase = FASEFOUR;
	}
	if (fase == FASEFOUR)
	{
		//TexAlpha = 0.5f;
	}

	MagicTex[0]->SetPosition({ Boss->GetPosition().x+20,Boss->GetPosition().y + 3,Boss->GetPosition().z });
	MagicTex[1]->SetPosition({ Boss->GetPosition().x-20,Boss->GetPosition().y + 3,Boss->GetPosition().z });

	for (int i = 0; i < EnemySize; i++) {
		MagicTex[i]->SetBillboard(FALSE);
		MagicTex[i]->SetScale(TexScl[i]);
		MagicTex[i]->SetRotation({ 90,0,RotY });
		MagicTex[i]->SetColor({ 1.0f,1.0f,1.0f,TexAlpha});
		MagicTex[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
	
	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void HalfAttack::Draw()
{

	Texture::PreDraw();
	for (int i = 0; i < EnemySize; i++) {
		MagicTex[i]->Draw();
	}
	Texture::PostDraw();
	if (fase == FASETHREE)
	{
		//Nail::GetInstance()->Draw();
	}
}


void HalfAttack::TurnCenter()
{
	float angleX, angleZ, dis;
	//追跡スピード
	float centerSpeed = 0.2f;

	angleX = (CenterPos.x - BossEnemyPos.x);
	angleZ = (CenterPos.z - BossEnemyPos.z);

	//敵とプレイヤーの距離求め
	dis = sqrtf((BossEnemyPos.x - CenterPos.x) * (BossEnemyPos.x - CenterPos.x)
		+ (BossEnemyPos.z - CenterPos.z) * (BossEnemyPos.z - CenterPos.z));

	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {CenterPos.x, CenterPos.y, CenterPos.z};
	XMVECTOR positionB = {BossEnemyPos.x, BossEnemyPos.y, BossEnemyPos.z};
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetRotation({
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetRotation().x,
		RotY * 60.0f + 110.0f,
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetRotation().z
	});
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

	XMMATRIX matRot = XMMatrixRotationY(
		XMConvertToRadians(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetRotation().y + 67.0f));

	move = XMVector3TransformNormal(move, matRot);

	EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetPosition({
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().x + move.m128_f32[0],
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().y,
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().z + move.m128_f32[2]
		}
	);
}

void HalfAttack::DamageJudg_Left()
{
	float Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().x;
	float Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition().x;
	if (Ppos >= Bpos)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
}

void HalfAttack::DamageJudg_Right()
{
	float Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().x;
	float Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition().x;
	if (Ppos <= Bpos)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
}
