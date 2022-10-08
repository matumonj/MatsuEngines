#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include"Model.h"
#include"DebugCamera.h"
#include<memory>
class TouchableObject;
class CollisionManager;
class Field
{
public:
	Field() {};
	~Field() ;
	static Field* GetInstance();

private:
	//ColObj
	TouchableObject* FieldObject;
	TouchableObject* BossFieldObject;
	//Obj
	std::unique_ptr <Object3d> CelestialSphereObject;
	std::unique_ptr <Object3d> BackObject;
	std::unique_ptr<Object3d>DamageAreaObj;
	std::unique_ptr<Object3d>miniObj;
	//Sprite
	Sprite* Explanation;
	Sprite* BossName;
	//Model
	Model* DamageAreaModel;
	Model* FieldModel;
	Model* BossFieldModel;
	Model* CelestialSphereModel;
	Model* BackM;
	DirectX::XMFLOAT3 ssp;

private:
	float t;
	float TexAlpha_BossName;
	const float feedSpeed_BossName=0.005f;

	bool feed;
	bool feed_BossName;
	const float feedSpeed_Explanation = 0.005f;

	DirectX::XMFLOAT3 Ppos; 
	DebugCamera* dc;
	float ypos;
public:
	void SetCamera(DebugCamera* camera) { dc = camera; }
	bool Initialize(DebugCamera*camera);
	void Update(DebugCamera* camera);
	void Draw();
	void Finalize();
	void WarningDraw();

	void MiniFieldDraw();
private:
	void SpriteFeed(float&alpha,bool&feed,const float feedSpeed,const float MaxAlphaValue);
	void FieldDamageAreaCol();
};

