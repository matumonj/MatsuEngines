#include "SistemConfig.h"
#include"ConfigSprite.h"
#include"HUDLayOut.h"
SistemConfig* SistemConfig::GetInstance()
{
	static SistemConfig instance;

	return &instance;
}
void SistemConfig::Initialize()
{
	input = Input::GetInstance();
	configSprite = ConfigSprite::GetInstance();

	configSprite->Initialize();
	//SelectSprite = std::make_unique<Sprite>();
	SelectSprite=Sprite::Create(123, configSprite->GetSpritePosition(m_number));
	SelectSprite->SetSize({ 120,120 });
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
			m_number++;
		}
		else if (input->TriggerCrossKey(input->Cross_Up)) {
			m_number--;
		}
		SelectSprite->SetPosition(configSprite->GetSpritePosition(m_number));

		if (count>5&&input->TriggerButton(input->Start)) {
			count = 0;
			m_ConfigFlag = false;
			HUDLayOut::GetInstance()->SetLayOutMode(false);
			
		}
	}


	if (NowSelectButton() == HUDRAYOUT) {
		if (input->TriggerButton(input->Button_A)) {
			HUDLayOut::GetInstance()->SetLayOutMode(true);
		}
	}
	if(HUDLayOut::GetInstance()->GetLayOutMode()==true){
		if (input->TriggerButton(input->Button_B)) {
			HUDLayOut::GetInstance()->SetLayOutMode(false);
		}
	}
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
		config = ENEMYHP;
		break;
	default:
		break;
	}
	return config;
}

void SistemConfig::Draw()
{
	Sprite::PreDraw();
	if (m_ConfigFlag&&HUDLayOut::GetInstance()->GetLayOutMode()==false) {
		configSprite->Draw();
		SelectSprite->Draw();
	}
	Sprite::PostDraw();
}