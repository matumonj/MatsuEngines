#pragma once
#include<memory>
#include"CreateObjManager.h"
#include"Wood.h"
class PlaceWood :public CreateObjManager
{
public:
	void Initialize(DebugCamera* camera)override;

	void Update(DebugCamera* camera)override;

	void ImGui_Draw()override;

	void FileWriting()override;

	void ArgMent(DebugCamera* camera)override;

	void Draw()override;
private:
	std::vector<std::unique_ptr<Wood>>woods;
};
