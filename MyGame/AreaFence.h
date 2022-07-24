#pragma once

#include<memory>
#include"OBBCollision.h"
#include"Model.h"
#include"Object3d.h"
class AreaFence
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	AreaFence() {};
	~AreaFence() {};

private:
	OBB playerOBB;
	OBB AreaFenceOBB;// = nullptr;
	OBBCollision* ps0 = nullptr;
	std::unique_ptr <Object3d>  AreaFenceObject;

	Model* AreaFenceModel;
	XMFLOAT3 Position;

	float FencePosY_Min;
public:
	bool Initialize(DebugCamera* camera);
	void Update(DebugCamera* camera);
	void Draw();
	
	void FenceOpenCondition(bool Condition);
	XMFLOAT3 GetPosition() { return Position; }
	void SetPosition(XMFLOAT3 pos) { Position = pos; }
	bool CollideAreaFence();
};

