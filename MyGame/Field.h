#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include"Model.h"
#include"DebugCamera.h"
#include<memory>
#include<array>
#include"ObjectManager.h"
#include"Texture.h"
#include"CollisionPrimitive.h"
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
	
	XMFLOAT3 GetClearTexpos() { return cleartex->GetPosition(); }
private:
	TouchableObject* FieldObject = nullptr;
	std::unique_ptr<Object3d> BossField;
	XMFLOAT3 BossFieldPos;

	//Obj
	enum ObjType
	{
		CELESTIALSPHERE,
		BOSSBACK,
		DAMAGEAREA,
		PEDESTAL
	};

	const static int objNum = 5;

	Texture* playerpoint = nullptr;
	std::vector<std::unique_ptr<Texture>> Enemyicon;
	std::unique_ptr<Object3d> m_object[objNum] = {nullptr};
	std::unique_ptr<Object3d> CelestialSphereObject;
	std::unique_ptr<Object3d> BackObject = nullptr;
	std::unique_ptr<Object3d> DamageAreaObj = nullptr;
	std::unique_ptr<Object3d> miniObj = nullptr;
	std::unique_ptr<Object3d> Pedestal;
	XMFLOAT3 pedestalpos;
	//Sprite
	Sprite* Explanation = nullptr;
	Sprite* BossName = nullptr;
	bool destf;
	XMFLOAT3 DestCenter;
	float destt=0;

	bool SkyMapDestF;
	float SkyMapDestT=1000.f;

	float KoloiamAlpha = 1.f;
	std::array<std::unique_ptr<Object3d>,4>SkyMap;
	//後半フェーズの塔オブジェ
	std::array<std::unique_ptr<Object3d>, 4>SkyBack;
	std::array<XMFLOAT3, 4>SkyBackPos;
	std::array<float, 4>SkyBackEaseT;
	std::array<float, 4>objroty;
	float celestalscl=16.f;
	//Model
	DirectX::XMFLOAT3 ssp = {0.0f, 0.0f, 0.0f};
	int EnemyIconSize = 0;
private:
	void GuardAreaTexUpda();
	static constexpr int GuardAreaSize = 16;
	std::array<std::unique_ptr<Texture>, GuardAreaSize> GuardArea;
	std::array<XMFLOAT3, GuardAreaSize> GuardAreaRot;
	std::array<float, GuardAreaSize> GuardAreaAngle;
	std::array<XMFLOAT3, GuardAreaSize> GuardareaPos;
	std::array<float, GuardAreaSize> GuardAreaAlphaEtime;
	std::array<float, GuardAreaSize> GuardAreaAlpha;
	Line2D camera_to_player;
	std::array<Point, GuardAreaSize> GuardAreaPoint;
	std::unique_ptr<Texture>cleartex;
	float cleartexrot;
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
	//void SetCamera() { dc = camera; }
	void Initialize() override;
	void Update() override;

	void Init_Play();
	void Init_Tutorial();
	void Init_Boss();

	void Update_Tutorial();
	void Update_Play();
	void Update_Boss();
	void Update_Edit();

	void SetnextStageF(bool f) { nextStageF = f; }
	bool GetnextStageF() { return nextStageF; }
	void Draw() override;
	void Finalize();
	void WarningDraw();
	void GuardAreaDraw();
	XMFLOAT3 GetPedestalPos() { return pedestalpos; }
	void MiniFieldDraw();
private:
	void SetFieldModel(ObjType type, Model* model, DebugCamera* camera);
	void SetFieldUpdate(ObjType type, DebugCamera* camera, XMFLOAT3 Pos, XMFLOAT3 Scl, bool uvscroll = FALSE,
	                    bool fog = FALSE);
	void ModelDraw_nullCheck(ObjType type);
	void SpriteFeed(float& alpha, bool& feed, float feedSpeed, float MaxAlphaValue);
	void FieldDamageAreaCol();


bool nextStageF;
	void BossFieldSet_PhaseOne();
	void BossFieldSet_PhaseTwo();
private:
	bool PedestalDownF;
	void PedestalMoving();
public:
	void NameDraw();
};
