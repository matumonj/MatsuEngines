#include "GrassField.h"
#include"PlayerControl.h"
#include"ModelManager.h"
GrassField::GrassField()
{

};
GrassField::~GrassField()
{

};
void GrassField::Initialize(DebugCamera* camera)
{
	FieldPos = { PlayerControl::GetInstance()->GetPlayer()->GetPosition() };
	FieldRot = { 0.0f,0.0f,0.0f };
	FieldScl={ 1.0f,1.0f,1.0f };
	FieldPos = { PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y -40,PlayerControl::GetInstance()->GetPlayer()->GetPosition().z };

	grass = std::make_unique<GrassObj>();
	grass->SetModel(ModelManager::GetIns()->GetModel(ModelManager::GRASSFIELD));
	grass->Initialize(camera);
}

void GrassField::Update(DebugCamera* camera)
{
	if (grass == nullptr)return;
	FieldRot = { 0.0f,0.0f,0.0f };
	FieldScl = { 1.0f,1.0f,1.0f };

	grass->SetPosition(FieldPos);
	grass->SetRotation(FieldRot);
	grass->SetScale(FieldScl);

	grass->Update({ 1,1,1,1 }, camera);
}
#include"imgui.h"
void GrassField::Draw()
{
	if (grass == nullptr)return;
	GrassObj::PreDraw();
	grass->Draw();
	GrassObj::PostDraw();
	ImGui::Begin("FieldPos");
	ImGui::SliderFloat("y", &FieldPos.y, 100, -100);
	ImGui::End();
}