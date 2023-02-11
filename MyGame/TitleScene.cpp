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

	Feed::GetIns()->initialize();

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
	if (Input::GetIns()->TriggerButton(Input::B))
	{
		CameraBackF = true;
		menujudg_Play = true;
		feedf = true;
	}
	if (Input::GetIns()->TriggerButton(Input::A))
	{
		SceneManager::GetIns()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}


	//��ʐ^�����ɂȂ�����V�[���؂�ւ�
	//
	if (ChangeScene() == true)
	{
		SceneManager::GetIns()->SetScene(SceneManager::TUTORIAL, sceneManager_);
	}

	//360��������O�Ƀ��Z�b�g
	if (Cangle >= PI_360)
	{
		Cangle = 0.0f;
	}

	//�J�������t�B�[���h���S�ɉ��悤��
	if (!CameraBackF)
	{
		CameraPos.x = sinf(Cangle * (PI / PI_180)) * 40.0f;
		CameraPos.y = cosf(Cangle * (PI / PI_180)) * 40.0f;
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
}


void TitleScene::MyGameDraw()
{
	//���f���`��
	Object3d::PreDraw();
	celestal->Draw();
	field->Draw();
	Object3d::PostDraw();
}

bool TitleScene::ChangeScene()
{
	bool FadeFlag = CameraPos.y <= -40.0f;
	float CameraCenterPosX = CameraPos.x - 26.0f;
	if (feedf)
	{
		CamAngleSpeed = 0.5f;

		float cameratocenter_x = sqrtf(CameraCenterPosX * CameraCenterPosX);

		if (cameratocenter_x < 1.0f && CameraPos.y < -30.0f)
		{
			CameraBackF = true;
		}
	}
	else
	{
		CamAngleSpeed = 0.1f;
	}

	//��]�p�A���O���̒l�𑫂��Ă���
	Cangle += CamAngleSpeed;

	//�V�[���J�ڎ�
	if (FadeFlag)
	{
		Feed::GetIns()->Update_White(Feed::FEEDIN);

		//��ʐ^�����ɂȂ�����V�[���؂�ւ�
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			return true;
		}
	}

	return false;
}

/*------------------------*/
/*-------�`�揈��--------*/
/*-----------------------*/
void TitleScene::SpriteDraw()
{
	//�X�v���C�g�`��
	titlesprite->setcolor({1.0f, 1.0f, 1.0f, 0.5f});

	Sprite::PreDraw();
	titlesprite->Draw();
	TitleMenu->Draw();
	DebugTextSprite::GetIns()->DrawAll();
	Sprite::PostDraw();

	Feed::GetIns()->Draw();
}

void TitleScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̕`��
	DirectXCommon::GetIns()->BeginDraw(); //�`��R�}���h�̏��ւ��
	MyGameDraw();
	SpriteDraw();
	DirectXCommon::GetIns()->EndDraw();
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
	Destroy_unique(camera);
	Destroy_unique(titlesprite);
	//Destroy_unique(titlesprite);
}

void TitleScene::TitleTexInit()
{
	titlesprite.reset(Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::TITLE1), {0.0f, 0.0f}));

	Sprite* navGameSprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMEPLAY), {0, 0.0f});
	Sprite* navEditSprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMEPLAY), {0, 0.0f});

	TitleMenu.reset(navGameSprite);

	menuAlpha = 1.0f;

	MenuScale = {1900.f, 1000.f};
	MenuPos = {950.f, 500.f};

	TitleMenu->SetPosition(MenuPos);
	TitleMenu->SetAnchorPoint({0.5f, 0.5f});
}

void TitleScene::TitleTexUpda()
{
	TitleMenu->SetSize(MenuScale);
	TitleMenu->setcolor({1.0f, 1.0f, 1.0f, menuAlpha});

	constexpr float l_MenuSclingSpeed = 20.f;
	constexpr float l_MenuSubAlphaSpeed = 0.02f;

	if (menujudg_Play)
	{
		MenuScale.x += l_MenuSclingSpeed;
		MenuScale.y += l_MenuSclingSpeed;
		menuAlpha -= l_MenuSubAlphaSpeed;
	}
	titlesprite->SetSize({WinApp::window_width / 2, WinApp::window_height});
}

void TitleScene::TitleTexDraw()
{
}

void TitleScene::TitleFieldInit()
{
	camera = std::make_unique<DebugCamera>(WinApp::window_width, WinApp::window_height);

	//�n�`
	field = std::make_unique<Object3d>();
	field->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FIELD));
	field->Initialize(camera.get());
	//�V��
	celestal = std::make_unique<Object3d>();
	celestal->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SKY));
	celestal->Initialize(camera.get());
}

void TitleScene::TitleFieldUpda()
{
	constexpr XMFLOAT3 FieldScl = {0.15f, 0.15f, 0.15f};
	constexpr XMFLOAT3 CelestalScl = {30.f, 30.1f, 30.1f};

	constexpr float addRotYSpeed = 0.1f;

	//�t�B�[���h���邭���
	FieldRotY += addRotYSpeed;

	//�p�����[�^���Z�b�g(�n�`)
	field->SetRotation({0.0f, FieldRotY, 0.0f});
	//�p�����[�^���Z�b�g(�V��)
	celestal->SetRotation({0.0f, 0.0f, 0.0f});
	celestal->SetScale(CelestalScl);

	//�X�V����
	field->setFog(true);
	celestal->Setf(true);
	celestal->setFog(true);
	celestal->SetFogCenter(camera->GetEye());

	field->Update(camera.get());
	celestal->Update(camera.get());
}

void TitleScene::LightUpdate()
{
}
