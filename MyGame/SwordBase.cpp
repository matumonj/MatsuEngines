#include "SwordBase.h"

#include "CameraControl.h"


void SwordBase::LoadCsv(std::string damval, std::string knoval, std::string speval, std::string modname)
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	file.open("Param_CSV/CharaParam/Player_Param.csv");

	popcom << file.rdbuf();

	file.close();
	//ダメージの追加乱数
	int CorrDamageVal = rand() % 8 - 1;

	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find(damval) == 0)
		{
			std::getline(line_stream, word, ',');
			int l_Damage = static_cast<int>(std::atof(word.c_str()));
			Damage = l_Damage+CorrDamageVal;
		}
		if (word.find(knoval) == 0)
		{
			std::getline(line_stream, word, ',');
			float l_KnockPower = static_cast<float>(std::atof(word.c_str()));
			KnockPower = l_KnockPower;
		}
		if (word.find(speval) == 0)
		{
			std::getline(line_stream, word, ',');
			float l_Speed = static_cast<float>(std::atof(word.c_str()));
			AnimationTime = l_Speed;
		}
		if (word.find(modname) == 0)
		{
			std::getline(line_stream, word, ',');
			auto l_ModelName = word.c_str();
			modelname = l_ModelName;
			break;
		}
	}

	//モデル割り当て
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	if (modelname == "Wand")
	{
		m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SMALLSWORD));
	}
	if (modelname == "sword")
	{
		m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NORMALSWORD));
	}
	if (modelname == "axe")
	{
		m_Object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	}
	//	swordModel.reset(Model::CreateFromOBJ(modelname));
}

SwordBase::SwordBase()
{
}

SwordBase::~SwordBase()
{
}

void SwordBase::Initialize()
{
}

void SwordBase::Update()
{
}

void SwordBase::Draw()
{
}
