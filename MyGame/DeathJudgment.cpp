#include "DeathJudgment.h"
#include"Enemy.h"
#include"Collision.h"
bool DeathJudgment::Judgment(Enemy* enemy)
{
	if (enemy->GetHP() <= 100&&enemy->GetDeath()==false) {
		return true;
	}
	return false;
}