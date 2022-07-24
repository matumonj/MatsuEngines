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
	static const int ObjType_Num=3;
	static CreateObjManager* placeobj[ObjType_Num];
public:
	static void ImGuiDraw();
	static void ObjectArgment(DebugCamera* camera);
	static void ObjectInitialize(DebugCamera*camera);
	static void ObjectUpdate(DebugCamera* camera);
	static void ObjectDraw();

private:
	static bool savef;
};

