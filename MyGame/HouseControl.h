#pragma once
#include "ControlBase.h"
#include"House.h"
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

#include "House.h"

class HouseControl :
	public ControlBase
{
public:
	~HouseControl() override
	{
	};
private:
	std::vector<std::unique_ptr< House>> houses;
	std::vector<XMFLOAT3> Load_HousePosition;
	XMFLOAT3 Player_OldPos;
	


public:
	static HouseControl* GetInstance();

	void Load(DebugCamera* camera) override;

	void Finalize() override;

	void ImGuiDraw();
private:
	void Init_Tutorial(DebugCamera* camera) override;

	void Init_Play(DebugCamera* camera) override;

	void Init_Boss(DebugCamera* camera) override;

	/*更新処理*/
	void Update_Tutorial(DebugCamera* camera) override;

	void Update_Play(DebugCamera* camera) override;

	void Update_Boss(DebugCamera* camera) override;

	/*更新処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;

};
