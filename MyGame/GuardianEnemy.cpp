#include "GuardianEnemy.h"
#include "PlayerControl.h"
#include "CameraControl.h"
#include "GuardianShotAttack.h"
#include "GuardianNAttack.h"
#include "ImageManager.h"
#include "mHelper.h"

GuardianEnemy::GuardianEnemy()
{
	state_guardian->Initialize(this);
}

GuardianEnemy::~GuardianEnemy()
{
	for (int i = 0; i < HPFrame.size(); i++)
	{
		//HPFrame[i].reset(nullptr);
	}
	smokepar.reset(nullptr);
	m_Object.reset(nullptr);

	//GuardianBomAttack::GetIns()->Finalize();
	//GuardianShotAttack::GetIns()->Finalize();
}


void GuardianEnemy::HPFrameInit()
{
	//HPバー初期化
	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	Sprite* l_enemyname = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENEMYNAME_GUARDIAN), {0, 0});

	HPFrame[0].reset(l_frame1);
	HPFrame[1].reset(l_frame2);
	HPFrame[2].reset(l_frame3);
	HPFrame[3].reset(l_frame4);
	EnemyName.reset(l_enemyname);
	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->SetAnchorPoint({0.0f, 0.0f});
	}
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
}

void GuardianEnemy::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_Object->SetModel(Model::CreateFromOBJ("Guardian"));

	MaxHP = 200;

	EnemyHP = MaxHP;
	//パラメータのセット
	Rotation = {114.0f, 118.0f, 165.0f};
	Scale = {1.f, 1.f, 1.f};

	//コライダー周り
	radius_adjustment = 0;
	SetCollider();

	//FBX切り替わりのタイム指定
	NormalAttackTime = 0.9f;
	DeathTime = 6.9f;
	Position = {-300.f, -50.f, 270.f};
	GuardianBomAttack::GetIns()->TexSet();
	GuardianShotAttack::GetIns()->TexSet();
	GuardianNAttack::GetIns()->TexSet();
	Color = {1.f, 1.f, 1.f, alpha};

	ENumber = GUARDIAN;

	HPFrameInit();

	Texture::LoadTexture(68, L"Resources/2d/BossAttackEffect/missileeffect.png");
	smokepar = std::make_unique<Particle>();
	smokepar->Init(64);
	smokepar->SetParScl({1.f, 1.f});
	smokepar->SetParColor({1.f, 1.f, 1.f, 1.f});
}

void GuardianEnemy::Move()
{
	PosYMovingT++;

	Position.y = -24.f + sinf(PI * 2.f / 120.f * PosYMovingT) * 2.f;
}

void GuardianEnemy::Smoke(bool& createf)
{
	smokepar->SetParF(1);
	smokepar->CreateParticle(createf, {Position.x, -32.f, Position.z});
	createf = false;
}


void GuardianEnemy::Update()
{
	state_guardian->Update(this);
	/*PosYMovingT++;
	Position.x =  -300.f;
	Position.z = 270.f;PosYMovingT++;
	Position.y=-20+sinf(3.14f * 2.f / 120.f * PosYMovingT) *-5.f;
	Rotation.x++;
	*/

	DamageTexDisplay();
	GuardianShotAttack::GetIns()->Upda();
	GuardianBomAttack::GetIns()->Upda();
	GuardianNAttack::GetIns()->Upda();
	HPFrameUpda();
	ParameterSet_Obj();
	//Move();
	smokepar->Upda_B();
}


void GuardianEnemy::AttackCoolTime()
{
}

void GuardianEnemy::FbxAnimationControl()
{
}


void GuardianEnemy::Death()
{
	isAlive = FALSE;
	alpha -= 0.02f;
	Task::GetIns()->SetGuardianDeathCount(1);
	SetColors({0.2f, 0.2f, 0.2f, alpha});
}

void GuardianEnemy::Draw()
{
	Draw_Obj();
	smokepar->Draw();
	GuardianShotAttack::GetIns()->Draw();
	GuardianBomAttack::GetIns()->Draw();
}

void GuardianEnemy::EnemyHPDraw()
{
	if (!isAlive)
	{
		return;
	}
	Player* l_player = PlayerControl::GetIns()->GetPlayer();

	if (Collision::GetLength(Position, l_player->GetPosition()) > 40) { return; }

	Sprite::PreDraw();
	for (int i = 0; i < HPFrame.size(); i++)
	{
		HPFrame[i]->Draw();
	}
	EnemyName->Draw();
	Sprite::PostDraw();
}
