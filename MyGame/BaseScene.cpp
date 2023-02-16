#include"BaseScene.h"
//コンストラクタ
BaseScene::BaseScene(SceneManager* sceneManager)
	: sceneManager_(sceneManager)
{

}

void BaseScene::LightSetParam(int index, bool actflag, XMFLOAT3 pos, XMVECTOR dir, XMFLOAT3 atten, XMFLOAT2 angle)
{
	lightGroup->SetCircleShadowCasterPos(index, {pos.x, pos.y, pos.z});
	lightGroup->SetCircleShadowAtten(index, atten);
	lightGroup->SetCircleShadowDir(index, dir);
	lightGroup->SetCircleShadowFactorAngle(index, angle);
}
