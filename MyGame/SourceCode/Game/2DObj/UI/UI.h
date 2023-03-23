#pragma once
#include"DebugCamera.h"
#include"HUD.h"

class UI
{
private:
	bool TurnOffDrawUI;
	std::vector<Enemy*> Subenemys;
public:
	static UI* GetIns();

	void Initialize();

	void HUDUpdate(bool& hudload, DebugCamera* camera);

	void Update(bool& hudload, DebugCamera* camera);

	void Finalize();
	void HUDDraw();
	void Draw();
	void AreaNameDraw();
public:
	void SetTurnoffUIDraw(bool f) { TurnOffDrawUI = f; }
	bool GetTurnoffUIDraw() { return TurnOffDrawUI; }

	void SetRadBlur(bool active) { BlurF = active; }
	float GetBlurPower() { return bloomval; }

	void TwistEffect(bool f,float radiusAddVal,float PowerAddVal);

	float GetTwistRad() { return TwistRad; }
	float GetTwistPower() { return TwistPower; }
	bool TwistF;
	float TwistPower, TwistRad;
private:
	bool BlurF;
	float EaseT;
	float bloomval;


	enum Selectex
	{
		
		EXPLO,
		BOSS,
		FRAME
	};

	float FrameX[2] = { 800.f,1200.f };
	bool SelectF;
	int index;

	std::array<bool, 3>AreaSel_F;
	std::array<float, 3>AreaSel_EaseC;
	std::array<float, 3>AreaSel_Size;

	std::array<std::unique_ptr<Sprite>,3>AreaSel;

	//ÉVÅ[ÉìêÿÇËë÷Ç¶
	bool SceneChange_Boss;
	bool SceneChange_Explo;

	static constexpr float AddEaseTime = 0.02f;
public:
	void AreaSelectTexInit();
	void AreaSelectTexUpda();
	bool GetAreaSelF() { return SelectF; }

	bool GetSceneChange_Boss() { return SceneChange_Boss; }
	bool GetSceneChange_Explo() { return SceneChange_Explo; }

	void SelectScene_Open();
	void SelectScene_Close();
};
