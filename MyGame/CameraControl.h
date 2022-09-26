#include"DebugCamera.h"
#include<windows.h>
#include "ControlBase.h"
#include"Input.h"
class CameraControl :
    public ControlBase
{
public:
    static CameraControl* GetInstance();
	void Load(DebugCamera* camera)override;

	void Initialize(DebugCamera* camera)override;

	void Update(DebugCamera* camera)override;

	void Draw()override;

    void Finalize()override;

    DebugCamera* GetCamera() { return camera; }
    enum TargetState {
        MAPCREATE,
        PLAYER,
        TUTORIAL,
        MOVEBOSSAREA,
        PLAYCUTSCENE,
        BOSSCUTSCENE
    };
private:
    Input* input;
    DebugCamera*camera;
    int pointsindex = 6;
    std::vector<XMFLOAT3>p;
    float elapsedTime;
    XMFLOAT3 pos1;
    XMFLOAT3 pos2;
    size_t startindex = 1;
    std::vector<XMFLOAT3>points;// { start, start, p2, p3, end, end };
    long long startCount = 0;
    long long nowCount = 0;
    long long elapsedCount = 0;

    float maxtime = 6.0f;
    float timerate;

    long long time = 0.00000f;
    long long j = time;

private:
    bool splinef;
    std::vector<std::unique_ptr<XMFLOAT3>>cpoints;
    std::vector <XMFLOAT3>Load_CameraPoints;

    XMFLOAT3 SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t);

    TargetState Tstate;
    XMFLOAT3 CameraPosition;
    float cameraAngle=-90.0f;
    float charaAngle;
    float CameraDis = 15.0f;
    float CameraHeight=20;
public: 

    XMFLOAT3 CameraPosIndex(int index) { return points[index]; }
    void SetCameraState(TargetState state) { Tstate = state; }
    TargetState GetCameraState() { return Tstate; }
    void SetColor(XMFLOAT4 color)override;
    void ShakeCamera();
private:
    bool BossCutSene = false;

public:
    enum BossCamera {//ボス登場時のカメラ挙動
        BOSSCUTSTART,
        ROLL,
        BOSSCUTEND,
    };
    enum SplineCamera {//フィールドざっくりと見回すカメラ挙動
        PLAYCUTSTART,
        SPLINE,
        FEED,
        PLAYCUTEND
    };
    enum MoveBossAreaCamera {
        NON,
        SHAKESTART,
        YPOSUP,
        TARGETPLAYER
    };
private:
    BossCamera bCamera;
    SplineCamera sCamera;
    MoveBossAreaCamera mCamera;
private:
    bool UpStage;
    int countAreaMove;
    float OldCameraPosY;
    float EaseTime=0;
private:
    int CutCount[3]={0};
    float BossCutAngle = 270.0f;
    bool AttackSceneF = false;
    float BCutCameraHeight = 5.0f;
private:
    float shakex = 0.0f;
    float shakey = 0.0f;
    float shake = 0.0f;
    float shaketime = 0.0f;
public:
    bool GetAttackSceneF() { return AttackSceneF; }
    int GetMoveBosAreaCam() { return mCamera; }
private:
    void BossSceneStart();
    void PlaySceneStart();
};
