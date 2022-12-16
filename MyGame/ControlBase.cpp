#include "ControlBase.h"

ControlBase::~ControlBase()
{
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void (ControlBase::* ControlBase::initTable[])() = {
	nullptr, //タイトル
	&ControlBase::Init_Tutorial, //チュートリアル
	&ControlBase::Init_Play, //プレーシーン
	&ControlBase::Init_Boss, //ボスシーン
	nullptr, //マップエディタ,
	nullptr,
	nullptr
};

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
/*scneManagerのシーン数と合わせる*/
void (ControlBase::* ControlBase::updateTable[])() = {
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

void ControlBase::Update()
{
	//シーンに応じた更新処理
	(this->*updateTable[SceneManager::GetInstance()->GetScene()])();
}

void ControlBase::Initialize()
{
	//シーンに応じた更新処理
	(this->*initTable[SceneManager::GetInstance()->GetScene()])();
}

void ControlBase::Draw()
{
	//シーンに応じた更新処理
	(this->*drawTable[SceneManager::GetInstance()->GetScene()])();
}
