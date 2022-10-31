#include "SmallSword.h"
#include"PlayerControl.h"
#include"TargetMarker.h"


#include"EnemyControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"CustomButton.h"
#include"SceneManager.h"
SmallSword::~SmallSword()
{
	//delete  m_Model;
}
void SmallSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("Wand");
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	//ƒ‚ƒfƒ‹Š„‚è“–‚Ä
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);

	Scale = { 1,1,1 };

	Rotation = { 0,0 + 30,0 + 100 };

	pMan= ParticleManager::Create(2, L"Resources/ParticleTex/normal.png");

	Bliz = std::make_unique<Blizzard>();
	Bliz->Init(camera);

	SlashArea = std::make_unique<Slash>();
	SlashArea->Init(camera);
	TargetMarker::GetInstance()->Initialize();
}

void SmallSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 5.0f;
	const float CoolTime_Value = 60.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		TargetMarker::GetInstance()->Update_Tutorial(camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		TargetMarker::GetInstance()->Update_PlayScene(camera);
	}
	
	m_Object->SetRotation(Rotation);
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1.0f,1.0f,1.0f,1.0f }, camera);
	MagicAttack();
	Bliz->Updata(camera);
	SlashArea->Updata(camera);

	
}

void SmallSword::Draw()
{
	Draw_Obj();
	Bliz->Draw();
	SlashArea->Draw();
	TargetMarker::GetInstance()->Draw();
	ParticleManager::PreDraw();
	pMan->Draw();
	ParticleManager::PostDraw();
	ImGui::Begin("rodpos");
			ImGui::SliderFloat("x", &Correction.x,-10.0,10.0f);
			ImGui::SliderFloat("y", &Correction.y, -10.0, 10.0f);
			ImGui::SliderFloat("z", &Correction.z, -10.0, 10.0f);
		
	ImGui::End();
}
void SmallSword::MagicAttack()
{
	int index = TargetMarker::GetInstance()->GetNearIndex();

	if(CustomButton::GetInstance()->Get2AttackAction()){
		//CameraTargetEnemy = true;
		//ReturnMoveCameraTarget = false;
		attackMotion = FIRESPHERE;
	}
	if (CustomButton::GetInstance()->GetAttackAction()) {
		//CameraTargetEnemy = true;
		//ReturnMoveCameraTarget = false;
		attackMotion = BLIZZARD;
	}
	if (Bliz->Getphase() == Bliz->DEST || SlashArea->Getphase() == SlashArea->DEST) {
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);	
		// 	ReturnMoveCameraTarget = true;
		}
	DirectX::XMFLOAT3 pPos= PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (attackMotion == NON) {
		EaseTime = { 0.0f,0.0f,0.0f };
		//magicSpherePos = { m_Object->ExtractPositionMat().r[3].m128_f32[0]+Correction.x, m_Object->ExtractPositionMat().r[3].m128_f32[1]+Correction.y, m_Object->ExtractPositionMat().r[3].m128_f32[2]+Correction.z };
	}

	if (attackMotion == FIRESPHERE || attackMotion == BLIZZARD) {

		if (EaseTime.x < 1.0f) {
			EaseTime.x += 0.02f;
			EaseTime.y += 0.02f;
			EaseTime.z += 0.02f;
		}
		
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
				bool canUseAttack = Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition()) < 50.0f;
				if (canUseAttack) {
					magicSpherePos.x = Easing::EaseOut(EaseTime.x, pPos.x, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[index]->GetPosition().x);
					magicSpherePos.y = Easing::EaseOut(EaseTime.y, pPos.y, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[index]->GetPosition().y);
					magicSpherePos.z = Easing::EaseOut(EaseTime.z, pPos.z, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[index]->GetPosition().z);
				}
			}
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			magicSpherePos.x = Easing::EaseOut(EaseTime.x, pPos.x, EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().x);
			magicSpherePos.y = Easing::EaseOut(EaseTime.y, pPos.y, EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y);
			magicSpherePos.z = Easing::EaseOut(EaseTime.z, pPos.z, EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().z);

		}
	}
	std::vector<float>len;
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		len.resize(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL).size());
		for (int i = 0; i < len.size(); i++) {
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
				len[i] = Collision::GetLength(magicSpherePos, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		len.resize(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size());
		for (int i = 0; i < len.size(); i++) {
			len[i] = Collision::GetLength(magicSpherePos, EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
		}
	}
	
	for (int i = 0; i < 1; i++) {
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//’Ç‰Á
		for (int i = 0; i < len.size(); i++) {
			if (len[i] > 2.0f) {
				pMan->Add(120, magicSpherePos, vel, acc, 1.0f, 0.0f);
			}
			else {
				magicSpherePos = pPos;
				if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
					if (attackMotion == BLIZZARD && EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
						Bliz->SetActFlag(TRUE);
					}
					if (attackMotion == FIRESPHERE && EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
						SlashArea->SetActFlag(TRUE);
					}
				}
				if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
					if (attackMotion == BLIZZARD && EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index] != nullptr) {
						Bliz->SetActFlag(TRUE);
					}
					if (attackMotion == FIRESPHERE && EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index] != nullptr) {
						SlashArea->SetActFlag(TRUE);
					}
				}

				attackMotion = NON;

			}
		}

	if (attackMotion == BLIZZARD) {
		pMan->SetColor({ 0.1,0.1,1.0f,0.6f });
	}
	if (attackMotion == FIRESPHERE) {
		pMan->SetColor({ 0.8,0.8,0.8f,0.6f });
	}

	}

	pMan->Update(pMan->NORMAL);
}

void SmallSword::Blizzard::Init(DebugCamera* camera)
{
	IceCrystalObj = std::make_unique<Object3d>();
	IceCrystalObj->Initialize(camera);
	Texture::LoadTexture(69, L"Resources/iceMagic.png");
	IceCrystalObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ICECRYSTAL));

	Texture*l_tex= { Texture::Create(69, {0.0f ,0.0f ,0.0f}, {100.0f ,100.0f ,1.0f}, {1.0f ,1.0f ,1.0f ,1.0f}) };
	IceMagicTex.reset(l_tex);
	IceMagicTex->CreateTexture();
	IceMagicTex->SetAnchorPoint({ 0.5f,0.5f });

	DestAlpha = 1.0f;
	DestTime = 0.0f;
}
void SmallSword::Slash::Init(DebugCamera* camera)
{
	Texture* l_tex[TexNum];
	Texture::LoadTexture(24, L"Resources/crystal.jpg");
	for (int i = 0; i<TexNum; i++) {
		l_tex[i] = {Texture::Create(24, {0.0f ,0.0f ,0.0f}, {100.0f ,100.0f ,1.0f}, {1.0f ,1.0f ,1.0f ,1.0f})};
		InpactTex[i].reset(l_tex[i]);
		InpactTex[i]->CreateTexture();
		InpactTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}

	pMan[0] = ParticleManager::Create(2, L"Resources/ParticleTex/normal.png");
	pMan[1] = ParticleManager::Create(2, L"Resources/ParticleTex/normal.png");
}

void SmallSword::Beam::Init(DebugCamera* camera)
{
	HolyObj = std::make_unique<Object3d>();
	HolyObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
}


/*-------------*/
/*    XV     */
/*-------------*/
void SmallSword::Blizzard::Updata(DebugCamera* camera)
{
	const float texSclupSpeed = 0.2f;
	if (phase == NON) {
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {

			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(false);
			}
			if (ActFlag) {
				IcePos.x = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().x;
				IcePos.z = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().z - 5;

				EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(20);
				phase = ACTIVE;
			}
		}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {

			int index = TargetMarker::GetInstance()->GetNearIndex();
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(false);
			if (ActFlag) {
				IcePos.x = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().x;
				IcePos.z = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().z - 5;

				EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->RecvDamage(20);
				phase = ACTIVE;
			}
		}
		EaseTime = 0.0f;
		IceExistence = 0;
		DestAlpha = 1.0f;
		DestTime = 0.0f;

		IceCrystalObj->SetDestFlag(FALSE);
		IceScl = { 5.0f,5.0f,5.0f };
		TexScl = { 0.0f,0.0f,1.0f };
	
	}
	else if (phase == ACTIVE) {

		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
			IcePos.y = Easing::EaseOut(EaseTime, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 20.0f, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 5.0f);
		}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			int index = TargetMarker::GetInstance()->GetNearIndex();

			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
			IcePos.y = Easing::EaseOut(EaseTime, EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y - 20.0f,
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y - 5.0f);
		}
		IceExistence++;
		TexScl.x += texSclupSpeed;
		TexScl.y += texSclupSpeed;
		TexAlpha -= 0.04f;
		if (EaseTime < 1.0f) {
			EaseTime += 1.0f / 25.0f;
		}
		bool nextPhase = IceExistence > 90;
		if (nextPhase) {

			phase = DEST;
		}
		///IceScl.y = min(IceScl.y, 5.0f);
	}
	else if (phase == DEST) {
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
		}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			int index = TargetMarker::GetInstance()->GetNearIndex();

			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
		}
		TexScl.x += texSclupSpeed;
		TexScl.y += texSclupSpeed;
		DestTime += 0.2f;
		DestAlpha -= 0.02f;

		IceCrystalObj->SetDestFlag(TRUE);
		IceCrystalObj->SetDestTime(DestTime);

		bool EndDest = DestAlpha < 0.0f;
		if (EndDest) {
			//IceCrystalObj->SetDestFlag(FALSE);
			ActFlag = false;
			phase = NON;
		}
	}
	IceMagicTex->SetDisplayRadius(100);
	IceMagicTex->SetBillboard(false);
	
	IceCrystalObj->SetScale(IceScl);
	IceCrystalObj->SetPosition(IcePos);
	IceCrystalObj->SetColor({ 1.0f,1.0f,1.0f,DestAlpha });
	IceCrystalObj->Update({ 1.0f,1.0f,1.0f,DestAlpha },camera);
	
	IceMagicTex->Update(camera);
	IceMagicTex->SetPosition({ IcePos.x, IcePos.y, IcePos.z });
	IceMagicTex->SetRotation({ 90.0f,0.0f,0.0f });
	IceMagicTex->SetScale(TexScl);
	IceMagicTex->SetColor({ 1,1,1,TexAlpha});

}

void SmallSword::Slash::Updata(DebugCamera* camera)
{
	int index = TargetMarker::GetInstance()->GetNearIndex();
	if (phase == NON) {
		if(ActFlag){
			for (int i = 0; i < TexNum; i++) {
				int randAlpha = rand() % 2;
				Alpha[i] = (float)randAlpha / 1.50f;
				//Alpha[i] = 1.0f;
				TexScale[i] = { 1.0f,2.0f,2.0f };
				slashCount[i] = 0;
				next = false;
			}
			if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[index]->RecvDamage(40);
			}
			if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->RecvDamage(40);
			}
			phase = ACTIVE;
		}
	}
	
	else if (phase == ACTIVE) {
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
		}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
		}
		for (int i = 1; i < TexNum; i++) {
			if (slashCount[0] <= 3||Alpha[0]>0.0f) {
				continue;
			}
			if (slashCount[0]== slashCount[i]&&Alpha[0]==Alpha[i]) {
				next = true;
			}
		}
		bool nextPhase;
		XMFLOAT3 ePos;
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			ePos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition();
		}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			ePos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition();
		}
		for (int i = 0; i<TexNum; i++) {
			Alpha[i] -= 0.01f;
			TexScale[i].x -= 0.02f;
			TexScale[i].y += 0.2f;
			if (TexScale[i].y >= 6.0f&&slashCount[i] <= 3) {
				rotCorrection_Value[i] = rand() % 60 - 31;
				posCorrection_Valuex[i] = rand() % 20- 10;
				posCorrection_Valuey[i] = rand() % 20 - 10;
				posCorrection_Valuez[i] = rand() % 5 - 10;
				InpactTex[i]->SetPosition({ ePos.x + (float)posCorrection_Valuex[i],ePos.y+(float)posCorrection_Valuey[i],ePos.z + (float)posCorrection_Valuez[i] });
				Alpha[i] = 1.0f;
				slashCount[i]++;
				TexScale[i] = { 1.0f,2.0f,2.0f };
				
			}
		}
		if (next) {
			phase = DEST;
		}
		
	} else if (phase == DEST) {
		next = false;
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[index]->SetMoveStop(false);
		}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(false);
		}
		phase = NON;
		ActFlag = false;
	}
	
	
for (int i = 0; i<TexNum; i++) {
		InpactTex[i]->SetDisplayRadius(100);
		

		InpactTex[i]->SetColor({ 1.0f,1.0f,1.0f,Alpha[i]});
		InpactTex[i]->SetScale(TexScale[i]);
		InpactTex[i]->Update( camera);
		InpactTex[i]->SetRotation({ 0.0f ,0.0f,0.0f + rotCorrection_Value[i] });

		

		Alpha[i] = min(Alpha[i], 1.0f);
		Alpha[i] = max(Alpha[i], 0.0f);
}

}


void SmallSword::Beam::Updata(DebugCamera* camera)
{
	const float texSclupSpeed = 0.2f;
	if (phase == NON) {
		EaseTime = 0.0f;
		if (ActFlag) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(20);
			phase = ACTIVE;
		}
	} else if (phase == ACTIVE) {
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
		if (EaseTime < 1.0f) {
			EaseTime += 1.0f / 25.0f;
		}
		bool nextPhase;
		if (nextPhase) {

			phase = DEST;
		}
		///IceScl.y = min(IceScl.y, 5.0f);
	} else if (phase == DEST) {
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(false);
		bool EndDest;
		if (EndDest) {
			//IceCrystalObj->SetDestFlag(FALSE);
			ActFlag = false;
			phase = NON;
		}
	}
	
	HolyObj->SetPosition(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	HolyObj->SetScale(HolyScl);
	HolyObj->SetRotation(HolyRot);
	HolyObj->Update({ 1.0f,1.0f,1.0f,1.0f }, camera);
}


/*-------------*/
/*     •`‰æ    */
/*-------------*/
void SmallSword::Blizzard::Draw()
{
	if (!ActFlag)return;
	Object3d::PreDraw();
	
	IceCrystalObj->Draw();
	Object3d::PostDraw();

	Texture::PreDraw();
	IceMagicTex->Draw();
	Texture::PostDraw();
}

void SmallSword::Slash::Draw()
{
	Texture::PreDraw();
	for (int i= 0; i<TexNum; i++) {
		InpactTex[i]->Draw();
	}
	Texture::PostDraw();

	
}
void SmallSword::Beam::Draw()
{
	if (!ActFlag)return;
	Object3d::PreDraw();

	HolyObj->Draw();
	Object3d::PostDraw();

}