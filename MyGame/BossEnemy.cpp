#include "BossEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;
BossEnemy::BossEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
	Enemy(ai_tree, max_hp, max_mp, attack, deffence)
{
	//m_BehaviorData = new BehaviorDatas();
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
BossEnemy::~BossEnemy()
{
	delete mob, MobModel;
}

//����������
void BossEnemy::Initialize(DebugCamera* camera)
{
	SetCollider();
}

//�X�V����
void BossEnemy::Update( DebugCamera* camera)
{

}

//�`�揈��
void BossEnemy::Draw()
{
	
}
//�������
void BossEnemy::Finalize()
{

}

void BossEnemy::Death()
{

}

void BossEnemy::Walk()
{

}

void BossEnemy::Stop()
{

}


void BossEnemy::Follow()
{
	animeflag = false;

	Turn_toPlayer();

	if (time > 210) {
		wf = true;
		endsearch = true;
		RecvDamageJudg = false;
		FollowFlag = false;
		time = 0;
	} else {
		time++;
		FollowFlag = true;
		wf = false;
		sf = false;
	}
}

void BossEnemy::FbxAnimationControl()
{

}
void BossEnemy::Attack()
{

}

void BossEnemy::AttackCoolTime()
{

}