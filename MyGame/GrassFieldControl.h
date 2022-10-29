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
	~GrassFieldControl();
private:
	std::vector<std::unique_ptr<GrassField>>grassfields;
	std::vector <XMFLOAT3>Load_GrassFieldPosition;
	std::vector<XMFLOAT3>Rotation;
	std::vector<std::unique_ptr<GrassField>>Tutorialgrassfields;
	
	enum SceneGrassField {
		TUTORIAL,
		PLAY,
		BOSS,
		MAPCREATE
	};
public:
	static GrassFieldControl* GetInstance();

	void Load(DebugCamera* camera)override;

	void Initialize(DebugCamera* camera)override;


	void Draw()override;

	void Finalize()override;

	void ImGuiDraw();

	GrassField* GetGrassField(int index) { return Tutorialgrassfields[index].get(); }
private:
	void Update_Tutorial(DebugCamera* camera)override;

	void Update_Play(DebugCamera* camera)override;

	void Update_Boss(DebugCamera* camera)override;

};

