#pragma once
#include<memory>
#include"CreateObjManager.h"
#include"GrassField.h"
#include"DebugCamera.h"

class PlaceGrassField : public CreateObjManager
{
public:
	void Initialize(DebugCamera* camera) override;

	void Update(DebugCamera* camera) override;

	void ImGui_Draw() override;

	void FileWriting() override;

	void ArgMent(DebugCamera* camera) override;

	void Draw() override;

	bool ErrorJudg() override;

	void Finalize() override;
private:
	int indextype;
	bool SubRockF;
	std::vector<std::unique_ptr<GrassField>> grassfields;
};
