#include "Field.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
Field::~Field()
{
	delete CelestialSphereModel, CelestialSphereObject;
	delete FieldObject, FieldModel;
}
Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}
bool Field::Initialize(DebugCamera* camera)
{
	CelestialSphereObject = std::make_unique<Object3d>();
	CelestialSphereModel = Model::CreateFromOBJ("skydome");
	FieldModel= Model::CreateFromOBJ("LowPoly_Landscape");

	//フィールドにモデル割り当て
	FieldObject = TouchableObject::Create(FieldModel,camera);

	CelestialSphereObject->Initialize(camera);
	CelestialSphereObject->SetModel(CelestialSphereModel);

	return true;
}

void Field::Update(DebugCamera* camera)
{
	CelestialSphereObject->SetPosition({ 0,30,0 });
	CelestialSphereObject->SetScale({ 30,30,30 });

	FieldObject->SetScale(1.0f);
	FieldObject->SetPosition({ 0,-20,0 });


	FieldObject->Update({ 0.6,0.6,0.6,1 }, camera);
	CelestialSphereObject->Update({ 1,1,1,1 }, camera);
}

void Field::Draw()
{
	CelestialSphereObject->PreDraw();
	CelestialSphereObject->Draw();
	CelestialSphereObject->PostDraw();

	FieldObject->PreDraw();
	FieldObject->Draw();
	FieldObject->PostDraw();
}