#pragma once
#include"Object3d.h"
#include"Model.h"
#include"f_Object3d.h"
#include"f_Model.h"
#include"DebugCamera.h"
#include"ModelManager.h"
#include<memory>

class ObjectManager
{
public:
	virtual ~ObjectManager();
protected:
	template <class T>
	using ComPtr = ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	virtual void SetCollider();

	virtual void Initialize(DebugCamera* camera) = 0;

	void ParameterSet_Obj(DebugCamera* camera);
	void ParameterSet_Fbx(DebugCamera* camera);
	void ParameterSet_Fbx2(DebugCamera* camera);

	virtual void Update(DebugCamera* camera) =0;

	void Draw_Obj();
	void Draw_Fbx();

	virtual void Draw() = 0;
	void CollisionField(DebugCamera* camera);

protected:
	std::unique_ptr<Object3d> m_Object = nullptr;
	Model* m_Model = nullptr;

	std::unique_ptr<f_Object3d> m_fbxObject = nullptr;
	f_Model* m_fbxModel = nullptr;

	XMFLOAT3 Position = {0.0f, 0.0f, 0.0f};
	XMFLOAT3 Rotation = {0.0f, 0.0f, 0.0f};
	XMFLOAT3 Scale = {0.0f, 0.0f, 0.0f};
	XMFLOAT4 Color = {0.0f, 0.0f, 0.0f, 0.0f};
	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	XMVECTOR fallV = {};
	float radius_adjustment = 0.0f;

protected:
	int savetime;
	int falltime;
	float ObjAlpha = 1.0f;
public:
	XMFLOAT3 GetPosition() { return Position; }
	XMFLOAT3 GetRotation() { return Rotation; }
	XMFLOAT3 GetScale() { return Scale; }
	void SetPosition(XMFLOAT3 position) { Position = position; }
	void SetRotation(XMFLOAT3 rotation) { Rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { Scale = scale; }
	void SetColor(XMFLOAT4 color) { Color = color; }
};
