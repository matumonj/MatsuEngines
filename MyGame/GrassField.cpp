#include "GrassField.h"

#include "CameraControl.h"
#include"PlayerControl.h"
#include"ModelManager.h"

GrassField::GrassField()
{
};

GrassField::~GrassField()
{
};

void GrassField::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//FieldPos = { PlayerControl::GetIns()->GetPlayer()->GetPosition() };
	FieldRot = {0.0f, 0.0f, 0.0f};
	FieldScl = {1.0f, 1.0f, 1.0f};
	//FieldPos = { PlayerControl::GetIns()->GetPlayer()->GetPosition().x,PlayerControl::GetIns()->GetPlayer()->GetPosition().y -40,PlayerControl::GetIns()->GetPlayer()->GetPosition().z };

	grass = std::make_unique<GrassObj>();
	grass->SetModel(ModelManager::GetIns()->GetModel(ModelManager::GRASSFIELD));
	grass->Initialize(camera);
	FieldRot = {0.0f, 0.0f, 0.0f};
	FieldScl = {1.0f, 1.0f, 1.0f};
}

void GrassField::Update()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	if (grass == nullptr)
	{
		return;
	}


	grass->SetPosition(FieldPos);
	grass->SetRotation(FieldRot);
	grass->SetScale(FieldScl);

	grass->Update({1, 1, 1, 1}, camera);
}

#include"imgui.h"

void GrassField::Draw()
{
	if (grass == nullptr)
	{
		return;
	}
	GrassObj::PreDraw();
	grass->Draw();
	GrassObj::PostDraw();
}
