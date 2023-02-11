#include "ThrowRockAttack.h"

#include "CameraControl.h"
#include "EnemyControl.h"
#include "PlayerControl.h"

ThrowRockAttack* ThrowRockAttack::GetIns()
{
	static ThrowRockAttack ins;
	return &ins;
}

void ThrowRockAttack::Init()
{
	//カメラのインスタンス
	camera = CameraControl::GetIns()->GetCamera();
	//石オブジェの初期化
	ThrowRockObj = std::make_unique<Object3d>();
	ThrowRockObj->Initialize(camera);
	ThrowRockObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::THROWROCK));
	ThrowRockObj->SetScale({2.f, 2.f, 2.f});

	//砕ける岩エフェクト
	for (int i = 0; i < DestRock.size(); i++)
	{
		DestRock[i] = std::make_unique<Object3d>();
		DestRock[i]->Initialize(camera);
		DestRock[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::THROWROCK));
		DestRock[i]->SetScale({1.2f, 1.2f, 1.2f});
	}
	//土煙パーティクルの初期化
	DestPar = std::make_unique<Particle>();
	DestPar->Init(64);
	DestPar->SetParScl({1.3f, 1.3f});
	DestPar->SetParColor({1.f, 1.f, 1.f, 1.f});
	//アクションフェーズの初期化
	tmotion = END;
}

void ThrowRockAttack::Upda()
{
	//石オブジェの更新
	ThrowRockObj->SetPosition(RockPos);
	ThrowRockObj->Update(camera);

	DestEffect();

	//砕ける岩エフェクトの更新
	for (int i = 0; i < DestRock.size(); i++)
	{
		DestRock[i]->SetColor({1.f, 1.f, 1.f, DestRockAlpha[i]});
		DestRock[i]->SetPosition(DestRockPos[i]);
		DestRock[i]->SetRotation(
			{0.f, 0.f, static_cast<float>(i) * 75.f});
		DestRock[i]->Update(camera);
	}
	//関数テーブルの更新
	ActionUpda();
}

void ThrowRockAttack::Draw()
{
	Object3d::PreDraw();
	//あたった瞬間描画切る
	if (!destF)
	{
		ThrowRockObj->Draw();
	}
	//アルファ値０以下なったら描画切る
	for (int i = 0; i < DestRock.size(); i++)
	{
		if (DestRockAlpha[i] < 0.f)
		{
			continue;
		}
		DestRock[i]->Draw();
	}
	Object3d::PostDraw();

	//パーティクル描画
	DestPar->Draw();
}

void ThrowRockAttack::DestEffect()
{
	for (int i = 0; i < DestRock.size(); i++)
	{
		if (destF)
		{
			DestRockAcc[i] += 0.002f;
			DestRockPos[i].x += 0.1f * cosf(static_cast<float>(i) * 360.f); //360度に広がるようにする
			DestRockPos[i].y += 0.1f * sinf(static_cast<float>(i) * 360.f) - DestRockAcc[i];

			DestRockAlpha[i] -= 0.01f;

			RockPos = {0, -90, 0};
		}
		else
		{
			DestRockAlpha[i] = 1.f;
			DestRockAcc[i] = 0.f;
			DestRockPos[i] = ThrowRockObj->GetPosition();
		}
	}
	if (EffectCreateF)
	{
		DestPar->SetParF(0);
		DestPar->CreateParticle(EffectCreateF, {
			                        ThrowRockObj->GetPosition().x,
			                        ThrowRockObj->GetPosition().y - 8.f,
			                        ThrowRockObj->GetPosition().z
		                        });
		EffectCreateF = false;
	}
	DestPar->Upda_B();
}


void ThrowRockAttack::ThrowPhase_Set()
{
	//ボスのインスタンス持ってくる
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();

	const double PickTime = 0.8;
	//投げモーション時　fbxタイムが一定超えたら披露モーション
	if (boss->GetNowMotion() != boss->THROW)
	{
		return;
	}

	if (boss->GetAnimationTime() > PickTime)
	{
		tmotion = PICK;
	}
}

void ThrowRockAttack::ThrowPhase_Pick()
{
	//ボスのインスタンス持ってくる
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();

	const double ThrowTime = 2.5;

	if (boss->GetNowMotion() != boss->THROW)
	{
		return;
	}

	if (boss->GetAnimationTime() > ThrowTime)
	{
		tmotion = THROW;
	}
	//岩の座標を手のボーン座標に
	RockPos = boss->HandLeftPos();

	destF = false;
}


void ThrowRockAttack::ThrowPhase_Throw()
{
	//ボスのインスタンス持ってくる
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
	//右手とプレイヤーのあたりはんてい
	XMFLOAT3 l_playerpos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//衝突判定
	bool ColJudg = Collision::GetLength(l_playerpos, RockPos) < 10.f;
	//ダメージ値
	const int Damage = 10;
	//飛ぶスピード
	const float RockSpeed = 19.f;


	if (ColJudg)
	{
		//土煙発生
		EffectCreateF = true;
		//投石の描画切る
		destF = true;
		//プレイヤーへのダメージ
		if (!TurnoffDrawF)
		{
			PlayerControl::GetIns()->GetPlayer()->RecvDamage(Damage);
		}
	}
	//岩が飛んでく挙動
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(boss->GetRotation().y + 180.f));
	move = XMVector3TransformNormal(move, matRot);
	//ボスの向いてる方へ飛ぶ
	if (!destF)
	{
		//向いた方向に進む
		RockPos.y -= 0.1f;
		RockPos.x += move.m128_f32[0] * RockSpeed;
		RockPos.z += move.m128_f32[2] * RockSpeed;
	}

	if (boss->GetAnimationTime() >= boss->GetFbxTimeEnd() - 0.3f)
	{
		tmotion = END;
	}
}

//アクションフェーズの関数テーブル
void (ThrowRockAttack::* ThrowRockAttack::actionTable[])() = {
	nullptr,
	&ThrowRockAttack::ThrowPhase_Set,
	&ThrowRockAttack::ThrowPhase_Pick,
	&ThrowRockAttack::ThrowPhase_Throw,
	&ThrowRockAttack::ThrowPhase_End,
};

void ThrowRockAttack::ActionUpda()
{
	//シーンに応じた更新処理
	(this->*actionTable[tmotion])();
}

void ThrowRockAttack::ThrowPhase_End()
{
	//destF = false;
}
