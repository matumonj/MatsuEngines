#include "ControlBase.h"

ControlBase::~ControlBase()
{
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void (ControlBase::* ControlBase::initTable[])(DebugCamera* camera) = {
	nullptr, //タイトル
	&ControlBase::Init_Tutorial, //チュートリアル
	&ControlBase::Init_Play, //プレーシーン
	&ControlBase::Init_Boss, //ボスシーン
	nullptr //マップエディタ
};

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
/*scneManagerのシーン数と合わせる*/
void (ControlBase::* ControlBase::updateTable[])(DebugCamera* camera) = {
	nullptr, //タイトル
	&ControlBase::Update_Tutorial, //チュートリアル
	&ControlBase::Update_Play, //プレーシーン
	&ControlBase::Update_Boss, //ボスシーン
	nullptr //マップエディタ
};

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void (ControlBase::* ControlBase::drawTable[])() = {
	nullptr, //タイトル
	&ControlBase::Draw_Tutorial, //チュートリアル
	&ControlBase::Draw_Play, //プレーシーン
	&ControlBase::Draw_Boss, //ボスシーン
	nullptr //マップエディタ
};

#include"SceneManager.h"

void ControlBase::Update(DebugCamera* camera)
{
	//シーンに応じた更新処理
	(this->*updateTable[SceneManager::GetInstance()->GetScene()])(camera);
}

void ControlBase::Initialize(DebugCamera* camera)
{
	//シーンに応じた更新処理
	(this->*initTable[SceneManager::GetInstance()->GetScene()])(camera);
}

void ControlBase::Draw()
{
	//シーンに応じた更新処理
	(this->*drawTable[SceneManager::GetInstance()->GetScene()])();
}
