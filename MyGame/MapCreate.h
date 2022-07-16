#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<memory>
#include<list>
#include <DirectXMath.h>
#include"DebugCamera.h"
#include"Enemy.h"
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
	static std::stringstream popcom;
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
private:
	static BehaviorTree behavior;
	static Model* BoxModel;
	static Object3d* BoxObj;
	static bool savef;
	static std::vector<XMFLOAT3>EnemyPosition;
	static FILE* fp;
	static bool MoveFlags;
	static bool MobArgment;
	static bool BossArgment;
	static bool EnemyDelete;
	static float a;
	static XMFLOAT3 pos;
	static  std::vector <int>randmove;
	static std::vector <int> randmovement;
	static std::vector<int>Number;
	static std::vector<std::unique_ptr<Enemy>>enemys;

public:
	static XMFLOAT3 GetBoxPosition() { return BoxObj->GetPosition(); }
};

