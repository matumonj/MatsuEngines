#include "TitleScene.h"

#include <algorithm>

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
	
	LoadWords.emplace("��", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_1),{ 0, 0.0f }));
	LoadWords.emplace("�\", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_2), { 0, 0.0f }));
	LoadWords.emplace("�[", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_3), { 0, 0.0f }));
	LoadWords.emplace("�X", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_4), { 0, 0.0f }));
	LoadWords.emplace("��", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_5), { 0, 0.0f }));
	LoadWords.emplace("��_1", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_6), { 0, 0.0f }));
	LoadWords.emplace("��", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_7), { 0, 0.0f }));
	LoadWords.emplace("��_2", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_6), { 0, 0.0f }));
	LoadWords.emplace("��", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_8), { 0, 0.0f }));
	LoadWords.emplace("��", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_9), { 0, 0.0f }));
	LoadWords.emplace("��", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_10), { 0, 0.0f }));


	WordsSize = { 220.f };

	postEffect = new PostEffect();
	postEffect->Initialize();
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
		SceneManager::GetIns()->SetLoad(true);
		CameraBackF = true;
		menujudg_Play = true;
	}
	if (Input::GetIns()->TriggerButton(Input::A))
	{
		SceneManager::GetIns()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}

	
	//��ʐ^�����ɂȂ�����V�[���؂�ւ�
	//
	if (CameraBackF)
	{
		timef++;
		SceneManager::GetIns()->LoadScene();
	}
	if (ChangeScene() == true)
	{
		SceneManager::GetIns()->SetScene(SceneManager::TUTORIAL, sceneManager_);
	}
	if(!FadeFlag)
	{
		if (feedf)
		{
			postEffect->SetSepiaF(true);
			Feed::GetIns()->Update_Black(Feed::FEEDOUT);
		} else
		{
			if (menujudg_Play) {
				Feed::GetIns()->Update_Black(Feed::FEEDIN);
				if (Feed::GetIns()->GetAlpha() >= 1.0f)
				{
					feedf = true;
				}
			}
		}
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
	LoadWordsSetParam();

	TitleTexUpda();
	//�t�B�[���h
	TitleFieldUpda();
	//�J�����X�V(��ňڂ�)
	LoadMenuSprite->SetSize({ 1900.f,1000.f });
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
	
	FadeFlag = timef&&!SceneManager::GetIns()->GetLoad();
	float CameraCenterPosX = CameraPos.x - 26.0f;
		CamAngleSpeed = 0.1f;
	

	//��]�p�A���O���̒l�𑫂��Ă���
	Cangle += CamAngleSpeed;

	//�V�[���J�ڎ�
	if (LoadWordsChange&&timef>=600.f)
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

	

	if (feedf) {
		LoadMenuSprite->Draw();
		//1�������ǂݍ���
		for (auto i = LoadWords.begin(); i != LoadWords.end(); i++) {
			if (LoadWords[i->first] == nullptr)continue;
			LoadWords[i->first].get()->Draw();
		}
	//	DebugTextSprite::GetIns()->DrawAll();
	}
	
	Sprite::PostDraw();

	Feed::GetIns()->Draw();
}

void TitleScene::Draw()
{
	
	//�|�X�g�G�t�F�N�g�̕`��
	DirectXCommon::GetIns()->BeginDraw(); //�`��R�}���h�̏��ւ��
	//postEffect->Draw();
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
	LoadWords["��"].reset();
	LoadWords["��_1"].reset();
	LoadWords["��"].reset();
	LoadWords["��_2"].reset();
	LoadWords["��"].reset();
	LoadWords["��"].reset();

	delete postEffect;
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
	Sprite* loadSprite= Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOAD), { 0, 0.0f });
	TitleMenu.reset(navGameSprite);
	LoadMenuSprite.reset(loadSprite);
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
	celestal->SetColor({ 1.f,1.f,1.f,1.f });
	//�X�V����
	field->setFog(true);
	celestal->SetFogCenter(camera->GetEye());

	field->Update(camera.get());
	celestal->Update(camera.get());
}

void TitleScene::LightUpdate()
{
}


void TitleScene::LoadWordsSetParam()
{
	//�㉺�^���̒��S
	constexpr XMFLOAT2 CenterPos = {200.f,800.f};
	//�����̊Ԋu
	constexpr float WordsInter = 100.f;

	constexpr float AddMovingVal = 2.2f;

	//�ŏ��̕������������Ɠ�����
	LoadWordsPosAngle[0] += 1.f;
	

	if(!LoadWordsChange){
		for (int i = 0; i < LoadWordsPosAngle.size(); i++)
		{
			if (i != 0 && LoadWordsPosAngle[i - 1] > 10.f)
			{
				LoadWordsPosAngle[i] += 1.f;
			}
			LoadWordsPos[i].x = CenterPos.x + static_cast<float>(i) * WordsInter;
			LoadWordsPos[i].y = CenterPos.y + sinf(PI * 2.f / 120.f * LoadWordsPosAngle[i]) * WordsInter / 2.f;
		}
	}
	else
	{
		for (int i = 0; i < LoadWordsPosAngle.size(); i++)
		{
			LoadWordsPos[i].x = CenterPos.x + static_cast<float>(i) * WordsInter;
			LoadWordsPos[i].y = CenterPos.y;
		}
	}
	//�����̃T�C�Y�ݒ�
	for (auto i = LoadWords.begin(); i != LoadWords.end(); i++) {
		if (LoadWords[i->first] == nullptr)continue;
		LoadWords[i->first].get()->SetAnchorPoint({ 0.5f,0.5f });
		LoadWords[i->first].get()->SetSize({ WordsSize,WordsSize });
	}


	//�킩��₷�����邽�߂ɌX�ŏ����Ă�
	if (LoadWordsChange) {
		LoadWords["��"].get()->SetPosition(LoadWordsPos[0]);
		LoadWords["��_1"].get()->SetPosition(LoadWordsPos[1]);
		LoadWords["��"].get()->SetPosition(LoadWordsPos[2]);
		LoadWords["��_2"].get()->SetPosition(LoadWordsPos[3]);
		LoadWords["��"].get()->SetPosition(LoadWordsPos[4]);
		LoadWords["��"].get()->SetPosition(LoadWordsPos[5]);

		//�v��Ȃ��Ȃ����e�N�X�`���͔j��
		LoadWords["��"].reset(nullptr);
		LoadWords["�\"].reset(nullptr);
		LoadWords["�["].reset(nullptr);
		LoadWords["�X"].reset(nullptr);
		LoadWords["��"].reset(nullptr);

		WordsSize += AddMovingVal*2.f;
	}
	else {
		LoadWords["��"] .get()->SetPosition(LoadWordsPos[0]);
		LoadWords["�\"].get()->SetPosition(LoadWordsPos[1]);
		LoadWords["�["].get()->SetPosition(LoadWordsPos[2]);
		LoadWords["�X"].get()->SetPosition(LoadWordsPos[3]);
		LoadWords["��"].get()->SetPosition(LoadWordsPos[4]);
		LoadWords["��_1"].get()->SetPosition(LoadWordsPos[5]);
		LoadWords["��"].get()->SetPosition(LoadWordsPos[6]);
		LoadWords["��_2"].get()->SetPosition(LoadWordsPos[7]);
		LoadWords["��"].get()->SetPosition(LoadWordsPos[8]);

		//�����B��
		LoadWords["��"].get()->SetSize({0.f,0.f});
		LoadWords["��"].get()->SetSize({0.f,0.f});

		if(FadeFlag)
		{
			WordsSize -= AddMovingVal;
		}

		LoadWordsChange = WordsSize <= 0.f;
	}


	WordsSize = std::clamp(WordsSize, 0.f, 200.f);
}