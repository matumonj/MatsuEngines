
#pragma once
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"

#include"Player.h"
#include<memory>
#include"OBBCollision.h"
	class Wood
	{
	private: // エイリアス
// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
		using XMVECTOR = DirectX::XMVECTOR;

	public:
		Wood() {};
		~Wood() {};
		
	private:
		OBB playerOBB;
		OBB woodOBB;// = nullptr;
		OBBCollision* ps0 = nullptr;
		std::unique_ptr <Object3d>  WoodObject;

		Model* WoodModel;
		XMFLOAT3 Position;
	public:
		bool Initialize(DebugCamera* camera);
		void Update(DebugCamera* camera);
		void Draw();
		float randRot = rand() % 30 - 60;
		float randRotZ = rand() % 30 - 60;
		XMFLOAT3 GetPosition() { return Position; }
		void SetPosition(XMFLOAT3 pos) { Position = pos; }
		bool CollideWood();
	};

