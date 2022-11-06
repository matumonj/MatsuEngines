#include "GigaBossEnemy.h"
#include"FbxLoader.h"
#include"CameraControl.h"

GigaBossEnemy* GigaBossEnemy::GetInstance()
{
	static  GigaBossEnemy ins;
	return &ins;
}
void GigaBossEnemy::Initialize(DebugCamera* camera)
{
	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("gigagolem"));
	m_fbxObject->PlayAnimation();
	Texture::LoadTexture(93, L"Resources/fissure.png");
	Position = { 0.0f,-56.0f,180.0f };
	Scale = { 0.5f,0.4f,0.4f };
	Rotation = { 0,180,0 };
	Texture*subFissureTex= Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	FissureTex.reset(subFissureTex);
	FissureTex->CreateTexture();
	FissureTex->SetAnchorPoint({ 0.5f,0.5f });
	fissureAlpha = 1.0f;
	disrad = 0;
}

void GigaBossEnemy::Update(DebugCamera* camera)
{
	fbxtimeControl();
	FissureTexUpdate(camera);
	ParameterSet_Fbx(camera);
}

void GigaBossEnemy::Draw()
{
	Texture::PreDraw();
	FissureTex->Draw();
	Texture::PostDraw();

	Draw_Fbx();
}

void GigaBossEnemy::fbxtimeControl()
{
	float SlamArmTime = 2.5f;
	
	
	if (!stopanimation) {
		f_time += 0.01f;
	}
	if (nowAttack==NON) {
		if (f_time >= SlamArmTime) {
			f_time = 0;
		}
	} else {
		if (f_time >= 3.9f && f_time <= 4.0f) {
			ArmonGround = true;
			CameraControl::GetInstance()->ShakeCamera();
		}
	}

	if (nowAttack == SLAM) {
		if (f_time <= 2.0f) {
			f_time = 2.0f;
		}
	}
	if (f_time >= m_fbxObject->GetEndTime()) {
		nowAttack = NON;
		f_time = 0;
	}
	m_fbxObject->SetFbxTime(f_time);
}

void GigaBossEnemy::FissureTexUpdate(DebugCamera* camera)
{
	FissureTex->SetRotation({ 90,0,0 });
	FissureTex->SetScale({ 8,8,3 });
	FissureTex->SetBillboard(false);
	FissureTex->SetColor({ 1.0f ,1.0f ,1.0f ,fissureAlpha });
	FissureTex->SetPosition({ 0.0f ,-18.0f ,30.0f });

	FissureTex->SetDisplayRadius(disrad);

	if (ArmonGround) {
		disrad += 0.5f;

		if (disrad >= 60.0f) {
			fissureAlpha -= 0.05f;
			if (fissureAlpha <= 0.0f) {
				ArmonGround = false;
		}
		}

	}
	else {
		fissureAlpha = 1.0f;
		disrad = 0.0f;
	}
	FissureTex->Update(camera);


	fissureAlpha = min(fissureAlpha, 1.0f);
	fissureAlpha = max(fissureAlpha, 0.0f);

	disrad = min(disrad, 60.0f);
	disrad = max(disrad, 0.0f);

}