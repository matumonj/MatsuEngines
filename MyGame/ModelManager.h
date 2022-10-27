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
		FIELD,
		MINI,
		BOSSFIELD,
		WOOD,
		CHEST,
		FENCE,
		NAIL,
		SPHERE,
		BACKGROUND,
		DAMAGEAREA,
		BOX,
		BEAM,
		PEDESTAL,
		SMALLSWORD,
		NORMALSWORD,
		BIGSWORD,
		CELESTIALSPHERE,
		ICECRYSTAL,
		BIGROCK_A,
		BIGROCK_B,
		GRASSFIELD
	};
	enum FBXName {
		PLAYER,
		GOLEM,
		GIGAGOLEM,
		Enemy_B,
		Enemy_C,
		BOSS,
	};

public:
	static ModelManager* GetIns();		//取得用

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	void LoadFBXModel(const FBXName modelName, std::string fileName);
	Model* GetModel(ModelName modelName) { return model[modelName]; }
	f_Model* GetFBXModel(FBXName FBXName) { return fbxModel[FBXName]; }
private:
	static std::map<ModelName, Model*> model;	//モデル格納マップ
	static std::map<FBXName, f_Model*> fbxModel;
};