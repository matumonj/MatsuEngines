#include "SistemConfig.h"
#include"ConfigSprite.h"
#include"CustomButton.h"
#include"SelectSword.h"
#include"PlayerControl.h"

SistemConfig* SistemConfig::GetIns()
{
	static SistemConfig instance;

	return &instance;
}

SistemConfig::~SistemConfig()
{
	delete SelectSprite;
	//delete configSprite;
}

void SistemConfig::Finalize()
{
	delete SelectSprite;
	//delete configSprite;
	ConfigSprite::GetIns()->Finalize();
}

void SistemConfig::Initialize()
{
	input = Input::GetIns();
	configSprite = ConfigSprite::GetIns();

	//Sprite::LoadTexture(24, L"Resources/04 ‘I‘ðŽˆ/button_select2_3.png");

	configSprite->Initialize();
	//SelectSprite = std::make_unique<Sprite>();
	//SelectSprite = Sprite::Create(24, configSprite->GetSpritePosition(m_number));
	//SelectSprite->SetAnchorPoint({1.0f, 1.0f});
	//SelectSprite->SetSize({600.0f, 100.0f});

	CustomButton::GetIns()->Initialize();
}

#include"CameraControl.h"

void SistemConfig::Update()
{
	m_number = min(m_number, 2);
	m_number = max(m_number, 0);

	if (CameraControl::GetIns()->GetCameraState() == CameraControl::PLAYER)
	{
		if (input->TriggerButton(input->START))
		{
			//	PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);
			//m_ConfigFlag = true;
		}
	}

	if (m_ConfigFlag)
	{
		count++;
		if (input->TiltStick(input->L_DOWN))
		{
			sAlpha = 0.0f;
			m_number++;
		}
		else if (input->TiltStick(input->L_UP))
		{
			sAlpha = 0.0f;
			m_number--;
		}
		SelectSprite->SetPosition(configSprite->GetSpritePosition(m_number));


		if (CustomButton::GetIns()->GetCustomButtonJudg() == false)
		{
		}
		if (NowSelectButton() == CUSTOMBUTTON)
		{
			if (input->TriggerButton(input->B))
			{
				CustomButton::GetIns()->SetCustomButtonJudg(true);
			}
		}
		if (CustomButton::GetIns()->GetCustomButtonJudg() == false)
		{
			if (NowSelectButton() == SWORDSELECT)
			{
			}
		}

		if (count > 5 && input->TriggerButton(input->START))
		{
			CustomButton::GetIns()->SetCustomButtonJudg(false);

			PlayerControl::GetIns()->GetPlayer()->SetStopFlag(false);
			count = 0;
			EndConfigJudg = true;
			m_ConfigFlag = false;
		}
	}
	CustomButton::GetIns()->Update();
	sAlpha += 0.05f;
	sAlpha = min(sAlpha, 1.0f);
	sAlpha = max(sAlpha, 0.0f);
	m_number = min(m_number, 2);
	m_number = max(m_number, 0);
	//SelectSprite->setcolor({1.0f, 1.0f, 1.0f, sAlpha});
	//configSprite->Update();
}

SistemConfig::Config SistemConfig::NowSelectButton()
{
	switch (m_number)
	{
	case 0:
		config = HUDRAYOUT;
		break;
	case 1:
		config = CUSTOMBUTTON;
		break;
	case 2:
		config = SWORDSELECT;
		break;
	default:
		break;
	}
	return config;
}

void SistemConfig::Draw()
{
	/*Sprite::PreDraw();
	if (m_ConfigFlag && CustomButton::GetIns()->GetCustomButtonJudg() == false && SelectSword::GetIns()->
		GetSelectJudg() == false)
	{
		configSprite->Draw();
		SelectSprite->Draw();
	}
	Sprite::PostDraw();
	CustomButton::GetIns()->Draw();
	SelectSword::GetIns()->Draw();*/
}
