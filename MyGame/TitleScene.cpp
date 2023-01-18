#include "TitleScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Feed.h"
#include "ImageManager.h"

TitleScene::TitleScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------����������--------*/
/*-----------------------*/
void TitleScene::Initialize()
{
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);

	Feed::GetInstance()->initialize();

	//�t�B�[���h
	TitleFieldInit();
	//�^�C�g���X�v���C�g
	TitleTexInit();
}

/*------------------------*/
/*-------�X�V����--------*/
/*-----------------------*/
#include"mHelper.h"

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerButton(Input::B))
	{
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);

		//�����ꂽ��
		menujudg_Play = true;
		//�����ꂽ��
		feedf = true;
	}
	if (Input::GetInstance()->TriggerButton(Input::A))
	{
		SceneManager::GetInstance()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}


	//��ʐ^�����ɂȂ�����V�[���؂�ւ�
	if (ChangeScene() == true)
	{
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL,sceneManager_);
	}

	//360��������O�Ƀ��Z�b�g
	if (Cangle >= 360.0f)
	{
		Cangle = 0.0f;
	}

	//�J�������t�B�[���h���S�ɉ��悤��
	if (!BackCam)
	{
		CameraPos.x = sinf(Cangle * (PI / 180.0f)) * 40.0f;
		CameraPos.y = cosf(Cangle * (PI / 180.0f)) * 40.0f;
	}
	else
	{
		//�J�����̂����W������
		CameraPos.y--;
	}

	//�X�v���C�g
	TitleTexUpda();
	//�t�B�[���h
	TitleFieldUpda();

	//�J�����X�V(��ňڂ�)
	camera->SetEye({CameraPos.x, 2.0f, CameraPos.y});
	camera->SetTarget({0.0f, 0.0f, 0.0f});
	camera->Update();
	SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);

	//	DebugTextSprite::GetInstance()->Print("aa", 0, 0, 2);
}

bool TitleScene::ChangeScene()
{
	if (feedf)
	{
		Cangle += 0.5f;
		float cameratocenter_x = sqrtf((CameraPos.x - 16.0f) * (CameraPos.x - 16.0f));
		if (cameratocenter_x < 1.0f && CameraPos.y < -30.0f)
		{
			BackCam = true;
		}
	}
	else
	{
		Cangle += 0.1f;
	}
	if (CameraPos.y <= -40.0f)
	{
		Feed::GetInstance()->Update_White(Feed::FEEDIN);
	}

	//��ʐ^�����ɂȂ�����V�[���؂�ւ�
	if (Feed::GetInstance()->GetAlpha() >= 1.0f)
	{
		return true;
	}
	return false;
}

/*------------------------*/
/*-------�`�揈��--------*/
/*-----------------------*/
void TitleScene::SpriteDraw()
{
	//���f���`��
	Object3d::PreDraw();
	celestal->Draw();
	field->Draw();
	Object3d::PostDraw();

	//�X�v���C�g�`��
	titlesprite->setcolor({1.0f, 1.0f, 1.0f, 0.5f});
	titlesprite2->setcolor({1.0f, 1.0f, 1.0f, 0.5f});

	Sprite::PreDraw();
	titlesprite2->Draw();
	titlesprite->Draw();
	TitleMenu[0]->Draw();
	DebugTextSprite::GetInstance()->DrawAll();
	Sprite::PostDraw();

	Feed::GetInstance()->Draw();
}

void TitleScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̕`��
	DirectXCommon::GetInstance()->BeginDraw(); //�`��R�}���h�̏��ւ��
	SpriteDraw();
	DirectXCommon::GetInstance()->EndDraw();
}


/*------------------------*/ //17,125
/*-------�������--------*/
/*-----------------------*/
void TitleScene::Finalize()
{
	field.release();
	celestal.release();
	//delete postEffect;
	delete lightGroup;
	delete camera;
	delete titlesprite, titlesprite2;
}

void TitleScene::TitleTexInit()
{
	titlesprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::TITLE1), {0.0f, 0.0f});

	titlesprite2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::TITLE2), {0, 0.0f});

	Sprite* navGameSprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMEPLAY), {0, 0.0f});
	Sprite* navEditSprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMEPLAY), {0, 0.0f});

	TitleMenu[0].reset(navGameSprite);

	TitleMenu[1].reset(navEditSprite);
	for (int i = 0; i < 2; i++)
	{
		menuAlpha[i] = 1.0f;
		MenuScale[i] = {1900, 1000};
		TitleMenu[i]->SetPosition({950, 500});
		TitleMenu[i]->SetSize({1900, 1000});
		TitleMenu[i]->SetAnchorPoint({0.5, 0.5});
	}
}

void TitleScene::TitleTexUpda()
{
	TitleMenu[0]->SetSize(MenuScale[0]);
	TitleMenu[1]->SetSize(MenuScale[1]);

	TitleMenu[0]->setcolor({1.0f, 1.0f, 1.0f, menuAlpha[0]});
	TitleMenu[1]->setcolor({1.0f, 1.0f, 1.0f, menuAlpha[1]});


	if (menujudg_Play)
	{
		MenuScale[0].x += 20.0f;
		MenuScale[0].y += 20.0f;
		menuAlpha[0] -= 0.02f;
		menuAlpha[1] = 0.0f;
	}
	if (menujudg_Edit)
	{
		MenuScale[1].x += 20.0f;
		MenuScale[1].y += 20.0f;
		menuAlpha[1] -= 0.02f;
		menuAlpha[0] = 0.0f;
	}
	titlesprite->SetSize({WinApp::window_width / 2, WinApp::window_height});
	titlesprite2->SetSize({WinApp::window_width, WinApp::window_height});
}

void TitleScene::TitleTexDraw()
{
}

void TitleScene::TitleFieldInit()
{
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	field = std::make_unique<Object3d>();
	field->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FIELD));
	field->Initialize(camera);
	celestal = std::make_unique<Object3d>();
	celestal->SetModel(ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE));
	celestal->Initialize(camera);
}

void TitleScene::TitleFieldUpda()
{
	//�t�B�[���h���邭���
	FieldRotY += 0.1f;

	//�p�����[�^���Z�b�g(�n�`)
	field->SetRotation({0.0f, 0.0f, 0.0f});
	field->SetScale({0.15f, 0.15f, 0.15f});
	field->SetFogCenter(FogPos);
	field->setFog(TRUE);

	//�p�����[�^���Z�b�g(�V��)
	celestal->SetRotation({0.0f, 0.0f, 0.0f});
	celestal->SetScale({30.f, 30.1f, 30.1f});

	//�X�V����
	field->Update({0.6f, 0.6f, 0.6f, 1.0f}, camera);
	celestal->Update({0.6f, 0.6f, 0.6f, 1.0f}, camera);
}
