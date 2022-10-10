#include "ControlBase.h"
ControlBase::~ControlBase()
{

}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
/*scneManagerのシーン数と合わせる*/
void (ControlBase::* ControlBase::updateTable[])(DebugCamera* camera) = {
	nullptr,//タイトル
	&ControlBase::Update_Tutorial,//チュートリアル
	&ControlBase::Update_Play,//プレーシーン
	&ControlBase::Update_Boss,//ボスシーン
	nullptr//マップエディタ
};

#include"SceneManager.h"
void ControlBase::Update(DebugCamera* camera)
{
	//シーンに応じた更新処理
	(this->*updateTable[SceneManager::GetInstance()->GetScene()])(camera);
}