#include "GuardianBomAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include "Field.h"
#include "PlayerControl.h"
#define PI 3.14f

GuardianBomAttack* GuardianBomAttack::GetIns()
{
	static GuardianBomAttack ins;
	return &ins;
}

void GuardianBomAttack::Finalize()
{
	for (int i = 0; i < ArmObjNum; i++)
	{
		DamageTex[i].reset(nullptr);
	}
	for (int i = 0; i < ArmObjNum; i++)
	{
		ArmObj[i].reset(nullptr);
		
		ArmEffect[i].reset(nullptr);
	
		//ミサイル爆発時のエフェクト
		BomEffect[i].reset(nullptr);
	}
}

void GuardianBomAttack::TexSet()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	Texture::LoadTexture(71, L"Resources/2d/icon/missileicon.png");

	//テクスチャセット
	Texture* l_tex[ArmObjNum];
	for (int i = 0; i < ArmObjNum; i++)
	{
		l_tex[i] = Texture::Create(71);
		DamageTex[i].reset(l_tex[i]);
		DamageTex[i]->CreateTexture();
		DamageTex[i]->SetAnchorPoint({0.5f, 0.5f});
	}
	//初期化
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};

	Texture::LoadTexture(66, L"Resources/2d/BossAttackEffect/missileeffect.png");

	XMFLOAT3 Gpos = {-300.0f, -32, 270};
	for (int i = 0; i < ArmObjNum; i++)
	{
		ArmObj[i] = std::make_unique<Object3d>();
		ArmObj[i]->Initialize(camera);
		ArmObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::MISSILE));
		ArmAlpha[i] = 1.f;

		ArmEffect[i] = std::make_unique<Particle>();
		ArmEffect[i]->Init(64);
		ArmEffect[i]->SetParScl({1.f, 1.f});
		ArmEffect[i]->SetParColor({1.f, 1.f, 1.f, 1.f});

		DTexAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(ArmObjNum));
		//位置の初期化
		DtexPos[i].x = Gpos.x + sinf(DTexAngle[i] * (PI / 180.0f)) * 20.0f;
		DtexPos[i].z = Gpos.z + cosf(DTexAngle[i] * (PI / 180.0f)) * 20.0f;
		DtexPos[i].y = -31.f;

		DtexAlpha[i] = 0.0f;

		ArmPos[i] = {DtexPos[i].x, 0.f, DtexPos[i].z};


		//ミサイル爆発時のエフェクト
		BomEffect[i] = std::make_unique<Particle>();
		BomEffect[i]->Init(66);
		BomEffect[i]->SetParScl({2.f, 2.f});
		BomEffect[i]->SetParColor({1.f, 1.f, 1.f, 1.f});

		ArmShotF[i] = false;
		MissileDestFlag[i] = false;

		isEndAttack = false;
		DestroyEffectF = false;
	}

	phase = NON;
}

void GuardianBomAttack::Upda()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

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
	TexRotZ++;
	for (int i = 0; i < ArmObjNum; i++)
	{
		if (BomEffect[i] == nullptr)continue;
		BomEffect[i]->Upda_B();
		DamageTex[i]->SetScale({5.f, 5.f, 5.f});
		DamageTex[i]->SetPosition(DtexPos[i]);
		DamageTex[i]->SetRotation({90.f, 0.f, TexRotZ});
		DamageTex[i]->SetBillboard(FALSE);
		DamageTex[i]->SetColor({1.f, 0.5f, 0.5f, DtexAlpha[i]});
		DamageTex[i]->Update(camera);
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
			ArmObj[i]->SetPosition(ArmPos[i]);
			ArmObj[i]->SetScale({2.f, 2.f, 1.f});
			ArmObj[i]->SetRotation(ArmRot[i]);
			ArmObj[i]->SetColor({1.f, 1.f, 1.f, ArmAlpha[i]});
			ArmObj[i]->Update({1.f, 1.f, 1.f, ArmAlpha[i]}, camera);
		}
		for (int i = 0; i < ArmObjNum; i++)
		{
			if (ArmEffect[i] == nullptr)
			{
				continue;
			}

			if (ArmObj[i] == nullptr)
			{
				ArmEffect[i]->EndUpda(true);
			}
			ArmEffect[i]->CreateParticle(true, {ArmPos[i]});
			ArmEffect[i]->Upda();
		}

		DestroyEffect();
	}
	TexAlpha = max(TexAlpha, 0.f);
}


void GuardianBomAttack::ArmShot()
{
	//ガーディアンインスタンス
	Enemy* Guardian = EnemyControl::GetInstance()->GetGuardianEnemy();
	//プレイヤー座標
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	//敵がプエレイヤーの方向く処理

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmObj[i] == nullptr)
		{
			continue;
		}

		if (ArmShotF[i] == false)
		{
			ArmRot[i] = {
				90.f, 90.f, 0.f
			};

			if (ShotCount % 90 == 0)
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
		DtexAlpha[i] += 0.02f;
		if (DtexAlpha[i] >= 1.f)
		{
			ArmPos[i].y -= 0.4f;
		}
	}
	ColPlayer();
}

void GuardianBomAttack::Phase_AreaSet()
{
	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 8.f;


	//if (nextPhase) {
	phase = BOM;
	//}
}

void GuardianBomAttack::Phase_Bom()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	ShotCount++;
	ArmShot();
	if ( DtexAlpha[ArmObjNum - 1] <= 0.0f &&ArmObj[ArmObjNum - 1] == nullptr)
	{
		phase = END;
	}
}

void GuardianBomAttack::Phase_End()
{
	for (int i = 0; i < ArmObjNum; i++)
	{
		BomEffect[i].reset();
		ArmEffect[i].reset();
	}
	EnemyControl::GetInstance()->GetGuardianEnemy()->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
}

#include "imgui.h"

void GuardianBomAttack::Draw()
{
	ImGui::Begin("armpos");
	ImGui::Text("%f", ArmPos[0].y);
	ImGui::End();
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
		BomEffect[i]->Draw();
	}
	Texture::PreDraw();
	for (int i = 0; i < ArmObjNum; i++)
	{
		DamageTex[i]->Draw();
	}
	Texture::PostDraw();
}

void GuardianBomAttack::ColPlayer()
{
	//プレイヤーの座標
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

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

		if (Collision::GetLength(ppos, ArmPos[i]) < 10.f)
		{
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
			MissileDestFlag[i] = true;
		}
		if (ArmPos[i].y < -32)
		{
			MissileDestFlag[i] = true;
		}

		if (Collision::GetLength(ArmPos[i], {-300.0f, -32, 270}) > 90)
		{
			ArmAlpha[i] -= 0.05f;
			//範囲外でても解放
			if (ArmAlpha[i] <= 0.0f)
			{
				//	ArmEffect[i].reset();
				//ArmObj[i].reset();
			}
		}
	}
}

void GuardianBomAttack::DestroyEffect()
{
	//プレイヤーの座標
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	//カメラ座標
	XMFLOAT3 cpos = CameraControl::GetInstance()->GetCamera()->GetEye();
	//エフェクト生成座標
	XMFLOAT3 epos = {ppos.x - ((ppos.x - cpos.x) / 2.f), ColMissilePos.y + 3.f, ppos.z - ((ppos.z - cpos.z) / 2.f)};
	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}

		if (MissileDestFlag[i])
		{
			BomEffect[i]->SetParF(1);
			BomEffect[i]->CreateParticle(MissileDestFlag[i], {ArmPos[i].x, -38.f, ArmPos[i].z});
			Destroy_unique(ArmObj[i]);
			MissileDestFlag[i] = false;
		}
		if (ArmObj[i] == nullptr && DtexAlpha[i] > 0.0f)
		{
			DtexAlpha[i] -= 0.05f;
		}
		BomEffect[i]->Upda_B();
	}
}
