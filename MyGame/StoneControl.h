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
	~StoneControl();
private:
	std::vector<std::unique_ptr<Stone>>stones;
	std::vector <XMFLOAT3>Load_StonePosition;
	std::vector<XMFLOAT3>Rotation;
	XMFLOAT3 Player_OldPos;
	std::vector<std::unique_ptr<Stone>>Tutorialstones;
	enum StoneType {
		BIG_A=1,BIG_B=2
	};
	enum SceneStone {
		TUTORIAL,
		PLAY,
		BOSS,
		MAPCREATE
	};
public:
	static StoneControl* GetInstance();

	void Load(DebugCamera* camera)override;

	void Initialize(DebugCamera* camera)override;


	void Draw()override;

	void Finalize()override;

	void ImGuiDraw();

	Stone* GetStone(int index) { return Tutorialstones[index].get(); }
private:
	void Update_Tutorial(DebugCamera* camera)override;

	void Update_Play(DebugCamera* camera)override;

	void Update_Boss(DebugCamera* camera)override;

};
