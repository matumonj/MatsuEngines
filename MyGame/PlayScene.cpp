#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"MeshCollider.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SphereCollider.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"HUD.h"
#include"PlayerAttackState.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include "FollowJudgement.h"
#include"FollowAction.h"
#include"StayJudgment.h"
#include"StayAction.h"
#include"EnemyAttackAction.h"
#include"SistemConfig.h"
#include"EnemyAttackJudgement.h"
//�V�[���̃R���X�g���N�^
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region �X�v���C�g�̐���
//�X�v���C�g����
void PlayScene::SpriteCreate()
{
	HUD::GetInstance()->Initialize();
	HUD::GetInstance()->EnemyHPGaugeInitialize();
	HUD::GetInstance()->SkillButtonInitialize();
	gameui = new GameUI();
	gameui->Initialize();
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont2.png");
	Sprite::LoadTexture(1, L"Resources/0057b6fa9ec85ae.jpg");
	Sprite::LoadTexture(2, L"Resources/tyuta_C.png");

	//���ʂ̃e�N�X�`��(�X�v���C�g����Ȃ���)
	Texture::LoadTexture(6, L"Resources/DQVDS_-_Mechanowyrm.png");
	Texture::LoadTexture(1, L"Resources/ball.png");
	//���f�������w�肵�ăt�@�C���ǂݍ���
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	SistemConfig::GetInstance()->Initialize();
}
#pragma endregion

#pragma region ���f���ƃG�t�F�N�g�ƃ��C�g�̃C���X�^���X����
void PlayScene::ModelCreate(DebugCamera* camera)
{

	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	//�p�����[�^�̐ݒ�
	lightGroup->LightSetting();

	Field::GetInstance()->Initialize(camera);

	collisionManager = CollisionManager::GetInstance();


}
#pragma endregion

#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void PlayScene::objUpdate(DebugCamera* camera)
{
	XMFLOAT3 oldp = Player::GetInstance()->GetPosition();
	Player::GetInstance()->Update({ 1,1,1,p_alpha }, camera);

	for (int i = 0; i < Enemy_Quantity; i++) {
		if (enemys[i] != nullptr) {
			enemys[i]->SetMoveFlag(true);
			enemys[i]->Update({ 1,1,1,e_alpha }, camera);
			enemys[i]->SearchAction(camera);
		}
	}
	for (int i = 0; i < Wood_Quantity; i++) {
		if (woods[i] != nullptr) {

			woods[i]->Update(camera);
		}
	}
	for (int i = 0; i < Fence_Quantity; i++) {
		if (fences[i] != nullptr) {

			fences[i]->Update(camera);
		}
	}
	for (int i = 0; i < Wood_Quantity; i++) {

		if (woods[i]->CollideWood() == true) {
			Player::GetInstance()->SetPosition(oldp);
			Player::GetInstance()->SetGround(true);
			break;
		} else {
			turnoff_player = false;
		}
	}

}
#pragma endregion

#pragma region ������
void PlayScene::Initialize()
{
	input = Input::GetInstance();
	behavior.AddNode("", "Root", 0, BehaviorTree::SELECT_RULE::PRIORITY, NULL, NULL);
	behavior.AddNode("Root", "Attack", 1, BehaviorTree::SELECT_RULE::NON, EnemyAttackJudgement::GetInstance(), EnemyAttackAction::GetInstance());
	behavior.AddNode("Root", "Walk", 2, BehaviorTree::SELECT_RULE::NON, WalkJudgment::GetInstance(), WalkAction::GetInstance());
	behavior.AddNode("Root", "Follow", 3, BehaviorTree::SELECT_RULE::NON, FollowJudgement::GetInstance(), FollowAction::GetInstance());
	//behavior.AddNode("Att", "MagicAttack", 2, BehaviorTree::SELECT_RULE::NON, MagicAttackJudgment::GetInstance(), MagicAction::GetInstance());
		//behavior.AddNode("Attack", "SkillAttack", 3, BehaviorTree::SELECT_RULE::NON, SkillAttackJudgment::GetInstance(), SkillAction::GetInstance());


	TargetMarker::GetInstance()->Initialize();
	//
	pobbcol = new OBBCollision();
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height/*input*/);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	camera->SetEye({ Player_Pos.x,Player_Pos.y + 5,Player_Pos.z - 15 });

	SpriteCreate();//
	ModelCreate(camera);//

	lightGroup->SetSpotLightPos(SpotLightPos);
	lightGroup->SetSpotLightColor(SpotLightColor);

	//f_Object3d::SetDevice();
	//�J�������Z�b�g
	f_Object3d::SetCamera(camera);
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	Player::GetInstance()->Initialize(camera);

	postEffect = new PostEffect();
	postEffect->Initialize();

}
#pragma endregion

#pragma region �X�V����
void PlayScene::Update()
{
	lightGroup->SpotLightUpdate();

	SistemConfig::GetInstance()->Update();
	LoadParam();
	LoadParam_Wood();
	LoadParam_Fence();
	if (input->Pushkey(DIK_RIGHT)) {
		charaAngle += 0.5f;
		cameraAngle -= 0.5f;
		Player::GetInstance()->SetCharaRotation(charaAngle);
	} else if (input->Pushkey(DIK_LEFT)||input->RightTiltStick(input->Left)) {
		cameraAngle += 0.5f;
		charaAngle -= 0.5f;
		Player::GetInstance()->SetCharaRotation(charaAngle);
	}
	if (cameraAngle >= 360 + 90 || cameraAngle <= -360) {
		cameraAngle = 0;
	}
	//FBX���f���̍X�V
	//object1->Updata(TRUE);
	CameraPosition.x = Player::GetInstance()->GetPosition().x + cosf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.z = Player::GetInstance()->GetPosition().z + sinf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.y = Player::GetInstance()->GetPosition().y + CameraHeight;

	//�J�����֌W�̏���
	camera->SetDistance(distance);//
	camera->SetEye(CameraPosition);
	camera->SetTarget({ Player::GetInstance()->GetPosition() });
	camera->Update();

	Field::GetInstance()->Update(camera);

	if (enemys.size() > 1) {
		TargetMarker::GetInstance()->Update(enemys, camera, Player::GetInstance());
		HUD::GetInstance()->EnemyHPGaugeUpdate(enemys);
		HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera, enemys);

		PlayerAttackState::GetInstance()->Update(enemys);
		
		TutorialFenceOpen = enemys[0]->GetHP() <= 0;
		fences[0]->FenceOpenCondition(TutorialFenceOpen);
	}

	objUpdate(camera);//�I�u�W�F�N�g�̍X�V����


	//gameui->Update(enemys, targetm);
	//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         BV
	//�V�[���`�F���W
	
	HUD::GetInstance()->SkillBottonUpdate();
	HUD::GetInstance()->Update();
	if (input->TriggerKey(DIK_R)) {//�����ꂽ��
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

}
#pragma endregion 

//�X�v���C�g�̕`��
#pragma region ���f���̕`��
void PlayScene::SpriteDraw()
{
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	Player::GetInstance()->PreDraw();
	if (!turnoff_player) {//�f�o�b�O�p
		Player::GetInstance()->Draw();
	}
	Player::GetInstance()->PostDraw();
	for (int i = 0; i < Enemy_Quantity; i++) {
		if (enemys[i] != nullptr&&enemys[i]->State_Dead()==false) {
			if (!turnoff_enemy) {//�f�o�b�O�p
				enemys[i]->Draw();
				enemys[i]->SearchDraw();
			}
		}
	}
	for (int i = 0; i < Wood_Quantity; i++) {
		if (woods[i] != nullptr) {
			woods[i]->Draw();
		}
	}
	for (int i = 0; i < Fence_Quantity; i++) {
		if (fences[i] != nullptr) {
			fences[i]->Draw();
		}
	}
}
//s�v���C�ƈȊO�̕`��
void PlayScene::MyGameDraw()
{

	Field::GetInstance()->Draw();

	SpriteDraw();

	Texture::PreDraw();
	TargetMarker::GetInstance()->Draw();
	Texture::PostDraw();


	//gameui->Draw(enemys,targetm,Player::GetInstance());
	//���ʂ̃e�N�X�`���̕`��
	//if (input->Pushkey(DIK_P)) {
	//Player::GetInstance()->normalAttack(targetm,enemys,camera);

}
#pragma endregion
//���ɓ���
#pragma region �`��(imgui�ƃX�v���C�g�ƃ��f���܂Ƃ߂�����)
void PlayScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̏ꍇ�킯(B�łڂ��� D���f�t�H���g)
	switch (c_postEffect)
	{
	case Blur://�ڂ����@�`�揀�Ⴄ����
		postEffect->PreDrawScene();
		MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();
		if (HUD::GetInstance()->GetLayOutMode()) {
			HUD::GetInstance()->EnemyHPGauge_MultiDraw();
			HUD::GetInstance()->Draw();
			HUD::GetInstance()->SkillBottonDraw();
		}
			SistemConfig::GetInstance()->Draw();
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		HUD::GetInstance()->EnemyHPGauge_MultiDraw();
		HUD::GetInstance()->Draw();
		HUD::GetInstance()->SkillBottonDraw();

		SistemConfig::GetInstance()->Draw();
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#pragma endregion

void PlayScene::ImGuiDraw()
{
	{
		ImGui::Begin("Obj1");
		ImGui::SetWindowPos(ImVec2(0, 500));
		ImGui::SetWindowSize(ImVec2(500, 300));
		if (ImGui::TreeNode("light_position")) {
			ImGui::SliderFloat("positionX", &SpotLightPos[0], -100, 100);
			ImGui::SliderFloat("positionY", &SpotLightPos[1], -100, 100);
			ImGui::SliderFloat("positionZ", &SpotLightPos[2], -100, 100);

			lightGroup->SetSpotLightPos(SpotLightPos);
			if (ImGui::Button("spotlight ON")) {
				lightGroup->SetSpotLightActive(0, true);
			}
			if (ImGui::Button("spotlight OFF")) {
				lightGroup->SetSpotLightActive(0, false);
			}
			ImGui::ColorPicker3("light_color", SpotLightColor);
			lightGroup->SetSpotLightColor(SpotLightColor);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Damage")) {
			int d = PlayerAttackState::GetInstance()->GetDamage();
			ImGui::SliderInt("positionX", &d, -100, 100);
			ImGui::TreePop();
		}
		ImGui::End();
	}
	//
	
	{
		ImGui::Begin("None");
		if (ImGui::Button("Load", ImVec2(70, 50))) {
			LoadEnemy = true;
		}
		//ImGui::Image(L"Resources/0057b6fa9ec85ae.jpg", ImVec2(100, 100));
		ImGui::End();
	}
	//
	Player::GetInstance()->ImguiDraw();
	{//�J����
		ImGui::Begin("Camera");
		ImGui::SliderFloat("positionXZ", &CameraDis, 0, 100);
		ImGui::SliderFloat("positionY", &CameraHeight, 0, 30);
		bool defaultPos;
		if (ImGui::RadioButton("DefaultPosition", &defaultPos)) {
			CameraDis = 25;
			CameraHeight = 9;
		}
		ImGui::SliderFloat("rotationX", &cameraAngle, -360, 360);

		ImGui::End();
	}
	//
	{//�G
		ImGui::Begin("Enemy");
		POINT p;
		GetCursorPos(&p);
		ImGui::Text(" Maus %d", p.x);
		///ScreenToClient(FindWindowA("DirectX", nullptr), &p);

		ImGui::Text(" Enemy_Quantitys %d", Enemy_Quantity);

		ImGui::Text(" EY %f", ey);

		ImGui::Text(" EY2 %f", ey1);
		
		ImGui::End();
	}
	{
		ImGui::Begin("Drawing");
		ImGui::Checkbox("TurnOff_Player", &turnoff_player);
		ImGui::Checkbox("TurnOff_Enemy", &turnoff_enemy);

		ImGui::End();
	}
	ImGui::Begin("Scene");

	if (ImGui::RadioButton("Scene_Create", t)) {
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (ImGui::RadioButton("Scene_Tittle", y)) {
		BaseScene* scene = new TitleScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	ImGui::End();
	//
	{
		unsigned long current_time = timeGetTime();
		float fps = float(count_frame) / (current_time - prev_time) * 1000;
		ImGui::SliderFloat("FPS", &fps, -10, 50);
		count_frame++;
		ImGui::End();
	}
}
#pragma region �������
void PlayScene::Finalize()
{
	delete model, model5, model2;
	//delete Player::GetInstance();


}

void PlayScene::LoadParam()
{
	if(LoadEnemy){
		//file2.open("EnemyParam_CSV/position.csv");
		file.open("EnemyParam_CSV/open.csv");
		//file2.open("EnemyParam_CSV/wood.csv");

		popcom << file.rdbuf();

		//popcom2 << file2.rdbuf();
		
		file.close();
		//file2.close();
		//return oi;
		//fopen_s(&fp, "posxx.json", "r");
		/*����Ƃ��Ă�
		�G�̐��ǂݍ���->
		�ǂݍ��񂾓G�̐����G�l�~�[�̃p�����[�^�z��̗v�f�����₷->
		�G�̐����������z��̒��g�����[�h�������̂�->
		�G�̔ԍ���1�������烿,2����������ŃC���X�^���X�����A������
		*/
		//fread(&Enemy_Quantity, sizeof(int), 1, fp);
		while (std::getline(popcom, line)) {
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("Enemy_Quantity") == 0) {
				std::getline(line_stream, word, ',');
				int quantity = (int)std::atof(word.c_str());
				Enemy_Quantity = quantity;
				break;
			}
		}
		Num.resize(Enemy_Quantity);
		stpos.resize(Enemy_Quantity);
		for (int i = 0; i < Enemy_Quantity; i++) {
			while (std::getline(popcom, line)) {
				std::istringstream line_stream(line);
				std::string word;
				std::getline(line_stream, word, ',');

				if (word.find("//") == 0) {
					continue;
				}
				if (word.find("Number") == 0) {
					std::getline(line_stream, word, ',');
					int number = (int)std::atof(word.c_str());
					Num[i] = number;
				}
				else if (word.find("POP") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());
					
					stpos[i] = { x,y,z };
					break;
				}
			}
		}
		enemys.resize(Enemy_Quantity);

		Load_EnemyPosition.resize(Enemy_Quantity);

		for (int i = 0; i < Enemy_Quantity; i++) {

			//����������
			if (Num[i] == 1) {
				enemys[i] = std::make_unique<MobEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
			}
			if (Num[i] == 2) {
				enemys[i] = std::make_unique<BossEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
			}

			enemys[i]->Initialize(camera);
			enemys[i]->SetEnemyPosition(stpos[i]);
			enemys[i]->SearchInit();
		}
		
		//fclose(fp);
		//LoadEnemy = false;
		hudload = true;
	}
}



void PlayScene::LoadParam_Wood()
{
	if (LoadEnemy) {
		//file2.open("EnemyParam_CSV/position.csv");
		//file.open("EnemyParam_CSV/open.csv");
		file2.open("EnemyParam_CSV/wood.csv");

		//popcom << file.rdbuf();

		popcom2 << file2.rdbuf();

		//file.close();
		file2.close();
		//return oi;
		//fopen_s(&fp, "posxx.json", "r");
		/*����Ƃ��Ă�
		�G�̐��ǂݍ���->
		�ǂݍ��񂾓G�̐����G�l�~�[�̃p�����[�^�z��̗v�f�����₷->
		�G�̐����������z��̒��g�����[�h�������̂�->
		�G�̔ԍ���1�������烿,2����������ŃC���X�^���X�����A������
		*/
		//fread(&Enemy_Quantity, sizeof(int), 1, fp);
		while (std::getline(popcom2, line2)) {
			std::istringstream line_stream(line2);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("Wood_Quantity") == 0) {
				std::getline(line_stream, word, ',');
				int quantity = (int)std::atof(word.c_str());
				Wood_Quantity = quantity;
				break;
			}
		}
		Wood_Num.resize(Wood_Quantity);
		woodpos.resize(Wood_Quantity);
		for (int i = 0; i < Wood_Quantity; i++) {
			while (std::getline(popcom2, line2)) {
				std::istringstream line_stream(line2);
				std::string word;
				std::getline(line_stream, word, ',');

				if (word.find("//") == 0) {
					continue;
				}
				 if (word.find("POP") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					woodpos[i] = { x,y,z };
					break;
				}
			}
		}
		woods.resize(Wood_Quantity);

		Load_WoodPosition.resize(Wood_Quantity);

		for (int i = 0; i < Wood_Quantity; i++) {

			woods[i] = std::make_unique<Wood>();
			
			woods[i]->Initialize(camera);
			woods[i]->SetPosition(woodpos[i]);
		}

		//fclose(fp);
		hudload = true;
	}
}



void PlayScene::LoadParam_Fence()
{
	if (LoadEnemy) {
		//file2.open("EnemyParam_CSV/position.csv");
		//file.open("EnemyParam_CSV/open.csv");
		file3.open("EnemyParam_CSV/fence.csv");

		//popcom << file.rdbuf();

		popcom3 << file3.rdbuf();

		//file.close();
		file3.close();
		//return oi;
		//fopen_s(&fp, "posxx.json", "r");
		/*����Ƃ��Ă�
		�G�̐��ǂݍ���->
		�ǂݍ��񂾓G�̐����G�l�~�[�̃p�����[�^�z��̗v�f�����₷->
		�G�̐����������z��̒��g�����[�h�������̂�->
		�G�̔ԍ���1�������烿,2����������ŃC���X�^���X�����A������
		*/
		//fread(&Enemy_Quantity, sizeof(int), 1, fp);
		while (std::getline(popcom3, line3)) {
			std::istringstream line_stream(line3);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("Fence_Quantity") == 0) {
				std::getline(line_stream, word, ',');
				int quantity = (int)std::atof(word.c_str());
				Fence_Quantity = quantity;
				break;
			}
		}
		Fence_Num.resize(Fence_Quantity);
		fencepos.resize(Fence_Quantity);
		for (int i = 0; i <Fence_Quantity; i++) {
			while (std::getline(popcom3, line3)) {
				std::istringstream line_stream(line3);
				std::string word;
				std::getline(line_stream, word, ',');

				if (word.find("//") == 0) {
					continue;
				}
				if (word.find("POP") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					fencepos[i] = { x,y,z };
					break;
				}
			}
		}
		fences.resize(Fence_Quantity);

		Load_FencePosition.resize(Fence_Quantity);

		for (int i = 0; i < Fence_Quantity; i++) {

			fences[i] = std::make_unique<AreaFence>();

			fences[i]->Initialize(camera);
			fences[i]->SetPosition(fencepos[i]);
		}

		//fclose(fp);
		LoadEnemy = false;
	}
}
#pragma endregion
