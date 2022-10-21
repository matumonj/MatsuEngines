#include "SmallSword.h"
#include"PlayerControl.h"
SmallSword::~SmallSword()
{
	//delete  m_Model;
}
void SmallSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("axe");

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
}

void SmallSword::Update(DebugCamera* camera)
{
	const float Damage_Value = 5.0f;
	const float CoolTime_Value = 60.0f;

	Damage = Damage_Value;
	CoolTime = CoolTime_Value;
	
	MagicAttack();
	Bliz->Updata(camera);
	SlashArea->Updata(camera);
	m_Object->SetRotation(Rotation);
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), { 1.0f,1.0f,1.0f,1.0f }, camera);
	
}

void SmallSword::Draw()
{
	Draw_Obj();
	Bliz->Draw();
	SlashArea->Draw();
	ParticleManager::PreDraw();
	pMan->Draw();
	ParticleManager::PostDraw();
}

#include"EnemyControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"CustomButton.h"
void SmallSword::MagicAttack()
{
	if(CustomButton::GetInstance()->Get2AttackAction()){
		attackMotion = FIRESPHERE;
	}
	DirectX::XMFLOAT3 pPos= PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (attackMotion == NON) {
		EaseTime = { 0.0f,0.0f,0.0f };
		magicSpherePos =pPos;
	}

	if (attackMotion == FIRESPHERE || attackMotion == BLIZZARD) {

		if (EaseTime.x < 1.0f) {
			EaseTime.x += 0.02f;
			EaseTime.y += 0.02f;
			EaseTime.z += 0.02f;
		}
		magicSpherePos.x = Easing::EaseOut(EaseTime.x, pPos.x, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().x);
		magicSpherePos.y = Easing::EaseOut(EaseTime.y, pPos.y, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y);
		magicSpherePos.z = Easing::EaseOut(EaseTime.z, pPos.z, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().z);
	}
	float len = Collision::GetLength(magicSpherePos, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());

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

	if (len > 2.0f) {
		pMan->Add(120, magicSpherePos, vel, acc, 1.0f, 0.0f);
	}
	else {
		magicSpherePos = pPos;
		attackMotion = NON;
		Bliz->SetActFlag(TRUE);
	}
	
	pMan->SetColor({ 1,0,0,1 });
		pMan->Update(pMan->NORMAL);
	}
	
}

void SmallSword::Blizzard::Init(DebugCamera* camera)
{
	IceCrystalObj = std::make_unique<Object3d>();
	IceCrystalObj->Initialize(camera);
	Texture::LoadTexture(69, L"Resources/iceMagic.png");
	IceCrystalObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD));

	Texture*l_tex= { Texture::Create(69, {0.0f ,0.0f ,0.0f}, {100.0f ,100.0f ,1.0f}, {1.0f ,1.0f ,1.0f ,1.0f}) };
	IceMagicTex.reset(l_tex);
	IceMagicTex->CreateTexture();
	IceMagicTex->SetAnchorPoint({ 0.5f,0.5f });

	DestAlpha = 1.0f;
	DestTime = 0.0f;
}
void SmallSword::Slash::Init(DebugCamera* camera)
{
	Texture* l_tex[5];
	for (int i = 0; i < 5; i++) {
		l_tex[i] = {Texture::Create(93, {0.0f ,0.0f ,0.0f}, {100.0f ,100.0f ,1.0f}, {1.0f ,1.0f ,1.0f ,1.0f})};
		InpactTex[i].reset(l_tex[i]);
		InpactTex[i]->CreateTexture();
		InpactTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}

}

void SmallSword::Blizzard::Updata(DebugCamera* camera)
{
	const float texSclupSpeed = 0.2f;
	if (phase == NON) {
		EaseTime = 0.0f;
		IceExistence = 0;
		DestAlpha = 1.0f;
		DestTime = 0.0f;

		IceCrystalObj->SetDestFlag(FALSE);
		IceScl = { 3.0f,5.0f,3.0f };
		TexScl = { 0.0f,0.0f,1.0f };
		if (ActFlag) {
			IcePos.x = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().x;
			IcePos.z = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().z;

			phase = ACTIVE;
		}
	}
	else if (phase == ACTIVE) {
		IceExistence++;
		TexScl.x += texSclupSpeed;
		TexScl.y += texSclupSpeed;
		TexAlpha -= 0.01f;
		if (EaseTime < 1.0f) {
			EaseTime += 1.0f / 25.0f;
		}
		IcePos.y=Easing::EaseOut(EaseTime, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 20, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y);
		bool nextPhase = IceExistence > 90;
		if (nextPhase) {

			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(20);
			phase = DEST;
		}
		///IceScl.y = min(IceScl.y, 5.0f);
	}
	else if (phase == DEST) {
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
	
	if (phase == NON) {

		for (int i = 0; i < 5; i++) {
			if (Alpha[i] == 0.0f) {
				rotCorrection_Value[i] = rand() % 20 - 21;
				posCorrection_Value[i] = rand() % 20 - 21;
			}
		}
	//	if(ActFlag){
			phase = ACTIVE;
		//}
	}
	
	else if (phase == ACTIVE) {
		bool nextPhase=false;
		if (nextPhase) {
			phase = DEST;
		}
		
	} else if (phase == DEST) {
		
		bool EndDest=false;
		if (EndDest) {
			//IceCrystalObj->SetDestFlag(FALSE);

		}
	}
	XMFLOAT3 ePos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition();

	
for (int i = 0; i < 5; i++) {
		InpactTex[i]->SetDisplayRadius(100);
		

		InpactTex[i]->SetColor({ 1.0f,1.0f,1.0f,1 });
		InpactTex[i]->SetScale({ 2.0f,5.0f,1.0f });
		InpactTex[i]->Update( camera);
		InpactTex[i]->SetRotation({ 0.0f ,0.0f,0.0f + rotCorrection_Value[i] });

		InpactTex[i]->SetPosition({ ePos.x + (float)posCorrection_Value[i],ePos.y,ePos.z + (float)posCorrection_Value[i] });


		Alpha[i] = min(Alpha[i], 1.0f);
		Alpha[i] = max(Alpha[i], 0.0f);
}

}
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

#include"imgui.h"
void SmallSword::Slash::Draw()
{
	Texture::PreDraw();
	for (int i= 0; i < 5; i++) {
		InpactTex[i]->Draw();
	}
	Texture::PostDraw();

	ImGui::Begin("posrand");
	ImGui::Text("1 %d", posCorrection_Value[0]);
	ImGui::Text("2 %d", posCorrection_Value[1]);
	ImGui::Text("3 %d", posCorrection_Value[2]);
	ImGui::Text("4 %d", posCorrection_Value[3]);
	ImGui::Text("5 %d", posCorrection_Value[4]);
	ImGui::End();

}