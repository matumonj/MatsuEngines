#include "ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model;
std::map<ModelManager::FBXName, f_Model*>  ModelManager::fbxModel;

ModelManager* ModelManager::GetIns() {
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize() {
	LoadModel(FIELD, "LowPoly_Landscape");
	LoadModel(BOSSFIELD, "BossField");
	LoadModel(WOOD, "wood");
	LoadModel(CHEST, "chest");
	LoadModel(FENCE, "Fence");
	LoadFBXModel(GOLEM, "monster_golem_demo");
	LoadFBXModel(Enemy_B, "Eagle");
	LoadFBXModel(PLAYER, "monster_golem_demo");
	LoadModel(SPHERE, "sphere");
	LoadModel(NAIL, "Nail");
	LoadModel(SPHERE, "sphere");
	LoadModel(NAIL, "Nail");
	LoadModel(BACKGROUND, "BackGround");
	LoadModel(DAMAGEAREA, "BossFieldDamageArea");
	LoadModel(BOX, "Box");
	LoadModel(BEAM, "BeamObj");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName) {
	model[modelName] = new Model();
	model[modelName] = Model::CreateFromOBJ(fileName);
}
void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName) {
	fbxModel[modelName] = new f_Model();
	fbxModel[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
