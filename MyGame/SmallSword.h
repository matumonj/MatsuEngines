#pragma once
#include "SwordBase.h"
#include"ParticleManager.h"
#include"Texture.h"
#include"CollisionPrimitive.h"

class SmallSword :
	public SwordBase
{
public:
	~SmallSword() override;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	bool GetCameraTargetEnemy() { return CameraTargetEnemy; }
	void SetCameraTargetEnemy(bool f) { CameraTargetEnemy = f; }
private:
	enum AttackMotion
	{
		NON,
		FIRESPHERE,
		BLIZZARD
	};

	const int Damage_Value = 2;
	const int Damage_Value_S = 3;
	const int CoolTime_Value = 360;
	const int CoolTime_Value_S = 180;

	AttackMotion attackMotion = NON;
	DirectX::XMFLOAT3 EaseTime = {0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT3 magicSpherePos = {0.0f, 0.0f, 0.0f};
	ParticleManager* pMan = nullptr;

	DirectX::XMFLOAT3 Correction = {0.0f, 0.0f, 0.0f};
	bool CameraTargetEnemy = false;
	XMFLOAT3 OldCameraTargetPos = {0.0f, 0.0f, 0.0f};
	XMFLOAT3 OldTargetEnemyPos = {0.0f, 0.0f, 0.0f};
	XMFLOAT3 CameraPos = {0.0f, 0.0f, 0.0f};
	float EaseTime_CT = 0.0f;
	bool ReturnMoveCameraTarget = false;

private:
	void MagicAttack();
	/*------------*/
	/*    –‚–@•X   */
	/*------------*/
	class Blizzard
	{
	private:
		std::unique_ptr<Object3d> IceCrystalObj = nullptr;
		std::unique_ptr<Texture> IceMagicTex = nullptr;
	private:
		bool CollideEnemy = false;
		bool ActFlag = false;
		int IceExistence = 0;
		float DestTime = 0.0f;
		float DestAlpha = 1.0f;
		float TexAlpha = 1.0f;
		float EaseTime = 0.0f;
		XMFLOAT3 IcePos = {0.0f, 0.0f, 0.0f};
		XMFLOAT3 IceScl = {0.0f, 0.0f, 0.0f};
		XMFLOAT3 TexScl = {0.0f, 0.0f, 0.0f};
	public:
		void Init(DebugCamera* camera);
		void Updata(DebugCamera* camera);
		void Draw();
		void SetActFlag(bool f) { ActFlag = f; }
	public:
		enum Phase
		{
			NON,
			SHOTMAGICSPHERE,
			ACTIVE,
			DEST
		};

		Phase Getphase() { return phase; }
	private:
		Phase phase = NON;
	};

	/*----------------*/
	/*  –‚–@ŽaŒ‚(”ÍˆÍ)  */
	/*----------------*/

	class Slash
	{
	private:
		static const int TexNum = 15;
		std::unique_ptr<Texture> InpactTex[TexNum] = {nullptr};
		std::unique_ptr<Object3d> HolyObj = nullptr;
		ParticleManager* pMan[2] = {nullptr};
		bool CollideEnemy = false;
		bool ActFlag = false;
		//int IceExistence = 0;

		float EaseTime = 0.0f;
		XMFLOAT3 inpactTexPos = {0.0f, 0.0f, 0.0f};
		XMFLOAT3 inpactTexScl = {0.0f, 0.0f, 0.0f};
		XMFLOAT3 HolyScl = {0.0f, 0.0f, 0.0f};

		bool next = false;
		int rotCorrection_Value[TexNum];
		int posCorrection_Valuex[TexNum];
		int posCorrection_Valuey[TexNum];
		int posCorrection_Valuez[TexNum];
		XMFLOAT3 TexScale[TexNum] = {{0.0f, 0.0f, 0.0f}};
		float Alpha[TexNum] = {1.0f};
		int slashCount[TexNum] = {0};
	public:
		void Init(DebugCamera* camera);
		void Updata(DebugCamera* camera);
		void Draw();

		void SetActFlag(bool f) { ActFlag = f; }
	public:
		enum Phase
		{
			NON,
			SHOTMAGICSPHERE,
			ACTIVE,
			DEST
		};

		Phase Getphase() { return phase; }
	private:
		Phase phase = NON;
	};

private:
	std::unique_ptr<Slash> SlashArea = nullptr;
	std::unique_ptr<Blizzard> Bliz = nullptr;
};
