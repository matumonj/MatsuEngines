#pragma once
#include"Enemy.h"
#include"Sprite.h"
#include"Player.h"
#include<memory>
#include<vector>

class HUD
{
public:
	~HUD();
	void playerini();
	static HUD* GetIns();
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

	Sprite* PlayerHP = nullptr;
	Sprite* PlayerHPFrame = nullptr;
	Sprite* PlayerHPFrame2 = nullptr;
	Sprite* MiniMapFrame = nullptr;
	XMFLOAT2 framescl = {0.0f, 0.0f};
	XMFLOAT2 framepos = {0.0f, 0.0f};
	Sprite* ButtonFrame = nullptr;
	XMFLOAT2 MiniFrameSize = {0.0f, 0.0f};
	XMFLOAT2 MiniframePos = {0.0f, 0.0f};
	float PlayerHPSize = 0.0f;
	float OldPlayerHPSize = 0.0f;
	bool RecvDamageflag = false;
	float easetime = 0.0f;
	float CoolTime_Time = 0.0f;
	float TimeSpeed = 0.0f;
	bool iconchangeF = false;
public:
	bool GetLayOutMode();
	void SetPosition_PlayerHP(XMFLOAT2 position);
	void SetAnchor_PlayerHP(XMFLOAT2 position);
	//void SetOldHP()
	Sprite* GetPlayerHP() { return PlayerHP; }
	Sprite* GetEnemyHP() { return EnemyHP_Border; }
	XMFLOAT2 GetSkillButtonPosition() { return CenterPosition; }

	enum SkillIcon
	{
		SWORD,
		WAND,
		AXE
	} skillicon = SWORD;

	void SetSkillIcon(SkillIcon icon);
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void EnemyHPGaugeInitialize();
	void EnemyHPGaugeUpdate(std::vector<std::unique_ptr<Enemy>>& enemy);
public: //ìGëÃóÕÇÃHUD
	Sprite* EnemyHP_Border = nullptr;
	Sprite* EnemyHP_Inner = nullptr;
	Sprite* TaskSprite = nullptr;
	float taskAlpha = 0.0f;
	XMFLOAT2 taskSpriteSize = {0.0f, 0.0f};
	bool taskfeed = false;
	std::vector<Texture*> EnemyHP_Border_Multi = {};
	std::vector<Texture*> EnemyHP_Inner_Multi = {};
public:
	void EnemyHPGauge_MultiInitialize();
	void EnemyHPGauge_MultiUpdate(bool& loadf, DebugCamera* camera, std::vector<std::unique_ptr<Enemy>>& enemy);
	void EnemyHPGauge_MultiDraw();
	XMFLOAT2 GetCenterPosition() { return CenterPosition; }
	void SetCenterPosition(XMFLOAT2 pos) { CenterPosition = pos; }
	float GetCoolTime() { return CoolTime_Time; }
	XMFLOAT2 GetMinimapFramePos() { return MiniframePos; };
	Sprite* GetMinimapSprite() { return MiniMapFrame; }
private:
	XMFLOAT2 CenterPosition = {1050.0f, 500.0f};
	Sprite* ChestCollect[5] = {nullptr};
	Sprite* ChestCollectFrame = nullptr;
	XMFLOAT2 CooltimeSize = {0.0f, 0.0f};
public:
	void SkillButtonInitialize();
	void SkillBottonUpdate();
	void SetMax();
	void TaskUpdate(DebugCamera* camera);
	void SkillBottonDraw();
	void ChangePosition(XMFLOAT2 MousePosition, XMFLOAT2& c);
	void AreaName();
	void SetRecvDamageFlag(bool f) { RecvDamageflag = f; }
	bool GetRecvDamageFlag() { return RecvDamageflag; }
private:
	bool f = false;
	float nowhp = 0.0f;
	XMFLOAT2 sizel = {0.0f, 0.0f};
	float oldhp = 0.0f;
	float Hpt = 0.0f;

	bool EnemyHPDrawFlag = false;
	//bool load;
	std::vector<float> multi_NowHP = {};
	std::vector<XMFLOAT3> multi_sizel = {};
	std::vector<float> multi_OldHP = {};
	std::vector<float> multi_Hpt = {};
	std::vector<float> Alpha = {};
};
