﻿#pragma once
#include "ControlBase.h"
#include"House.h"
#include"Enemy.h"
#include<memory>

class HouseControl :
	public ControlBase
{
public:
	~HouseControl() override
	{
	};
private:
	std::vector<std::unique_ptr<House>> houses;
	std::vector<XMFLOAT3> Load_HousePosition;
	XMFLOAT3 Player_OldPos;


public:
	static HouseControl* GetIns();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();
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
