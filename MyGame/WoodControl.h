#pragma once
#include "ControlBase.h"

#include"Wood.h"
#include"Enemy.h"
#include"Player.h"
#include"DebugCamera.h"
#include<memory>

class WoodControl :
	public ControlBase
{
	~WoodControl() override;
private:
	std::vector<std::unique_ptr<WoodBase>> woods;
	std::vector<XMFLOAT3> Load_WoodPosition;

	XMFLOAT3 Player_OldPos;
	std::vector<std::unique_ptr<WoodBase>> Tutorialwoods;

public:
	static WoodControl* GetInstance();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	WoodBase* GetWood(int index) { return Tutorialwoods[index].get(); }
private:
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*更新処理*/
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*更新処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;
};
