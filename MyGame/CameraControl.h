#include"DebugCamera.h"
#include<windows.h>
#include "ControlBase.h"
#include"Input.h"

class CameraControl :
	public ControlBase
{
public:
	static CameraControl* GetInstance();
	void Load() override;
	void ParamSet();
	void Finalize() override;

	DebugCamera* GetCamera() { return camera; }

	enum TargetState
	{
		MAPCREATE,
		PLAYER,
		TUTORIAL,
		MOVEBOSSAREA,
		PLAYCUTSCENE,
		GUARDIANCUTSCENE,
		BOSSCUTSCENE,
		RUSHSCENE,
		ENDSCENE
	};

private:
	Input* input = nullptr;
	DebugCamera* camera = nullptr;
	int pointsindex = 6;
	std::vector<XMFLOAT3> p = {{0.0f, 0.0f, 0.0f}};
	float elapsedTime = 0.0f;
	XMFLOAT3 pos1 = {0.0f, 0.0f, 0.0f};
	XMFLOAT3 pos2 = {0.0f, 0.0f, 0.0f};;
	size_t startindex = 1;
	std::vector<XMVECTOR> points = {};
	float startCount = 0.0f;
	float nowCount = 0.0f;
	float elapsedCount = 0.0f;

	float maxtime = 6.0f;
	float timerate = 0.0f;

	float time = 0.00000f;
	float j = time;

private:
	bool splinef = false;
	std::vector<XMVECTOR> Load_CameraPoints = {};

	XMVECTOR SplinePosition(const std::vector<XMVECTOR>& points, size_t startindex, float t);

	TargetState Tstate;
	XMFLOAT3 CameraPosition = {0.0f, 0.0f, 0.0f};
	XMFLOAT2 dis = {0.0f, 0.0f};
	XMFLOAT2 distance = {0.0f, 0.0f};
	float angle = 180.0f;
	float cameraAngle = -90.0f;
	float charaAngle = 0.0f;
	float CameraDis = 15.0f;
	float CameraHeight = 10.0f;
public:
	std::vector<XMVECTOR> point;

	XMFLOAT3 CameraPointIndex(int index)
	{
		return {points[index].m128_f32[0], points[index].m128_f32[1], points[index].m128_f32[2]};
	}

	void SetCameraState(TargetState state) { Tstate = state; }
	TargetState GetCameraState() { return Tstate; }
	void ShakeCamera();
private:
	bool BossCutSene = false;

public:
	enum GuardianEnemyEncount
	{
		START,
		BATTLE,
		END
	};

	enum BossCamera
	{
		//ボス登場時のカメラ挙動
		BOSSCUTSTART,
		CAMERADOWN,
		ROLL,
		BOSSCUTEND,
	};

	enum SplineCamera
	{
		//フィールドざっくりと見回すカメラ挙動
		PLAYCUTSTART,
		SPLINE,
		FEED,
		PLAYCUTEND
	};

	enum BossAreaCamera
	{
		NON,
		FEED_BOSS,
		TARGETFENCE,
		TARGETPLAYER,
		END_BOSS,
	};

	enum GuardianCamera
	{
		NON_GUAR,
		FEED_GUAR,
		PEDESTALMOVING,
		GUARDIANAPPEAR,
		END_GUAR
	};

	enum RushAttackCamera
	{
		NON_RUSH,
		UPBOSS,
		RETURNPLAYER
	};

	enum PlayerDeathCamera
	{
		NON_PDEATH,
		UPDA_PDEATH,
		END_PDEATH
	};
	SplineCamera GetCameraState_Spline() { return sCamera; }
private:
	BossCamera bCamera;
	SplineCamera sCamera;
	BossAreaCamera mCamera;
	RushAttackCamera rCamera;
	GuardianCamera gcamera;
	PlayerDeathCamera pdcamera;
private:
	bool UpStage;
	int countAreaMove;
	float OldCameraPosY;
	float EaseTime = 0;
private:
	int CutCount[3] = {0};
	float BossCutAngle = 270.0f;
	bool AttackSceneF = false;
	float BCutCameraHeight = 5.0f;
	float CameraEaseT;
private:
	float rCameraEtime;
	XMFLOAT3 OldPos;
	XMFLOAT3 OldTarget;
	XMFLOAT3 TargetPos;
	XMFLOAT3 rCameraPos;
private:
	bool EncountFlag;
	XMFLOAT3 OldCameraPos;
	XMFLOAT3 OldCameratarget;
	XMFLOAT3 EaseTime_EnGuar_target = {0.0f, 0.0f, 0.0f};
	XMFLOAT3 EaseTime_EnGuar_pos = {0.0f, 0.0f, 0.0f};
	int waitCount;


private:
	float shakex = 0.0f;
	float shakey = 0.0f;
	float shake = 0.0f;
	float shaketime = 0.0f;
public:
	bool GetAttackSceneF() { return AttackSceneF; }
	void SetAttackSceneF(bool f) { AttackSceneF = f; }
	int GetMoveBosAreaCam() { return mCamera; }
private:
	void BossSceneStart();
	void BossCutScene_Spline();
	void BossCutScene_Start(XMFLOAT3 BossPos);
	void PlaySceneStart();
	void GuardianCutScene();
	void RushTargetBoss();
	void BossDeathStart();
private:
	void AngleRotation();

	void TargetPlayer();
	void TargetBossField();

	static void (CameraControl::* targetTable[])();
private:
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*更新処理*/
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*更新処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;
};
