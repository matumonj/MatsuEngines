#pragma once
#include "ControlBase.h"

#include"GrassField.h"
#include"Enemy.h"
#include"Player.h"
#include"DebugCamera.h"
#include<memory>

class GrassFieldControl :
	public ControlBase
{
	~GrassFieldControl() override;
private:
	std::vector<std::unique_ptr<GrassField>> grassfields;
	std::vector<XMFLOAT3> Load_GrassFieldPosition;
	std::vector<XMFLOAT3> Rotation;
	std::vector<std::unique_ptr<GrassField>> Tutorialgrassfields;

	enum SceneGrassField
	{
		TUTORIAL,
		PLAY,
		BOSS,
		MAPCREATE
	};

public:
	static GrassFieldControl* GetInstance();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	GrassField* GetGrassField(int index) { return Tutorialgrassfields[index].get(); }
private:
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;
};
