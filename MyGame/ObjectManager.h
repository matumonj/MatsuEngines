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

	virtual void Initialize() = 0;

	void ParameterSet_Obj();
	void ParameterSet_Fbx();
	void ParameterSet_Fbx2();
	void ParameterSet_Fbx3();

	virtual void Update() =0;

	void Draw_Obj();
	void Draw_Fbx();
	void Draw_Obj(Object3d* obj);
	virtual void Draw() = 0;
	void CollisionField();


	bool ModelSetJudg() { if (m_fbxObject != nullptr) { return true; }return false; }
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
	//FBX
	int FallGroundTime = 0;
	int m_Number = 0;
	bool m_AnimeLoop = false;
	double m_AnimeTimer = 0;
	double m_AnimeSpeed = 1;
	bool m_AnimationStop = false;
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
	void SetColors(XMFLOAT4 color) { Color = color; }
	double GetFbxTime() { return m_fbxObject->GetFTime(); }
	double GetAnimationTime() { return m_fbxObject->GetAnimeTime(); }

	double GetFbxTimeEnd() { return m_fbxObject->GetEndTime(); }
};
