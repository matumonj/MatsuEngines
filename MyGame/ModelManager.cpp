#include "ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model;
std::map<ModelManager::FBXName, f_Model*> ModelManager::fbxModel;

ModelManager* ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(FIELD, "LowPoly_Landscape");
	LoadModel(BOSSFIELD, "BossField");
	LoadModel(WOOD, "wood");
	LoadModel(WOOD2, "wood2");
	LoadModel(CHEST, "chest");
	LoadModel(BIGROCK_B, "crystal");
	LoadModel(FENCE, "Fence");
	LoadFBXModel(PLAYER, "playerGolem");
	LoadFBXModel(BOSS, "boss");
	LoadModel(SPHERE, "sphere");
	LoadModel(NAIL, "Nail");
	LoadModel(BIGSWORD, "axe");
	LoadModel(DAMAGEAREA, "BossFieldDamageArea");
	LoadModel(BOX, "Box");
	LoadModel(PLACEBOX, "placebox");
	LoadModel(MISSILE, "missile");
	LoadModel(SMALLSWORD, "Hammer");
	LoadModel(ICECRYSTAL, "crystal");
	LoadModel(CELESTIALSPHERE, "skydome");
	LoadModel(NORMALSWORD, "sword");
	LoadModel(BIGROCK_A, "BigStone_A");
	LoadModel(HOUSE, "house");
	LoadModel(GRASSFIELD, "GrassField");
	LoadModel(SANDFIELD, "Sandfield");
	LoadModel(SKYMAP, "BackGround");
	LoadModel(LANTHANUM, "lanthanum");
	LoadModel(BEAM, "BeamObj");
	LoadModel(THROWROCK, "throwrock");
	LoadModel(BEAM2, "BossBeam");
	LoadModel(TOWER, "SkyBack");
	LoadModel(BOSSWEAPON, "BossWeapon");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new Model();
	model[modelName] = Model::CreateFromOBJ(fileName,true);
}

void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName)
{
	fbxModel[modelName] = new f_Model();
	fbxModel[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
