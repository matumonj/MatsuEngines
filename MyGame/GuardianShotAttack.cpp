#include "GuardianShotAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include "PlayerControl.h"
#include"Field.h"
#define PI 3.14f

GuardianShotAttack* GuardianShotAttack::GetIns()
{
	static GuardianShotAttack ins;
	return &ins;
}

void GuardianShotAttack::Finalize()
{
	DamageTex.reset(nullptr);
	for (int i = 0; i < ArmObjNum; i++)
	{
		ArmObj[i].reset(nullptr);
		ArmEffect[i].reset(nullptr);
	}
	PlayerDamageEffect.reset(nullptr);
}


void GuardianShotAttack::TexSet()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	Texture::LoadTexture(70, L"Resources/2d/icon/enemyicon.png");

	//テクスチャセット
	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({0.5f, 0.5f});

	//初期化
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};

	for (int i = 0; i < ArmObjNum; i++)
	{
		ArmObj[i] = std::make_unique<Object3d>();
		ArmObj[i]->Initialize(camera);
		ArmObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::MISSILE));
		ArmAlpha[i] = 1.f;
		ArmShotF[i] = false;

		ArmEffect[i] = std::make_unique<Particle>();
		ArmEffect[i]->Init(Particle::BOM);
		ArmEffect[i]->SetParScl({1.f, 1.f});
		ArmEffect[i]->SetParColor({1.f, 0.2f, 0.2f, 1.f});
	}

	//ミサイル爆発時のエフェクト

	PlayerDamageEffect = std::make_unique<Particle>();
	PlayerDamageEffect->Init(Particle::TOURCHFIRE);
	PlayerDamageEffect->SetParScl({2.f, 2.f});
	PlayerDamageEffect->SetParColor({0.8f, 0.2f, 0.2f, 1.f});

	DestroyEffectF = false;
	isEndAttack = false;
	phase = NON;
}

void GuardianShotAttack::Upda()
{
	XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	switch (phase)
	{
	case NON:
		TexScl = {0.f, 0.f};
		TexAlpha = 0.0f;
		ColorT = 0.0f;
		break;

	case AREASET:
		Phase_AreaSet();
		break;

	case BOM:
		Phase_Bom();
		break;

	case END:
		Phase_End();
		break;
	}

	//各パラメータのセット
	if (phase == BOM)
	{
		for (int i = 0; i < ArmObjNum; i++)
		{
			if (ArmObj[i] == nullptr)
			{
				continue;
			}
			if (ArmEffect[i] == nullptr)
			{
				continue;
			}
			ArmObj[i]->SetPosition(ArmPos[i]);
			ArmObj[i]->SetScale({2.f, 2.f, 1.f});
			ArmObj[i]->SetRotation(ArmRot[i]);
			ArmObj[i]->SetColor({1.f, 1.f, 1.f, ArmAlpha[i]});
			ArmObj[i]->Update(camera);

			ArmEffect[i]->CreateParticle(true, {ArmPos[i]});
			ArmEffect[i]->Upda();
		}

		DestroyEffect();
	}
	TexAlpha = max(TexAlpha, 0.f);
}


void GuardianShotAttack::ArmShot()
{
	//ガーディアンインスタンス
	Enemy* Guardian = EnemyControl::GetIns()->GetGuardianEnemy();
	//プレイヤー座標
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		PlayerControl::GetIns()->GetPlayer()->GetPosition().x,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().y,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().z
	};
	//
	XMVECTOR positionB[ArmObjNum];
	XMVECTOR SubVector[ArmObjNum];
	float RotY[ArmObjNum];
	XMVECTOR move[ArmObjNum];
	XMMATRIX matRot[ArmObjNum];

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmObj[i] == nullptr)
		{
			continue;
		}
		positionB[i] = {ArmPos[i].x, ArmPos[i].y, ArmPos[i].z};
		//プレイヤーと敵のベクトルの長さ(差)を求める
		SubVector[i] = XMVectorSubtract(positionB[i], positionA); // positionA - positionB;

		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		RotY[i] = atan2f(SubVector[i].m128_f32[0], SubVector[i].m128_f32[2]);
		//移動ベクトルをy軸周りの角度で回転
		move[i] = {0.f, 0.f, 0.1f, 0};

		matRot[i] = XMMatrixRotationY(XMConvertToRadians(ArmRot[i].y));

		move[i] = XMVector3TransformNormal(move[i], matRot[i]);

		if (ArmShotF[i] == false)
		{
			ArmPos[i] = {
				EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition().x,
				-40.f,
				EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition().z
			};

			ArmRot[i] = {
				0,
				RotY[i] * 60.f + 180.f,
				0
			};

			if (ShotCount % 120 == 0)
			{
				ArmShotF[i] = true;
				break;
			}
		}
	}

	//打つ
	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmObj[i] == nullptr)
		{
			continue;
		}

		if (ArmShotF[i] == false)
		{
			continue;
		}
		ArmPos[i] = {
			ArmPos[i].x + move[i].m128_f32[0] * 2.f,
			-40.f,
			ArmPos[i].z + move[i].m128_f32[2] * 2.f
		};
	}
	ColPlayer();
}

void GuardianShotAttack::Phase_AreaSet()
{
	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 8.f;


	phase = BOM;
}

void GuardianShotAttack::Phase_Bom()
{
	XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
	ShotCount++;
	ArmShot();
	if (ArmObj[ArmObjNum - 1] == nullptr)
	{
		phase = END;
	}
}

void GuardianShotAttack::Phase_End()
{
	EnemyControl::GetIns()->GetGuardianEnemy()->SetColors({1.0f, 1.0f, 1.0f, 1.0f});
}

void GuardianShotAttack::Draw()
{
	if (phase != BOM)
	{
		return;
	}
	Object3d::PreDraw();
	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}
		if (ArmObj[i] == nullptr)
		{
			continue;
		}
		ArmObj[i]->Draw();
	}
	Object3d::PostDraw();

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}
		if (ArmEffect[i] == nullptr)
		{
			continue;
		}
		ArmEffect[i]->Draw();
	}
	PlayerDamageEffect->Draw();
}

void GuardianShotAttack::ColPlayer()
{
	//プレイヤーの座標
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}
		if (ArmObj[i] == nullptr)
		{
			continue;
		}
		if (Collision::GetLength(ArmPos[i], ppos) < 5.0f)
		{
			ColMissilePos = ArmPos[i];
			ArmAlpha[i] -= 0.05f;
			//衝突時に解放
			if (ArmAlpha[i] <= 0.0f)
			{
				ArmEffect[i].reset();
				ArmObj[i].reset();
			}
			DestroyEffectF = true;
		}
		if (Collision::GetLength(ArmPos[i], {-300.0f, -32, 270}) > 90)
		{
			ArmAlpha[i] -= 0.05f;
			//範囲外でても解放
			if (ArmAlpha[i] <= 0.0f)
			{
				ArmEffect[i].reset();
				ArmObj[i].reset();
			}
		}
	}
}

void GuardianShotAttack::DestroyEffect()
{
	//プレイヤーの座標
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//カメラ座標
	XMFLOAT3 cpos = CameraControl::GetIns()->GetCamera()->GetEye();
	//エフェクト生成座標
	XMFLOAT3 epos = {ppos.x - ((ppos.x - cpos.x) / 2.f), ColMissilePos.y + 3.f, ppos.z - ((ppos.z - cpos.z) / 2.f)};
	if (DestroyEffectF)
	{
		PlayerControl::GetIns()->GetPlayer()->RecvDamage(20);
		PlayerDamageEffect->SetParF(1);
		PlayerDamageEffect->CreateParticle(DestroyEffectF, {epos});
		DestroyEffectF = false;
	}
	PlayerDamageEffect->Upda_B();
}
