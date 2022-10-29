#pragma once
#include"Player.h"
#include "ControlBase.h"
#include"Texture.h"
class PlayerControl :
    public ControlBase
{
public:
	~PlayerControl() {};
private:
	std::unique_ptr<Player>player;
	Input* input;
	XMFLOAT3 StartPos;
	XMFLOAT3 Player_OldPos;
	Sprite* DamageTex;
	float dalpha=0.0f;
private:
	enum {
		TYUTORIAL,
		STAGE1_1,
	};
public:
	static PlayerControl* GetInstance();

	void Load(DebugCamera* camera)override;

	void Initialize(DebugCamera* camera)override;

	void Draw()override;

	void Finalize()override;

	void ImGuiDraw();

	void DamageTexDraw();
	Player* GetPlayer() { return player.get(); }

private:
	void Update_Tutorial(DebugCamera*camera)override;

	void Update_Play(DebugCamera*camera)override;

	void Update_Boss(DebugCamera*camera)override;

};

