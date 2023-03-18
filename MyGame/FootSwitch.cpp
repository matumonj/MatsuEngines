#include "FootSwitch.h"

#include <algorithm>

#include"Collision.h"
#include"PlayerControl.h"
#include"ModelManager.h"
#include"CameraControl.h"
#include"mHelper.h"

FootSwitch* FootSwitch::GetIns()
{
	static FootSwitch ins;
	return &ins;
}

void FootSwitch::Init()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	for (auto i = 0; i < switch_param_.size(); i++)
	{
		//���f��������
		switch_param_[i].Obj = std::make_unique<Object3d>();
		switch_param_[i].Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SWITCH));
		switch_param_[i].Obj->Initialize(camera);

		//�e�p�����[�^���Z�b�g
		switch_param_[i].SwitchClearJudg = false;
		switch_param_[i].FootSwitchJudg = false;
		switch_param_[i].EaseCount = 0.f;
		switch_param_[i].Color = {1.f, 1.f, 1.f, 1.f};

		//�������
		switch_param_[i].FrameObj = std::make_unique<Object3d>();
		switch_param_[i].FrameObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SWITCH));
		switch_param_[i].FrameObj->Initialize(camera);

		//���ݏI�������̃G�t�F�N�g�p�~�`���f��
		for (auto j = 0; j < CEffectSize; j++)
		{
			switch_param_[i].CEffectObj[j] = std::make_unique<Object3d>();
			switch_param_[i].CEffectObj[j]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SWITCH));
			switch_param_[i].CEffectObj[j]->Initialize(camera);
		}
	}
}

void FootSwitch::Upda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//�X�C�b�`���W��CSV����
	LoadCSV();

	for (auto i = 0; i < switch_param_.size(); i++)
	{
		//�ӂ܂ꂽ�炢�납����
		FootEffectUpda();
		//�������������Ȃ��Ă��܂��Ă�̂ō\���̂ɂ����ق�����������
		if (switch_param_[i].SwitchClearJudg == false)
		{
			switch_param_[i].Color = SwitchChangeColor(switch_param_[i].Pos, switch_param_[i].FootSwitchJudg,
			                                           switch_param_[i].SwitchClearJudg, switch_param_[i].EaseCount);
		}
		//�p�����[�^�̃Z�b�g
		switch_param_[i].Obj->SetPosition(switch_param_[i].Pos);
		switch_param_[i].Obj->SetScale(FootSwitchScl);
		switch_param_[i].Obj->SetColor(switch_param_[i].Color);

		switch_param_[i].Obj->Update(camera);

		//�������
		switch_param_[i].FrameObj->SetPosition(switch_param_[i].Pos);
		switch_param_[i].FrameObj->SetScale(FootSwitchFrameScl);
		switch_param_[i].FrameObj->SetColor({1.f, 0.5f, 0.5f, 1.f});

		switch_param_[i].FrameObj->Update(camera);

		//�G�t�F�N�g�p���f��
		for (auto j = 0; j < CEffectSize; j++)
		{
			switch_param_[i].CEffectObj[j]->SetPosition(switch_param_[i].CEffectPos[j]);
			switch_param_[i].CEffectObj[j]->SetScale(FootSwitchFrameScl);
			switch_param_[i].CEffectObj[j]->SetColor({0.2f, 0.9f, 0.2f, switch_param_[i].CEffectAlpha[j]});
			switch_param_[i].CEffectObj[j]->Update(camera);
		}
	}
}

void FootSwitch::FootEffectUpda()
{
	//�G�t�F�N�g�̍������
	std::array<float, SwitchSize> l_PosYMax;

	//�I�u�W�FY���W�㏸�l
	constexpr float l_MovingYVal = 0.2f;

	for (auto i = 0; i < switch_param_.size(); i++)
	{
		//�G�t�F�N�g��֏オ�����l
		l_PosYMax[i] = switch_param_[i].Pos.y + 15.f;

		//�ӂ܂ꂽ��G�t�F�N�g����
		if (FootJudg(switch_param_[i].Pos) == true)
		{
			switch_param_[i].CeffectCreateF[0] = true;
		}

		for (auto j = 0; j < CEffectSize; j++)
		{
			//���f���̍��W�����
			switch_param_[i].CEffectPos[j].x = switch_param_[i].Pos.x;
			switch_param_[i].CEffectPos[j].z = switch_param_[i].Pos.z;
			switch_param_[i].CEffectAlpha[j] -= 0.01f;
			//�O�̗v�f��Y���W�����l��������t���OON
			if (j != 0 && switch_param_[i].CEffectPos[j - 1].y > switch_param_[i].Pos.y + 5.f)
			{
				switch_param_[i].CeffectCreateF[j] = true;
			}

			//�t���O�I���̂Ƃ��͉~���f������֓�����
			if (switch_param_[i].CeffectCreateF[j])
			{
				switch_param_[i].CEffectPos[j].y += l_MovingYVal;
			}
			//����l�܂œ��B������
			if (switch_param_[i].CEffectPos[j].y > l_PosYMax[i])
			{
				switch_param_[i].CEffectAlpha[j] = 0.7f;
				//���̈ʒu�ɂ��ǂ�
				switch_param_[i].CEffectPos[j].y = switch_param_[i].Pos.y;
			}
			switch_param_[i].CEffectAlpha[j] = std::clamp(switch_param_[i].CEffectAlpha[j], 0.f, 1.f);
		}
	}
}

void FootSwitch::Draw()
{
	//�`��
	Object3d::PreDraw();
	for (auto i = 0; i < switch_param_.size(); i++)
	{
		if (switch_param_[i].Obj == nullptr)
		{
			continue;
		}
		switch_param_[i].Obj->Draw();
		switch_param_[i].FrameObj->Draw();
		for (auto j = 0; j < CEffectSize; j++)
		{
			if (switch_param_[i].CEffectAlpha[j] <= 0.f)
			{
				continue;
			}
			switch_param_[i].CEffectObj[j]->Draw();
		}
	}
	Object3d::PostDraw();
}

bool FootSwitch::FootJudg(XMFLOAT3 switchpos)
{
	bool ColSwitch = Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),
	                                      switchpos) < 5.f;

	if (ColSwitch)
	{
		return true;
	}
	return false;
}

FootSwitch::XMFLOAT4 FootSwitch::SwitchChangeColor(XMFLOAT3 switchpos, bool& judg, bool& clearJudg,
                                                   float& ColoEaseCount)
{
	XMFLOAT4 color;

	if (FootJudg(switchpos) == true)
	{
		judg = true;
	} //�����ӂ܂�ĐF�����S�΂Ȃ�����J�E���^���i�߂�@
	if (ColoEaseCount >= 1.f)
	{
		ClearSwitchQuantity++;
		clearJudg = true;
	}

	if (judg)
	{
		//�C�[�W���O�J�E���^�i�߂�
		ColoEaseCount += 0.02f;

		//R,B���Z
		color.x = Easing::EaseOut(ColoEaseCount, 1.f, 0.f);
		color.z = Easing::EaseOut(ColoEaseCount, 1.f, 0.f);
		color.y = 1.f;
		color.w = 1.f;
	}
	else
	{
		color = {1.f, 1.f, 1.f, 1.f};
	}

	//0�ȉ��ɂ͂Ȃ�Ȃ�
	color.x = std::clamp(color.x, 0.f, 1.f);
	color.z = std::clamp(color.z, 0.f, 1.f);
	ColoEaseCount = std::clamp(ColoEaseCount, 0.f, 1.f);

	return color;
}

void FootSwitch::LoadCSV()
{
	file.open("Param_CSV/Gimmic.csv");

	popcom << file.rdbuf();

	file.close();

	for (auto i = 0; i < switch_param_.size(); i++)
	{
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("POSITION") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				switch_param_[i].Pos = {x, y, z};
				break;
			}
		}
	}
}

XMFLOAT3 FootSwitch::Switchs_CenterPos()
{
	//4�n�_�̕��ύ��W x_y_z
	const float x = (switch_param_[0].Pos.x + switch_param_[1].Pos.x + switch_param_[2].Pos.x + switch_param_[3].Pos.x)
		/ static_cast<float>(SwitchSize);
	const float y = (switch_param_[0].Pos.y + switch_param_[1].Pos.y + switch_param_[2].Pos.y + switch_param_[3].Pos.y)
		/ static_cast<float>(SwitchSize);
	const float z = (switch_param_[0].Pos.z + switch_param_[1].Pos.z + switch_param_[2].Pos.z + switch_param_[3].Pos.z)
		/ static_cast<float>(SwitchSize);

	//Y���W�␳�l
	constexpr float CorrVal = 4.f;

	return {x, y + CorrVal, z};
}
