#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"BossEnemyStay.h"
#include"PlayerControl.h"
#include"GuardianAppearState.h"
using namespace DirectX;

Enemy::Enemy()
{
	//��Ԃ̏�����
	state_boss = new BossEnemyStay();
	state_mob = new EnemyWalkState();
	state_guardian = new GuardianAppearState();
}

Enemy::~Enemy()
{
	//��Ԃ̔j��
	Destroy(state_mob);
	Destroy(state_boss);
	Destroy(state_guardian);
}


void Enemy::Action()
{
	AttackCoolTime();
}

#include"PlayerAttackState.h"
#include"CameraControl.h"

void Enemy::RecvDamage(int Damage)
{
	//��_���[�W���̍s��
	//��O����
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

	/*�_���[�W�e�N�X�`���̕\��*/
	{
		DamageSize = Damage;
		DamageTexPos = Position;
		std::unique_ptr<DamageManager> newdTex;
		//�K�[�f�B�A���̂ݍ��W�����̂ł��炷
		if (ENumber == GUARDIAN)
		{
			//�C���X�^���X����
			newdTex = std::make_unique<DamageManager>(
				XMFLOAT3(Position.x + rand() % 10 - 5, -10 + Position.y + rand() % 10 - 5, Position.z), Damage);
		}
		else
		{
			//�C���X�^���X����
			newdTex = std::make_unique<DamageManager>(
				XMFLOAT3(Position.x + rand() % 10 - 5, Position.y + rand() % 10 - 5, Position.z), Damage);
		}
		//���X�g�ɒǉ�
		dMans_.push_back(std::move(newdTex));
	}
	/*----------------------*/

	//�̗͌��炷
	EnemyHP = EnemyHP - Damage;
	//�_���[�W�p�[�e�B�N���\��
	DamageParticleCreateF = true;
}

void Enemy::HPFrameUpda()
{
	//�J�����̃C���X�^���X�擾
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	//2D->3D�ϊ��p
	XMVECTOR tex2DPos[4];

	for (int i = 0; i < 4; i++)
	{
		//w���Z�ō��W���QD�ϊ�
		tex2DPos[i] = {Position.x, Position.y + 13.0f, Position.z};
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetProjectionMatrix(), true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewPort(), false);

		//���W�Z�b�g
		HPFrame[i]->SetPosition({tex2DPos[i].m128_f32[0] - 80.0f, tex2DPos[i].m128_f32[1]});
	}

	//��_���[�W��
	if (RecvDamagef)
	{
		//�C�[�W���O�I�����e�t���O�؂�
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
		//�G�̗͍̑ő�l���猻�݂̗̑͂̊���
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
		//�C�[�W���O�J�n
		FrameScalingETime += 0.05f;
		//�̗̓Q�[�W��0.0�؂�ĂȂ����Q�[�W�����Ă�
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
		//�̗͂��O�Ȃ����Ƃ�����Ease�̏I�����O��
		if (EnemyHP <= 0)
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, 0.f);
			InnerFrameScalingF = false;
		}
		else
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, NowFrameX);
		}
		//�Q�[�W�̌��肪�~�܂�����t���O�؂�
		if (FrameScalingETime_Inner >= 1.0f)
		{
			InnerFrameScalingF = false;
		}
	}

	/*0->���g 1->���g 2->�Q�[�W�O 3->�Q�[�W��*/
	HPFrame[3]->SetSize({FrameScl.x, 15});
	HPFrame[2]->SetSize({FrameScl_Inner.x, 15.0f});
	HPFrame[1]->SetSize({200.0f, 15.0f});
	HPFrame[0]->SetSize({200.0f, 15.0f});

	//�G�̖��O�\��
	EnemyName->SetPosition({tex2DPos[0].m128_f32[0] - 80.0f, tex2DPos[0].m128_f32[1] - 30.f});
	EnemyName->SetSize({200.0f, 15.0f});

	//�̗̓o�[�̉��T�C�Y�ő�l
	FrameScl.x = max(FrameScl.x, 0.0f);
	FrameScl_Inner.x = max(FrameScl_Inner.x, 0.0f);

}
void Enemy::Respawn()
{
	//���X�|�[���J�E���g
	RespawnCount++;


	if (RespawnJudg() == true) {
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
	bool respawnjudg = Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position) > 180.f;

	if (RespawnCount > RespawnCountMax&&respawnjudg)
	{
		return true;
	}
	return false;
}

void Enemy::DamageTexDisplay()
{
	//�_���[�W�X�v���C�g����
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->DamageDisPlay(1, {1.f, 1.f, 1.f, 1.f});
	}

	//�A���t�@�l���ȉ��Ȃ�����j��
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex)
	{
		return dTex->GetAlpha() <= 0.1f;
	});
}

void Enemy::DamageTexDisplay_Draw()
{
	if (this == nullptr)
	{
		return;
	}
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

void Enemy::SetAnimation(int number, bool loop,double speed)
{
	if (m_Number != number)
	{
		m_AnimeLoop = loop;
		m_Number = number;
		m_AnimeSpeed = speed;
		m_fbxObject->PlayAnimation(m_Number);
	}
}