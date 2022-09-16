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
	TouchableObject* FieldObject;
	TouchableObject* BossFieldObject;
	std::unique_ptr <Object3d> CelestialSphereObject;

	std::unique_ptr <Object3d> BackObject;

	std::unique_ptr<Object3d>DamageAreaObj;

	Sprite* Explanation;
	Sprite* BossName;
	float TexAlpha;
	bool feed;
	Model* DamageAreaModel;
	Model* FieldModel;
	Model* CelestialSphereModel;
	Model* BackM;
	DirectX::XMFLOAT3 Ppos; 
public:
	bool Initialize(DebugCamera*camera);
	void Update(DebugCamera* camera);
	void Draw();
	void Finalize();
	void WarningDraw();
};

