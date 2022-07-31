#pragma once
#include "EnemyActionBase.h"

class EnemyAttackAction :public EnemyActionBase
{

	private:
		//Microosoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		//DirectX::���ȗ�
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
		using XMVECTOR = DirectX::XMVECTOR;
	public:
		static EnemyAttackAction* GetInstance()
		{
			static EnemyAttackAction instance;
			return &instance;
		}

		virtual STATE Run(Enemy* enemy);

	private:
		bool wf = true;
		bool sf = false;
		float BeforeRot;
		float rot;

		int RandMove = rand() % 120 - 120;
		int RandMovement = rand() % 380 + 220;

		float AfterRot;
		float RotTime = 0;
		int StayCount = 0;
	};

