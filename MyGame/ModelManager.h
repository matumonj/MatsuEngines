#pragma once
#include <string>
#include "Model.h"
#include <map>
#include <f_Model.h>

class ModelManager final {
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager& r) = default;
	ModelManager& operator= (const ModelManager& r) = default;

public:

	enum ModelName {
		Field
	};
	enum FBXName {
		Player,
		Enemy_Alpha,
		Enemy_Beta,
		Enemy_Camma,
		Boss_Enemy,
	};

public:
	static ModelManager* GetIns();		//�擾�p

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	void LoadFBXModel(const FBXName modelName, std::string fileName);
	Model* GetModel(ModelName modelName) { return model[modelName]; }
	f_Model* GetFBXModel(FBXName FBXName) { return fbxModel[FBXName]; }
private:
	static std::map<ModelName, Model*> model;	//���f���i�[�}�b�v
	static std::map<FBXName, f_Model*> fbxModel;
};