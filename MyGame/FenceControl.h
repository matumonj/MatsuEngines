#pragma once
#include "ControlBase.h"
#include"AreaFence.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
#include"Wood.h"
#include"Player.h"
#include"Enemy.h"
#include"DebugCamera.h"
#include<memory>

class FenceControl :
	public ControlBase
{
public:
	~FenceControl() override
	{
	};
private:
	std::vector<std::unique_ptr<AreaFence>> fences;
	std::vector<XMFLOAT3> Load_FencePosition;
	std::vector<std::unique_ptr<AreaFence>> Tutorialfence;
	std::unique_ptr<AreaFence> BossGate;
	XMFLOAT3 Player_OldPos;

	bool TutorialFenceOpen;
	bool BossGateOpen;
public:
	AreaFence* GetBossGateFence() { return BossGate.get(); };
private:
	enum
	{
		TYUTORIAL,
		STAGE1_1,
	};

public:
	static FenceControl* GetInstance();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	void SetTutorialFenceOpen(bool judg) { TutorialFenceOpen = judg; }
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
