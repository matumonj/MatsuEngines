#include "EnemyActionBase.h"

// ñÇñ@çsìÆ
class StayAction :public EnemyActionBase
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
	static StayAction* GetInstance()
	{
		static StayAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy* enemy);

private:

private:
	float BeforeRot;
	float rot;
	float tempx, tempz;
	float AfterRot;
	float RotTime = 0;
	int StayCount = 0;
};