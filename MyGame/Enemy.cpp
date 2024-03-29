#include "Enemy.h"

#include <algorithm>

#include"mHelper.h"
#include"EnemyWalkState.h"
#include"BossEnemyStay.h"
#include"PlayerControl.h"
#include"GuardianAppearState.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include"CameraControl.h"
#include "SelectSword.h"
using namespace DirectX;

Enemy::Enemy()
{
	//状態の初期化
	state_boss = new BossEnemyStay();
	state_mob = new EnemyWalkState();
	state_guardian = new GuardianAppearState();
}

Enemy::~Enemy()
{
	//状態の破棄
	Destroy(state_mob);
	Destroy(state_boss);
	Destroy(state_guardian);
}


void Enemy::Action()
{
	AttackCoolTime();
}

void Enemy::HelpIconInit()
{
	Sprite* l_frame = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::HELPICON), {0, 0});

	HelpIcon.reset(l_frame);
	HelpIcon->SetAnchorPoint({0.5f, 0.5f});
	HelpIconAlpha = 0.f;
}

void Enemy::HelpIconShow()
{
	if (HelpJudg)
	{
		HelpIconAlpha += 0.1f;
		if (HelpIconAlpha >= 2.f)
		{
			HelpJudg = false;
		}
	}
	else
	{
		HelpIconAlpha -= 0.02f;
	}


	XMVECTOR tex2DPos = {Position.x, Position.y + 5.f, Position.z};
	tex2DPos = MatCal::PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetViewMatrix(), false);
	tex2DPos = MatCal::PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetProjectionMatrix(), true);
	tex2DPos = MatCal::WDivi(tex2DPos, false);
	tex2DPos = MatCal::PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetViewPort(), false);

	HelpIconAlpha = std::clamp(HelpIconAlpha, 0.0f, 2.f);
	HelpIcon->SetPosition({tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]});
	HelpIcon->setcolor({1.f, 1.f, 1.f, HelpIconAlpha});
	HelpIcon->SetSize({200.f, 200.f});
}

void Enemy::HelpIconDraw()
{
	if (HelpIconAlpha <= 0.f)
	{
		return;
	}
	Sprite::PreDraw();
	HelpIcon->Draw();
	Sprite::PostDraw();
}


#include"PlayerAttackState.h"

void Enemy::HelpAction()
{
	//3D->2D変換 3引数消す
}

void Enemy::RecvDamage(int Damage)
{
	//HelpJudg = true;

	//被ダメージ時の行動
	//例外処理
	if (this == nullptr || EnemyHP < 0)
	{
		return;
	}
	if (ENumber == MINIGOLEM)
	{
		if (animeState == IDLE)
		{
			RecvDamagef2 = true;
		}
	}
	else
	{
		RecvDamagef2 = true;
	}

	RecvDamagef = true;

	/*ダメージテクスチャの表示*/
	{
		DamageSize = Damage;
		DamageTexPos = Position;
		std::unique_ptr<DamageManager> newdTex;
		//ガーディアンのみ座標高いのでずらす
		if (ENumber == GUARDIAN)
		{
			//インスタンス生成
			newdTex = std::make_unique<DamageManager>(
				XMFLOAT3(Position.x + rand() % 10 - 5, -10 + Position.y + rand() % 10 - 5, Position.z), Damage);
		}
		else
		{
			//インスタンス生成
			newdTex = std::make_unique<DamageManager>(
				XMFLOAT3(Position.x + rand() % 10 - 5, Position.y + rand() % 10 - 5, Position.z), Damage);
		}
		//リストに追加
		dMans_.push_back(std::move(newdTex));
	}
	/*----------------------*/

	//体力減らす
	EnemyHP = EnemyHP - Damage;
	//ダメージパーティクル表示
	DamageParticleCreateF = true;
}

void Enemy::HPFrameUpda()
{
	if (SceneManager::GetIns()->GetScene() != SceneManager::TUTORIAL)
	{
		Position.x = std::clamp(Position.x, -300.f, 300.f);
		Position.z = std::clamp(Position.z, -200.f, 400.f);
	}
	//カメラのインスタンス取得
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	//2D->3D変換用
	XMVECTOR tex2DPos[4];

	for (int i = 0; i < 4; i++)
	{
		//w除算で座標を２D変換
		tex2DPos[i] = {Position.x, Position.y + 13.0f, Position.z};
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetProjectionMatrix(), true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewPort(), false);

		//座標セット
		HPFrame[i]->SetPosition({tex2DPos[i].m128_f32[0] - 80.0f, tex2DPos[i].m128_f32[1]});
	}

	//被ダメージ時
	if (RecvDamagef)
	{
		//イージング終了時各フラグ切る
		if (FrameScalingETime >= 1.0f)
		{
			InnerFrameScalingF = true;
			RecvDamagef = false;
		}
		FrameScalingETime_Inner = 0.0f;
		if (!InnerFrameScalingF)
		{
			OldFrameX_Inner = OldFrameX;
		}
		//敵の体力最大値から現在の体力の割合
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
		//イージング開始
		FrameScalingETime += 0.05f;
		//体力ゲージが0.0切れてない時ゲージ減ってく
		if (FrameScl.x > 0.0f)
		{
			FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;

		FrameScalingETime = 0.0f;
	}
	if (EnemyHP <= 0)
	{
		if (FrameScl_Inner.x > 0.f)
		{
			FrameScalingETime_Inner += 0.02f;
		}
		FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, 0.f);
		InnerFrameScalingF = false;
	}
	if (InnerFrameScalingF)
	{
		FrameScalingETime_Inner += 0.02f;
		//体力が０なったときだけEaseの終わりを０に
		if (EnemyHP <= 0)
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, 0.f);
			InnerFrameScalingF = false;
		}
		else
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, NowFrameX);
		}
		//ゲージの減りが止まったらフラグ切る
		if (FrameScalingETime_Inner >= 1.0f)
		{
			InnerFrameScalingF = false;
		}
	}

	/*0->黒枠 1->白枠 2->ゲージ外 3->ゲージ内*/
	HPFrame[3]->SetSize({FrameScl.x, 15});
	HPFrame[2]->SetSize({FrameScl_Inner.x, 15.0f});
	HPFrame[1]->SetSize({200.0f, 15.0f});
	HPFrame[0]->SetSize({200.0f, 15.0f});

	//敵の名前表示
	EnemyName->SetPosition({tex2DPos[0].m128_f32[0] - 80.0f, tex2DPos[0].m128_f32[1] - 30.f});
	EnemyName->SetSize({200.0f, 15.0f});

	//体力バーの横サイズ最大値
	FrameScl.x = max(FrameScl.x, 0.0f);
	FrameScl_Inner.x = max(FrameScl_Inner.x, 0.0f);
}

void Enemy::Respawn()
{
	//リスポーンカウント
	RespawnCount++;


	if (RespawnJudg() == true)
	{
		EvaMotionStart = false;
		MagicMotionStart = false;
		RoarMotionFlag = false;
		IdleMotionFlag = false;
		DeathMotionFlag = false;
		SideWalk_RightMotionFlag = false;
		SideWalk_LeftMotionFlag = false;
		DeathFlag = false;
		nowAttack = false;
		nowDeath = false;

		alpha = 1.f;
		Position = RespawnPos;
		EnemyHP = MaxHP;

		FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
	}
}

bool Enemy::RespawnJudg()
{
	bool respawnjudg = Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), Position) > 180.f;

	if (RespawnCount > RespawnCountMax && respawnjudg)
	{
		return true;
	}
	return false;
}

void Enemy::DamageTexDisplay()
{
	if (m_fbxObject == nullptr)
	{
		return;
	}
	//ダメージスプライト生成
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->DamageDisPlay(1, {1.f, 1.f, 1.f, 1.f});
	}

	//アルファ値一定以下なったら破棄
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex)
	{
		return dTex->GetAlpha() <= 0.1f;
	});
}

void Enemy::DamageTexDisplay_Draw()
{
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->Draw();
	}
}

void Enemy::isRespawn()
{
	//f_time = 0;
	//DeathFlag = false;
	//nowDeath = false;
}

void Enemy::ChangeState_Guardian(GuardianState* state)
{
	Destroy(state_guardian);
	state_guardian = state;
}

void Enemy::ChangeState_Mob(EnemyState* state)
{
	Destroy(state_mob);
	state_mob = state;
}

void Enemy::ChangeState_Boss(BossEnemyState* state)
{
	Destroy(state_boss);
	state_boss = state;
}

void Enemy::SetAnimation(int number, bool loop, double speed)
{
	m_AnimeSpeed = speed;
	if (m_Number != number)
	{
		m_AnimeLoop = loop;
		m_Number = number;
		m_fbxObject->PlayAnimation(m_Number);
	}
}
