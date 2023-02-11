#pragma once
#include"BossEnemyState.h"
#include"Enemy.h"

class BossEnemyAttackBrzBeam : public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	DirectX::XMVECTOR positionA;
	// DirectX::‚ğÈ—ª
	using XMVECTOR = DirectX::XMVECTOR;

	void TurnCenter(Enemy* enemy);
};
