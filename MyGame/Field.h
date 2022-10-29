#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include"Model.h"
#include"DebugCamera.h"
#include<memory>
#include"ObjectManager.h"
#include"Texture.h"
class TouchableObject;
class CollisionManager;
class Field:public ObjectManager
{
public:
	Field() {};
	~Field() ;
	static Field* GetInstance();
	void Setplay(DebugCamera* camera);
private:
	
	TouchableObject* FieldObject;
	//Obj
	enum ObjType {
		CELESTIALSPHERE,
		BOSSBACK,
		DAMAGEAREA,
		MINI
	};
	const static int objNum=4;

	Texture* playerpoint;
	std::vector<std::unique_ptr<Texture>>Enemyicon;
	std::unique_ptr<Object3d>m_object[objNum];
	std::unique_ptr <Object3d> CelestialSphereObject;
	std::unique_ptr <Object3d> BackObject;
	std::unique_ptr<Object3d>DamageAreaObj;
	std::unique_ptr<Object3d>miniObj;
	//Sprite
	Sprite* Explanation;
	Sprite* BossName;
	//Model
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
	void Initialize(DebugCamera*camera)override;
	void Update(DebugCamera* camera)override;

	void Update_Tutorial(DebugCamera* camera);
	void Update_Play(DebugCamera* camera);
	void Update_Boss(DebugCamera* camera);
	void Update_Edit(DebugCamera* camera);
	
	void Draw()override;
	void Finalize();
	void WarningDraw();

	void MiniFieldDraw();
private:
	void SetFieldModel(ObjType type, Model* model,DebugCamera*camera);
	void SetFieldUpdate(ObjType type, DebugCamera* camera,XMFLOAT3 Pos,XMFLOAT3 Scl,bool uvscroll=FALSE,bool fog=FALSE);
	void ModelDraw_nullCheck(ObjType type);
	void SpriteFeed(float&alpha,bool&feed,const float feedSpeed,const float MaxAlphaValue);
	void FieldDamageAreaCol();
};

