#pragma once
#include "ControlBase.h"
#include"AreaFence.h"
#include"Enemy.h"
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

	bool OpenDebug;
public:
	AreaFence* GetBossGateFence() { return BossGate.get(); };
private:
	enum
	{
		TYUTORIAL,
		STAGE1_1,
	};

public:
	static FenceControl* GetIns();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	void SetTutorialFenceOpen(bool judg) { TutorialFenceOpen = judg; }
private:
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*�X�V����*/
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*�X�V����*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;
};
