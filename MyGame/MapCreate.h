#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<memory>
#include<list>
#include <DirectXMath.h>
#include"DebugCamera.h"
#include"Enemy.h"
#include"Wood.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include"AreaFence.h"
#include <iostream>
#include"PlaceFence.h"
#include"PlaceWood.h"
#include"PlaceEnemy.h"
#include"CreateObjManager.h"
class MapCreate
{
public:
	MapCreate();
	static void LoadEnemyParam();
private:
	//設置オブジェクトの数[木・柵・敵]
	static const int ObjType_Num=3;
	//設置オブジェクト
	static std::vector<CreateObjManager*> placeobj;
public:
	//ImGui
	static void ImGuiDraw();
	//設置処理
	static void ObjectArgment(DebugCamera* camera);
	//初期化
	static void ObjectInitialize(DebugCamera*camera);
	//更新処理
	static void ObjectUpdate(DebugCamera* camera);
	//描画
	static void ObjectDraw();

private:
	static bool savef;
};

