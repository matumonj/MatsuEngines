#include "HUDLayOut.h"
#include"HUD.h"
#include"Collision.h"
HUDLayOut* HUDLayOut::GetInstance()
{
	static HUDLayOut instance;

	return &instance;
}
void HUDLayOut::ChangePosition_PlayerHP()
{
	
}

void HUDLayOut::Update()
{
	//�}�E�X���W�̎擾
	GetCursorPos(&p);
	ScreenToClient(FindWindowA("DirectX", nullptr), &p);
	//LONG->float
	x = p.x;
	y = p.y;

	NowOperation();

	if (LayOutMode) {
		if (operation != ENEMYHP&&operation !=Skill) {//�v���C���[�ȊO��HUD���������ĂȂ��Ƃ�����HUD��������悤��
			HUD::GetInstance()->GetPlayerHP()->ChangePosition_PlayerHP({ x,y });
		}
			if (operation!=PLAYERHP&& operation !=Skill) {
				//HUD::GetInstance()->GetEnemyHP()->ChangePosition_PlayerHP({ x,y });
			}
			if (operation != PLAYERHP&& operation != ENEMYHP) {
				HUD::GetInstance()->ChangePosition({ x,y }, center);
			}
			//���C�A�E�g���[�h��ON�ɂ������A�u����OFF�ɂȂ�Ȃ��悤��
			count++;
		
		if (count>10&&Input::GetInstance()->TriggerKey(DIK_G)) {
			LayOutMode = false;
			count = 0;
		}
	}
	HUD::GetInstance()->SetCenterPosition(center);

}

void HUDLayOut::NowOperation()
{
	////HUD���m���d�Ȃ�̂�h���@���Ȃ艘�����@�Ȃ̂łł����璼�������A�A
	if (HUD::GetInstance()->GetPlayerHP()->GetHUDLayOutFlag() == true) {
		operation = PLAYERHP;
	}
	
	if (HUD::GetInstance()->GetSkillButton()->GetHUDLayOutFlag() == true) {
		operation = Skill;
	}
	if (HUD::GetInstance()->GetPlayerHP()->GetHUDLayOutFlag() == false &&
		//HUD::GetInstance()->GetEnemyHP()->GetHUDLayOutFlag() == false&&
		HUD::GetInstance()->GetSkillButton()->GetHUDLayOutFlag() == false) {
		operation = None;
	}
}