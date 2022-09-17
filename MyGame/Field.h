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
	//Sprite
	Sprite* Explanation;
	Sprite* BossName;
	//Model
	Model* DamageAreaModel;
	Model* FieldModel;
	Model* CelestialSphereModel;
	Model* BackM;
private:
	float t;
	float TexAlpha_BossName;
	const float feedSpeed_BossName=0.005f;

	bool feed;
	bool feed_BossName;
	const float feedSpeed_Explanation = 0.005f;

	DirectX::XMFLOAT3 Ppos; 
public:
	bool Initialize(DebugCamera*camera);
	void Update(DebugCamera* camera);
	void Draw();
	void Finalize();
	void WarningDraw();

private:
	void SpriteFeed(float&alpha,bool&feed,const float feedSpeed,const float MaxAlphaValue);
	void FieldDamageAreaCol();
};

