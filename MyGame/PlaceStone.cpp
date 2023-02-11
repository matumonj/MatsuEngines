#include "PlaceStone.h"

void PlaceStone::Initialize(DebugCamera* camera)
{
	//Model = Model::CreateFromOBJ("wood");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGROCK_A));

	scl = {2, 3, 2};
}

void PlaceStone::FileWriting()
{
	file.open("stone.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/stone.csv"); // ファイルパスを指定する
	ofs << "Stone_Quantity" << "," << stones.size() << std::endl;

	for (int i = 0; i < stones.size(); i++)
	{
		ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POP" << "," << stones[i]->GetPosition().x
			<< "," << stones[i]->GetPosition().y
			<< "," << stones[i]->GetPosition().z << std::endl;
		ofs << "ROTATION" << "," << stones[i]->GetRotation().x
			<< "," << stones[i]->GetRotation().y
			<< "," << stones[i]->GetRotation().z << std::endl;
		ofs << "SCALE" << "," << stones[i]->GetScale().x
			<< "," << stones[i]->GetScale().y
			<< "," << stones[i]->GetScale().z << std::endl;
	}
}

bool PlaceStone::ErrorJudg()
{
	return false;
}

void PlaceStone::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag)
	{
		std::unique_ptr<Stone> newStone;
		if (ArgmentFlag)
		{
			newStone = std::make_unique<Stone_A>();
		}
		if (SubRockF)
		{
			newStone = std::make_unique<Stone_B>();
		}
		newStone->Initialize();
		newStone->SetPosition(pos);
		newStone->SetRotation(rot);
		newStone->SetScale(scl);
		stones.push_back(std::move(newStone));
		if (ArgmentFlag)
		{
			Number.push_back(1);
		}
		if (SubRockF)
		{
			Number.push_back(2);
		}
		ArgmentFlag = false;
		SubRockF = false;
	}
	for (std::unique_ptr<Stone>& wood : stones)
	{
		if (wood != nullptr)
		{
			wood->SetColors({1, 1, 1, 1});
			wood->Update();
			wood->CollisionField();
		}
	}
	if (DeleteFlag && stones.size() > 1)
	{
		stones.pop_back();
		DeleteFlag = false;
	}
}

void PlaceStone::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetRotation(rot);
	Obj->SetScale(scl);
	//	Obj->SetScale({ 2,3,2 });

	Obj->Update( camera);
}

void PlaceStone::Draw()
{
	for (std::unique_ptr<Stone>& stone : stones)
	{
		if (stone != nullptr)
		{
			//enemy->PreDraw();
			stone->Draw();
			//enemy->PostDraw();
		}
	}
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
}

#include"imgui.h"

void PlaceStone::ImGui_Draw()
{

}

void PlaceStone::Finalize()
{
	Destroy(Obj);
	for (int i = 0; i < stones.size(); i++)
	{
		Destroy_unique(stones[i]);
	}
	stones.clear();
}
