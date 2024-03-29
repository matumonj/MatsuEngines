#pragma once
#include"DebugCamera.h"
#include"Enemy.h"
#include"Wood.h"
#include"CreateObjManager.h"

class MapCreate
{
public:
	MapCreate();

	static void LoadEnemyParam();
private:
	//設置オブジェクト[木・柵・敵]
	enum
	{
		WOOD=0,
		FENCE=1,
		ENEMY=2,
		CHEST=3,
		STONE=4,
		GRASS=5
	};

	//設置オブジェクト
	std::vector<CreateObjManager*> placeobj;
public:
	static MapCreate* GetIns();
	//ImGui
	void ImGuiDraw();
	//設置処理
	void ObjectArgment(DebugCamera* camera);
	//初期化
	void ObjectInitialize(DebugCamera* camera);
	//更新処理
	void ObjectUpdate(DebugCamera* camera);
	//描画
	void ObjectDraw();
	//警告表示
	bool Error();
	//
	void Finalize();
private:
	bool savef;
};
