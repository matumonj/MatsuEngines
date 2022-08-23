#include "DeathJudgment.h"
#include"Enemy.h"
#include"Collision.h"
bool DeathJudgment::Judgment(Enemy* enemy)
{
	if (enemy->GetHP() <= 0&&enemy->GetNowDeath()==false) {
		return true;
	}
	return false;
}