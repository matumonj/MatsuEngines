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
	return&instance;
}
void HalfAttack::Initialize()
{
	
	Texture::LoadTexture(21, L"Resources/damage.png");

	HalfAreaTex = Texture::Create(21, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	HalfAreaTex->CreateTexture();
	HalfAreaTex->SetAnchorPoint({ 0.5f,0.5f });

}

void HalfAttack::ActionJudg()
{
	
//fase1　カウントダウンと中央に戻る処理
	PlayerPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	BossEnemyPos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition();
	CenterPos = { 0.0f ,-18.0f ,0.0f };
	if (fase==FASEONE) {
		BossSpell::GetInstance()->SetEndSpell(BossSpell::HALF_RIGHT,false);
		BossSpell::GetInstance()->SetEndSpell(BossSpell::HALF_LEFT,false);

		TexAlpha += 0.02f;
		TurnCenter();
		if (Collision::GetLength(BossEnemyPos, CenterPos) < 10.0f) {
			if (PlayerPos.x>0.0f) {
				Area = LEFT;
			} else if(PlayerPos.x <=0.0f) {
				Area = RIGHT;
			}
			fase = FASETWO;	
		}
	}

	if (fase==FASETWO) {
		if (Area == LEFT) {
			BossSpell::GetInstance()->SetStartSpell(BossSpell::HALF_RIGHT,true);
		} else if (Area == RIGHT) {
			BossSpell::GetInstance()->SetStartSpell(BossSpell::HALF_LEFT,true);
		}
		if (BossSpell::GetInstance()->GetEndSpell(BossSpell::HALF_LEFT)|| BossSpell::GetInstance()->GetEndSpell(BossSpell::HALF_RIGHT)) {
			fase = FASETHREE;
		}
	}

	if (fase==FASETHREE) {
		if (Nail::GetInstance()->GetEndAction_Half()) {
			Nail::GetInstance()->SetEndAction_Half(false);
		}
		if ( Area == LEFT) {
			DamageJudg_Left();
			Nail::GetInstance()->HalfAttack(Nail::RIGHT);
		}
		else if (Area == RIGHT) {
			DamageJudg_Right();
			Nail::GetInstance()->HalfAttack(Nail::LEFT);
		}
		TexAlpha -= 0.02f;
		AttackCount = 0;
		if (Nail::GetInstance()->GetEndAction_Half()) {
			fase = FASEFOUR;
		}
	}
	if (fase==FASEFOUR) {
		TexAlpha = 0.5f;
	}
	HalfAreaTex->SetUVMove(true);
	HalfAreaTex->SetBillboard(false);
	HalfAreaTex->SetColor({ 1.0f ,1.0f ,1.0f ,TexAlpha });

	HalfAreaTex->Update(CameraControl::GetInstance()->GetCamera());
	HalfAreaTex->SetPosition({ 0.0f ,-18.0f ,0.0f });
	HalfAreaTex->SetRotation({ 90,0,0 });
	switch (Area)
	{
	case HalfAttack::LEFT:

		HalfAreaTex->SetPosition({ 30.0f ,-18.0f ,0.0f });
		HalfAreaTex->SetScale({ 8.0f ,11.5f,3.0f  });
		break;
	case HalfAttack::RIGHT:
		HalfAreaTex->SetPosition({ -30.0f ,-18.0f ,0.0f });
		HalfAreaTex->SetScale({ 8.0f ,11.5f,3.0f });
		break;
	default:
		break;
	}
	TexAlpha = min(TexAlpha, 0.6f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void HalfAttack::Draw()
{
	if (fase == FASETWO) {
		Texture::PreDraw();
		HalfAreaTex->Draw();
		Texture::PostDraw();
	}
	 if (fase == FASETHREE) {
		Nail::GetInstance()->Draw();
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
		XMVECTOR positionA = { CenterPos.x,CenterPos.y, CenterPos.z };
		XMVECTOR positionB = { BossEnemyPos.x,BossEnemyPos.y,BossEnemyPos.z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		EnemyControl::GetInstance()->GetBossEnemyindex()[0]->SetRotation({
			EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetRotation().x,
			RotY * 60.0f + 180.0f ,
			EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetRotation().z });
		//移動ベクトルをy軸周りの角度で回転
		XMVECTOR move = { 0.0f ,0.0f ,0.1f,0.0f };

		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetRotation().y));

		move = XMVector3TransformNormal(move, matRot);

		EnemyControl::GetInstance()->GetBossEnemyindex()[0]->SetPosition({
					EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().x + move.m128_f32[0],
					EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().y,
					EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().z + move.m128_f32[2] }
		);
}

void HalfAttack::DamageJudg_Left()
{
	float Bpos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().x;
	float Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition().x;
	if (Ppos >= Bpos) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
}

void HalfAttack::DamageJudg_Right()
{
	float Bpos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().x;
	float Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition().x;
	if (Ppos <= Bpos) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
}
