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
	~WoodControl();
private:
	std::vector<std::unique_ptr<Wood>>woods;
	std::vector <XMFLOAT3>Load_WoodPosition;

	XMFLOAT3 Player_OldPos;
	std::vector<std::unique_ptr<Wood>>Tutorialwoods;

public:
	static WoodControl* GetInstance();

	void Load(DebugCamera* camera)override;

	void Finalize()override;

	void ImGuiDraw();

	Wood* GetWood(int index) { return Tutorialwoods[index].get(); }
private:
	void Update_Tutorial(DebugCamera*camera)override;

	void Update_Play(DebugCamera*camera)override;

	void Update_Boss(DebugCamera*camera)override;

	/*更新処理*/
	void Init_Tutorial(DebugCamera* camera)override;

	void Init_Play(DebugCamera* camera)override;

	void Init_Boss(DebugCamera* camera)override;

	/*更新処理*/
	void Draw_Tutorial()override;

	void Draw_Play()override;

	void Draw_Boss()override;
};

