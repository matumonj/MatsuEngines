
#include "FootSwitch.h"

#include <algorithm>

#include"Collision.h"
#include"PlayerControl.h"
#include"ModelManager.h"
#include"CameraControl.h"
#include"mHelper.h"
void FootSwitch::Init()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		FootSwitchObj[i] = std::make_unique<Object3d>();
		FootSwitchObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
		FootSwitchObj[i]->Initialize(camera);
	}


}

void FootSwitch::Upda()
{
	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		//�ӂ܂ꂽ�炢�납����
		if (!FootSwitchJudg[i]) {
			FootSwitchColor[i] = SwitchChangeColor(FootSwitchPos[i], FootSwitchJudg[i]);
		}

		//�p�����[�^�̃Z�b�g
		FootSwitchObj[i]->SetPosition(FootSwitchPos[i]);
		FootSwitchObj[i]->SetColor(FootSwitchColor[i]);
	}

}

void FootSwitch::Draw()
{
	//�`��
	Object3d::PreDraw();
	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		FootSwitchObj[i]->Draw();
	}
	Object3d::PostDraw();
}

bool FootSwitch::FootJudg(XMFLOAT3 switchpos)
{
	bool ColSwitch = Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),
		switchpos) < 5.f;

	ClearSwitchQuantity++;

	if (ColSwitch)
	{
		return true;
	}
	return false;
}

FootSwitch::XMFLOAT4 FootSwitch::SwitchChangeColor(XMFLOAT3 switchpos, bool& judg)
{
	XMFLOAT4 color;

	//���Z�l
	constexpr float l_ColorSubVal = 0.05f;

	if (FootJudg(switchpos))
	{
		//R,B���Z
		color.x -= l_ColorSubVal;
		color.z -= l_ColorSubVal;

		//�����ӂ܂�ĐF�����S�΂Ȃ�����J�E���^���i�߂�@
		if ((color.z || color.x) <= 0.f)
		{
			ClearSwitchQuantity++;
			judg = true;
		}
	} else
	{
		color = { 1.f,1.f,1.f,1.f };
	}

	//0�ȉ��ɂ͂Ȃ�Ȃ�
	color.x = std::clamp(color.x, 0.f, 1.f);
	color.z = std::clamp(color.z, 0.f, 1.f);

	return color;
}




