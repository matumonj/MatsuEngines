#include "SistemConfig.h"
#include"ConfigSprite.h"
#include"HUDLayOut.h"
#include"CustomButton.h"
SistemConfig* SistemConfig::GetInstance()
{
	static SistemConfig instance;

	return &instance;
}

SistemConfig::~SistemConfig()
{
	delete SelectSprite;
}
void SistemConfig::Initialize()
{
	input = Input::GetInstance();
	configSprite = ConfigSprite::GetInstance();

	Sprite::LoadTexture(124, L"Resources/04 ‘I‘ðŽˆ/button_select2_3.png");

	configSprite->Initialize();
	//SelectSprite = std::make_unique<Sprite>();
	SelectSprite=Sprite::Create(124, configSprite->GetSpritePosition(m_number));
	SelectSprite->SetAnchorPoint({ 1.0f,1.0f });
	SelectSprite->SetSize({ 600,100 });
}

void SistemConfig::Update()
{
	m_number = min(m_number, 2);
	m_number = max(m_number, 0);

	if (input->TriggerButton(input->Start)) {
		m_ConfigFlag = true;
	}

	if (m_ConfigFlag) {
		count++;
		if (input->TriggerCrossKey(input->Cross_Down)) {
			sAlpha = 0;
			m_number++;
		}
		else if (input->TriggerCrossKey(input->Cross_Up)) {
			sAlpha = 0;
			m_number--;
		}
		SelectSprite->SetPosition(configSprite->GetSpritePosition(m_number));



		if (NowSelectButton() == HUDRAYOUT) {
			if (input->TriggerButton(input->Button_A)) {
				HUDLayOut::GetInstance()->SetLayOutMode(true);
			}
		}
		if (NowSelectButton() == CUSTOMBUTTON) {
			HUDLayOut::GetInstance()->SetLayOutMode(false);
			if (input->TriggerButton(input->Button_A)) {
				CustomButton::GetInstance()->SetCustomButtonJudg(true);
			}
		}

		if (count>5&&input->TriggerButton(input->Start)) {
			
			HUDLayOut::GetInstance()->SetLayOutMode(false);
			CustomButton::GetInstance()->SetCustomButtonJudg(false);
			count = 0;
			m_ConfigFlag = false;
		}
	}

	sAlpha += 0.05;
	sAlpha = min(sAlpha, 1);
	sAlpha = max(sAlpha, 0);
	SelectSprite->setcolor({ 1,1,1,sAlpha });
	configSprite->Update();
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
	default:
		break;
	}
	return config;
}

void SistemConfig::Draw()
{
	Sprite::PreDraw();
	if (m_ConfigFlag&&HUDLayOut::GetInstance()->GetLayOutMode()==false&&CustomButton::GetInstance()->GetCustomButtonJudg()==false) {
		configSprite->Draw();
		SelectSprite->Draw();
	}
	Sprite::PostDraw();
}