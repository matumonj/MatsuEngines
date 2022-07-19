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
#include <iostream>
class MapCreate
{
public:
	MapCreate();
	static void LoadEnemyParam();
private:

	static std::ifstream file;
	static std::ifstream file2;
	static std::stringstream popcom;

	static std::stringstream popcom2;
	//Microosoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	static void SetBoxModel(DebugCamera* camera);
	static void UpdateBoxModel(DebugCamera* camera);
	static void DrawBoxModel();
	static void CreateMapMode();
	static void CreateMapMode_MoveCamera(DebugCamera*camera, XMFLOAT3 map);
	
	static void ImguiDraw_Enemy();
	static void EnemyArgment(DebugCamera* camera);
	static void EnemyDraw();

	static void ImguiDraw_Wood();
	static void WoodArgments(DebugCamera* camera);
	static void WoodDraw();
private:
	static BehaviorTree behavior;
	static Model* BoxModel;
	static Object3d* BoxObj;
	static Model* WoodModel;
	static Object3d* WoodObj;
	static bool savef;
	static std::vector<XMFLOAT3>EnemyPosition;
	static std::vector<XMFLOAT3>WoodPosition;
	static FILE* fp;
	static bool MoveFlags;
	static bool MobArgment;
	static bool WoodArgment;
	static bool BossArgment;
	static bool EnemyDelete;
	static bool WoodDelete;
	static float a;
	static XMFLOAT3 pos;
	static XMFLOAT3 wood_pos;
	static  std::vector <int>randmove;
	static std::vector <int> randmovement;
	static std::vector<int>Number;
	static std::vector<std::unique_ptr<Enemy>>enemys;
	static std::vector<std::unique_ptr<Wood>>woods;
private:

public:
	static XMFLOAT3 GetBoxPosition() { return BoxObj->GetPosition(); }
};

