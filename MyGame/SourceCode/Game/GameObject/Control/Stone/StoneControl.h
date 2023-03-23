#pragma once
#include "ControlBase.h"

#include"Stone.h"
#include"Enemy.h"
#include"Player.h"
#include"DebugCamera.h"
#include<memory>

class StoneControl :
	public ControlBase
{
	~StoneControl() override;
private:
	XMFLOAT3 defpos;
	std::vector<std::unique_ptr<Stone>> stones;
	std::vector<XMFLOAT3> Load_StonePosition;
	std::vector<XMFLOAT3> Rotation;
	XMFLOAT3 Player_OldPos;
	std::vector<std::unique_ptr<Stone>> Tutorialstones;

	enum StoneType
	{
		BIG_A=1,
		BIG_B=2
	};

	enum SceneStone
	{
		TUTORIAL,
		PLAY,
		BOSS,
		MAPCREATE
	};

public:
	static StoneControl* GetIns();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	Stone* GetStone(int index) { return Tutorialstones[index].get(); }
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
