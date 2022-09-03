#include "FollowAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
#include"PlayerControl.h"
using namespace DirectX;

EnemyActionBase::STATE FollowAction::Run(Enemy* enemy)
{

		return EnemyActionBase::STATE::COMPLETE;
	
}