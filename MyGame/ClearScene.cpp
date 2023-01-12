#include "ClearScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Feed.h"
#include "ImageManager.h"

ClearScene::ClearScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------����������--------*/
/*-----------------------*/
void ClearScene::Initialize()
{
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);

	Feed::GetInstance()->initialize();
	
	//�^�C�g���X�v���C�g
	TitleTexInit();
	feedf = false;

}

/*------------------------*/
/*-------�X�V����--------*/
/*-----------------------*/
#include"mHelper.h"

void ClearScene::Update()
{
	if(!feedf)
	{
		if(Feed::GetInstance()->GetAlpha()>0.f)
		{
			Feed::GetInstance()->Update_White(Feed::FEEDOUT);
		}
	}
	if (Input::GetInstance()->TriggerButton(Input::B))
	{
		//SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);

		//�����ꂽ��
		menujudg_Play = true;
		//�����ꂽ��
		feedf = true;
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
	} else
	{
		//�J�����̂����W������
		CameraPos.y--;
	}

	//�X�v���C�g
	TitleTexUpda();
}

bool ClearScene::ChangeScene()
{
	if (feedf)
	{
		Cangle += 0.5f;
		float cameratocenter_x = sqrtf((CameraPos.x - 16.0f) * (CameraPos.x - 16.0f));
		if (cameratocenter_x < 1.0f && CameraPos.y < -30.0f)
		{
			BackCam = true;
		}
	} else
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
void ClearScene::SpriteDraw()
{

	//�X�v���C�g�`��
	titlesprite->setcolor({ 1.0f, 1.0f, 1.0f, 0.5f });

	Sprite::PreDraw();
	for (int i = 0; i < cleartex.size(); i++)
	{
		cleartex[i]->Draw();
	}
	titlesprite->Draw();
	Sprite::PostDraw();

	Feed::GetInstance()->Draw();
}

void ClearScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̕`��
	DirectXCommon::GetInstance()->BeginDraw(); //�`��R�}���h�̏��ւ��
	SpriteDraw();

	DirectXCommon::GetInstance()->EndDraw();
}


/*------------------------*/ //17,125
/*-------�������--------*/
/*-----------------------*/
void ClearScene::Finalize()
{
	//delete postEffect;
	delete lightGroup;
	//delete titlesprite;
}

void ClearScene::TitleTexInit()
{
	titlesprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::CLEARBACK), { 0.0f, 0.0f });

	std::array<Sprite*, 9>l_cleratex;
	l_cleratex[0] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_G), {0, 0.0f});
	l_cleratex[1] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_A) , { 0, 0.0f });
	l_cleratex[2] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_M), { 0, 0.0f });
	l_cleratex[3] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_E), { 0, 0.0f });
	l_cleratex[4] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_C), { 0, 0.0f });
	l_cleratex[5] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_L), { 0, 0.0f });
	l_cleratex[6] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_E), { 0, 0.0f });
	l_cleratex[7] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_A), { 0, 0.0f });
	l_cleratex[8] = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMECLEAR_R), { 0, 0.0f });


	for(int i=0;i<cleartex.size();i++)
	{
		cleartex[i].reset(l_cleratex[i]);
	}

	for (int i = 0; i < 9; i++)
	{
		texalpha[i] = 1.0f;
		MenuScale[i] = { 1900, 1000 };
		cleartex[i]->SetPosition(texpos[i]);
		cleartex[i]->SetSize({ 300, 300 });
		cleartex[i]->SetAnchorPoint({ 0.5, 0.5 });
	}
}

void ClearScene::TitleTexUpda()
{
	texangle[0] += 1.f;
	for(int i=0;i<cleartex.size();i++)
	{
		cleartex[i]->SetSize({ 300.f,300.f });
		
		if (i != 0&& texangle[i - 1] > 10.f) {
			texangle[i] += 1.f;
		}
		texpos[i].x = 200.f + float(i) * 200.f;
		texpos[i].y = 400.f + sinf(PI * 2.f / 120.f * texangle[i]) * 100.f;
		cleartex[i]->SetPosition(texpos[i]);
		//cleartex[i]->SetPosition({ texpos[i] });

		cleartex[i]->setcolor({ 1.f,1.f,1.f,1 });

	}
	if (menujudg_Play)
	{
		MenuScale[0].x += 20.0f;
		MenuScale[0].y += 20.0f;
		//menuAlpha[0] -= 0.02f;
		//menuAlpha[1] = 0.0f;
	}
	if (menujudg_Edit)
	{
		MenuScale[1].x += 20.0f;
		MenuScale[1].y += 20.0f;
		//menuAlpha[1] -= 0.02f;
		//menuAlpha[0] = 0.0f;
	}
	titlesprite->SetSize({ WinApp::window_width, WinApp::window_height });
}

void ClearScene::TitleTexDraw()
{
}
