#include "ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model;
std::map<ModelManager::FBXName, f_Model*> ModelManager::fbxModel;

ModelManager* ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::InitField()
{
	LoadModel(FIELD, "LowPoly_Landscape");
}

void ModelManager::Initialize()
{
	LoadModel(BOSSFIELD, "BossField");
	LoadModel(WOOD, "wood");
	LoadModel(WOOD2, "wood2");
	LoadModel(CHEST, "chest");
	LoadModel(BIGROCK_B, "crystal");
	LoadModel(FENCE, "Fence");

	LoadModel(NORMALSWORD, "sword");
	LoadModel(SPHERE, "sphere");
	LoadModel(NAIL, "Nail");
	LoadModel(BIGSWORD, "axe");
	LoadModel(BOX, "Box");
	LoadModel(PLACEBOX, "placebox");
	LoadModel(MISSILE, "missile");
	LoadModel(DESTROCK, "DestRock");
	LoadModel(SMALLSWORD, "Wand");
	LoadModel(SKY, "skydome");
	LoadModel(ICECRYSTAL, "crystal");
	LoadModel(BIGROCK_A, "BigStone_A");
	LoadModel(HOUSE, "house");
	LoadModel(GRASSFIELD, "GrassField");
	LoadModel(SANDFIELD, "Sandfield");
	LoadModel(SKYMAP, "BackGround");
	LoadModel(BEAM, "BeamObj");
	LoadModel(THROWROCK, "throwrock");
	LoadModel(BEAM2, "BossBeam");
	LoadModel(TOWER, "SkyBack");
	LoadModel(BOSSWEAPON, "BossWeapon");
	LoadModel(SHIELD, "Shield");
	LoadModel(SWITCH, "FootSwitch");
	LoadModel(CAGE, "Cage");
	LoadModel(TORCH, "Torch");
	LoadModel(CHARGEEFFECT, "ChargeEffect");
	LoadModel(RING, "CircleDamage");

	LoadFBXModel(BOSS, "boss");
	LoadFBXModel(PLAYER, "ko");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new Model();
	model[modelName] = Model::CreateFromOBJ(fileName,true);

	if(modelName==BOX)
	{
		model[modelName] = Model::CreateFromOBJ(fileName,false);
	}
}

void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName)
{
	fbxModel[modelName] = new f_Model();
	fbxModel[modelName] = FbxLoader::GetIns()->LoadModelFromFile(fileName);
}
