#include "HintBoard.h"
void HintBoard::Initialize( DebugCamera* camera)
{
	hintmodel = Model::CreateFromOBJ("hint");
	//モデル割り当て
	//MobObject = new Object3d();
	hintobj = Object3d::Create(camera);
	hintobj->SetModel(hintmodel);
	
	//パラメータのセット
	scale = { 2,2,2 };
	rotation = { 0,180,0 };
	position = { 14,-2,38 };

}

void HintBoard::Update(DebugCamera* camera)
{
	hintobj->SetPosition(position);
	hintobj->SetRotation(rotation);
	hintobj->SetScale(scale);
	hintobj->Update({ 1,1,1,1 },camera);
}
void HintBoard::Draw()
{
	hintobj->PreDraw();
	hintobj->Draw();
	hintobj->PostDraw();
}
void HintBoard::Finalize()
{
	delete hintobj, hintmodel;
}