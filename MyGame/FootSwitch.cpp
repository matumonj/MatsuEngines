
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

	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		//モデル初期化
		FootSwitchObj[i] = std::make_unique<Object3d>();
		FootSwitchObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SWITCH));
		FootSwitchObj[i]->Initialize(camera);

		//各パラメータリセット
		SwitchClearJudg[i] = false;
		FootSwitchJudg[i] = false;
		FootSwitchEaseCount[i] = 0.f;
		FootSwitchColor[i] = { 1.f,1.f,1.f,1.f };

		//台座部分
		FootSwitchFrameObj[i] = std::make_unique<Object3d>();
		FootSwitchFrameObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SWITCH));
		FootSwitchFrameObj[i]->Initialize(camera);

		//踏んだエフェクト
		FootEffect[i] = std::make_unique<Particle>();
		FootEffect[i]->Init(Particle::BOM);
	}


}

void FootSwitch::Upda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//スイッチ座標をCSVから
	LoadCSV();

	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		//ふまれたらいろかえる
		FootEffectUpda();
		//引数部分長くなってしまってるので構造体にしたほうがいいかも
		if (SwitchClearJudg[i] == false) {
			FootSwitchColor[i] = SwitchChangeColor(FootSwitchPos[i], FootSwitchJudg[i], SwitchClearJudg[i], FootSwitchEaseCount[i]);
		}
		//パラメータのセット
		FootSwitchObj[i]->SetPosition(FootSwitchPos[i]);
		FootSwitchObj[i]->SetScale(FootSwitchScl);
		FootSwitchObj[i]->SetColor(FootSwitchColor[i]);

		FootSwitchObj[i]->Update(camera);

		//台座部分
		FootSwitchFrameObj[i]->SetPosition(FootSwitchPos[i]);
		FootSwitchFrameObj[i]->SetScale(FootSwitchFrameScl);
		FootSwitchFrameObj[i]->SetColor({1.f,0.5f,0.5f,1.f});

		FootSwitchFrameObj[i]->Update(camera);
	}

}

void FootSwitch::FootEffectUpda()
{
	for(auto i=0;i<FootEffect.size();i++)
	{
		if(!FootSwitchJudg[i])
		{
			if (FootJudg(FootSwitchPos[i]) == true)
			{
				FootEffect[i]->SetParF(0);
			}
		}
		FootEffect[i]->CreateParticle(!FootSwitchJudg[i]&& FootJudg(FootSwitchPos[i]) == true,FootSwitchPos[i]);

		FootEffect[i]->Upda_B(true);
	}

}

void FootSwitch::Draw()
{
	//描画
	Object3d::PreDraw();
	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		if (FootSwitchObj[i] == nullptr)continue;
		FootSwitchObj[i]->Draw();
		FootSwitchFrameObj[i]->Draw();
	}
	Object3d::PostDraw();
	//エフェクト
	for (auto i = 0; i < FootSwitchObj.size(); i++)
	{
		if (FootEffect[i] == nullptr)continue;
		FootEffect[i]->Draw();
	}
}

bool FootSwitch::FootJudg(XMFLOAT3 switchpos)
{
	bool ColSwitch = Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),
		switchpos) < 5.f;

	if (ColSwitch)
	{
		return true;
	} else {
		return false;
	}
}

FootSwitch::XMFLOAT4 FootSwitch::SwitchChangeColor(XMFLOAT3 switchpos, bool& judg,bool&clearJudg,float& ColoEaseCount)
{
	XMFLOAT4 color;
	
	if (FootJudg(switchpos) == true)
	{
		judg = true;
	}//もしふまれて色が完全緑なったらカウンタっ進める　
	if (ColoEaseCount >= 1.f)
	{
		ClearSwitchQuantity++;
		clearJudg = true;
	}

	if(judg)
	{
	//イージングカウンタ進める
	ColoEaseCount += 0.02f;

	//R,B減算
	color.x = Easing::EaseOut(ColoEaseCount, 1.f, 0.f);;
	color.z = Easing::EaseOut(ColoEaseCount, 1.f, 0.f);;
	color.y = 1.f;
	color.w = 1.f;
	
}else
	{
		color = { 1.f,1.f,1.f,1.f };
	}

	//0以下にはならない
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

	for (auto i = 0; i < FootSwitchObj.size(); i++) {
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

				FootSwitchPos[i] = { x, y, z };
				break;
			}
		}
	}
}

XMFLOAT3 FootSwitch::Switchs_CenterPos()
{
	//4地点の平均座標 x_y_z
	const float x = (FootSwitchPos[0].x + FootSwitchPos[1].x + FootSwitchPos[2].x + FootSwitchPos[3].x) / float(SwitchSize);
	const float y = (FootSwitchPos[0].y + FootSwitchPos[1].y + FootSwitchPos[2].y + FootSwitchPos[3].y) / float(SwitchSize);
	const float z = (FootSwitchPos[0].z + FootSwitchPos[1].z+ FootSwitchPos[2].z + FootSwitchPos[3].z) / float(SwitchSize);

	//Y座標補正値
	constexpr float CorrVal= 4.f;

	return { x,y+CorrVal,z };
}


