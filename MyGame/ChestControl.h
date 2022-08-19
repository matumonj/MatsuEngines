#pragma once
#include"ControlBase.h"
#include"Chest.h"
class ChestControl :public ControlBase
{
public:
	static ChestControl* GetInstance();

	void Load(DebugCamera* camera)override;

	void Initialize(DebugCamera* camera)override;

	void Update(DebugCamera* camera)override;

	void Draw()override;

	void SetColor(XMFLOAT4 color)override;

	void GetChestAction(int index);

	void ChestDestroy(int index);
private:
	std::vector<std::unique_ptr<Chest>>chests;
	std::vector <XMFLOAT3>Load_ChestPosition;

	int GetChestCount = 0;

	int ChestMax;
};

