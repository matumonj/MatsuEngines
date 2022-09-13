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

    DebugCamera* GetCamera() { return camera; }
    enum TargetState {
        PLAYER,
        TUTORIAL,
        SPLINE,
        BOSSCUTSCENE
    };
private:
    Input* input;
    DebugCamera*camera;
    int pointsindex = 6;
    std::vector<XMFLOAT3>p;
   
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
    float CameraHeight=12;
public: 

    XMFLOAT3 CameraPosIndex(int index) { return points[index]; }
    void SetCameraState(TargetState state) { Tstate = state; }
    TargetState GetCameraState() { return Tstate; }
    void move(XMFLOAT3& pos);
    void SetColor(XMFLOAT4 color)override;

private:
    bool BossCutSene = false;
    enum BossCamera {
        START,
        ROLL,
        END,
    };
    int CutCount[3]={0};
    float BossCutAngle = 270.0f;
    BossCamera bCamera;
    bool AttackSceneF = false;
    float BCutCameraHeight = 5;
public:
    void BossSceneStart();
    bool GetAttackSceneF() { return AttackSceneF; }
};
