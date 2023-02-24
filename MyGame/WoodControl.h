#pragma once
#include "ControlBase.h"

#include"Wood.h"
#include"Enemy.h"
#include<memory>

class WoodControl :
	public ControlBase
{
	~WoodControl() override;
private:
	//木オブジェ
	std::vector<std::unique_ptr<WoodBase>> woods;

	//読み込み座標
	std::vector<XMFLOAT3> Load_WoodPosition;

	//チュートリアルの木は別で
	std::vector<std::unique_ptr<WoodBase>> Tutorialwoods;
	static constexpr int WoodSize = 5;
public:
	static WoodControl* GetIns();

	void Load() override;

	void Finalize() override;

	void ImGuiDraw();

	WoodBase* GetWood(int index) { return Tutorialwoods[index].get(); }
private:
	/*初期化処理*/
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*更新処理*/
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*描画処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;

private:
	enum
	{
		WOOD_A=1,
		WOOD_B=2
	};
};
