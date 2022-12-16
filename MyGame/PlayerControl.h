#pragma once
#include"Player.h"
#include "ControlBase.h"
#include"Texture.h"

class PlayerControl :
	public ControlBase
{
public:
	~PlayerControl() override
	{
	};
private:
	std::unique_ptr<Player> player;
	Input* input;
	XMFLOAT3 StartPos;
	XMFLOAT3 Player_OldPos;
	Sprite* DamageTex;
	float dalpha = 0.0f;
private:
	enum
	{
		TYUTORIAL,
		STAGE1_1,
	};

public:
	static PlayerControl* GetInstance();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	void DamageTexDraw();
	Player* GetPlayer() { return player.get(); }

	void BossFieldCol();
	void DamageTexUpdate();
private:
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*更新処理*/
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*更新処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;
};
