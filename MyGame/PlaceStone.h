#pragma once
#include<memory>
#include"CreateObjManager.h"
#include"Stone.h"
#include"DebugCamera.h"

class PlaceStone : public CreateObjManager
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
	std::vector<std::unique_ptr<Stone>> stones;
};
