#pragma once

#include "CreateObjManager.h"

class PlaceCameraPoints : public CreateObjManager
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
	std::vector<std::unique_ptr<Object3d>> points;
};
