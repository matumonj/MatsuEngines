#pragma once

#include "EnemyActionBase.h"

class FollowAction :public EnemyActionBase
{
	private:
		//Microosoft::WRL::Çè»ó™
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		//DirectX::Çè»ó™
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
		using XMVECTOR = DirectX::XMVECTOR;
	public:
		static FollowAction* GetInstance()
		{
			static FollowAction instance;
			return &instance;
		}

		virtual STATE Run(Enemy* enemy);

		void Walk(Enemy* enemy);
		void Stop(Enemy* enemy);
	private:
		float tempx, tempz;
		int movement;
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

