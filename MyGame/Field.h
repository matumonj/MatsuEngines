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

class Field : public ObjectManager
{
public:
	Field()
	{
	};
	~Field() override;
	static Field* GetInstance();
	void Setplay(DebugCamera* camera);
private:
	TouchableObject* FieldObject = nullptr;

	//Obj
	enum ObjType
	{
		CELESTIALSPHERE,
		BOSSBACK,
		DAMAGEAREA,
		MINI
	};

	const static int objNum = 4;

	Texture* playerpoint = nullptr;
	std::vector<std::unique_ptr<Texture>> Enemyicon;
	std::unique_ptr<Object3d> m_object[objNum] = {nullptr};
	std::unique_ptr<Object3d> CelestialSphereObject;
	std::unique_ptr<Object3d> BackObject = nullptr;
	std::unique_ptr<Object3d> DamageAreaObj = nullptr;
	std::unique_ptr<Object3d> miniObj = nullptr;
	//Sprite
	Sprite* Explanation = nullptr;
	Sprite* BossName = nullptr;
	//Model
	DirectX::XMFLOAT3 ssp = {0.0f, 0.0f, 0.0f};
	int EnemyIconSize = 0;
private:
	float t = 0.0f;
	float TexAlpha_BossName = 0.0f;
	const float feedSpeed_BossName = 0.005f;

	bool feed = false;
	bool feed_BossName = false;
	const float feedSpeed_Explanation = 0.005f;

	DirectX::XMFLOAT3 Ppos = {0.0f, 0.0f, 0.0f};
	float CelestalRot = 0.0f;
	DebugCamera* dc = nullptr;
	float ypos = 0.0f;
	XMFLOAT3 FogCenterPos = {0.0f, 0.0f, 0.0f};
public:
	void SetFogCenterPos(XMFLOAT3 pos) { FogCenterPos = pos; }
	void SetCamera(DebugCamera* camera) { dc = camera; }
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;

	void Update_Tutorial(DebugCamera* camera);
	void Update_Play(DebugCamera* camera);
	void Update_Boss(DebugCamera* camera);
	void Update_Edit(DebugCamera* camera);

	void Draw() override;
	void Finalize();
	void WarningDraw();

	void MiniFieldDraw();
private:
	void SetFieldModel(ObjType type, Model* model, DebugCamera* camera);
	void SetFieldUpdate(ObjType type, DebugCamera* camera, XMFLOAT3 Pos, XMFLOAT3 Scl, bool uvscroll = FALSE,
	                    bool fog = FALSE);
	void ModelDraw_nullCheck(ObjType type);
	void SpriteFeed(float& alpha, bool& feed, float feedSpeed, float MaxAlphaValue);
	void FieldDamageAreaCol();
};
