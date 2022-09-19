#include "SistemConfig.h"
#include"ConfigSprite.h"
#include"HUDLayOut.h"
#include"CustomButton.h"
#include"SelectSword.h"
SistemConfig* SistemConfig::GetInstance()
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
	ConfigSprite::GetInstance()->Finalize();
}
void SistemConfig::Initialize()
{
	input = Input::GetInstance();
	configSprite = ConfigSprite::GetInstance();

	Sprite::LoadTexture(24, L"Resources/04 ‘I‘ðŽˆ/button_select2_3.png");

	configSprite->Initialize();
	//SelectSprite = std::make_unique<Sprite>();
	SelectSprite=Sprite::Create(24, configSprite->GetSpritePosition(m_number));
	SelectSprite->SetAnchorPoint({ 1.0f,1.0f });
	SelectSprite->SetSize({ 600.0f,100.0f });

	CustomButton::GetInstance()->Initialize();

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
			sAlpha = 0.0f;
			m_number++;
		}
		else if (input->TriggerCrossKey(input->Cross_Up)) {
			sAlpha = 0.0f;
			m_number--;
		}
		SelectSprite->SetPosition(configSprite->GetSpritePosition(m_number));


		if (CustomButton::GetInstance()->GetCustomButtonJudg() == false) {
			if (NowSelectButton() == HUDRAYOUT) {
				if (input->TriggerButton(input->Button_B)) {
					HUDLayOut::GetInstance()->SetLayOutMode(true);
				}
			}
		}
		if (NowSelectButton() == CUSTOMBUTTON) {
			//HUDLayOut::GetInstance()->SetLayOutMode(false);
			if (input->TriggerButton(input->Button_B)) {
				CustomButton::GetInstance()->SetCustomButtonJudg(true);
			}
		}
		if (CustomButton::GetInstance()->GetCustomButtonJudg() == false) {
			if (NowSelectButton() == SWORDSELECT) {
				//HUDLayOut::GetInstance()->SetLayOutMode(false);
				if (input->TriggerButton(input->Button_B)) {
					SelectSword::GetInstance()->SetSelectJudg(true);
				}
			}
		}

		if (count>5&&input->TriggerButton(input->Start)) {
			SelectSword::GetInstance()->SetSelectJudg(false);
			HUDLayOut::GetInstance()->SetLayOutMode(false);
			CustomButton::GetInstance()->SetCustomButtonJudg(false);
			count = 0;
			EndConfigJudg = true;
			m_ConfigFlag = false;
		}
	}

	CustomButton::GetInstance()->Update();

	sAlpha += 0.05f;
	sAlpha = min(sAlpha, 1.0f);
	sAlpha = max(sAlpha, 0.0f);
	m_number = min(m_number, 2.0f);
	m_number = max(m_number, 0.0f);
	SelectSprite->setcolor({ 1.0f,1.0f,1.0f,sAlpha });
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
	Sprite::PreDraw();
	if (m_ConfigFlag&&HUDLayOut::GetInstance()->GetLayOutMode()==false&&CustomButton::GetInstance()->GetCustomButtonJudg()==false&& SelectSword::GetInstance()->GetSelectJudg()==false) {
		configSprite->Draw();
		SelectSprite->Draw();
	}
	Sprite::PostDraw();
	CustomButton::GetInstance()->Draw();
}