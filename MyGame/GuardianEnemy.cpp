#include "GuardianEnemy.h"
#include "PlayerControl.h"
#include "CameraControl.h"
#include "ImageManager.h"
#include "mHelper.h"

GuardianEnemy::GuardianEnemy()
{
	state_guardian->Initialize(this);
}

GuardianEnemy::~GuardianEnemy()
{
}


void GuardianEnemy::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_Object->SetModel(Model::CreateFromOBJ("Guardian"));

	MaxHP = 100;

	EnemyHP = MaxHP;
	//パラメータのセット
	Rotation = {114.0f, 118.0f, 165.0f};
	Scale = {1.f, 1.f, 1.f};

	//コライダー周り
	radius_adjustment = 0;
	SetCollider();


	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	HPFrame[0].reset(l_frame1);
	HPFrame[1].reset(l_frame2);
	HPFrame[2].reset(l_frame3);
	HPFrame[3].reset(l_frame4);
	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->SetAnchorPoint({0.0f, 0.0f});
	}
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;

	//FBX切り替わりのタイム指定
	NormalAttackTime = 0.9f;
	DeathTime = 6.9f;
	Position = {-300.f, -50.f, 270.f};
	GuardianBomAttack::GetIns()->TexSet();

	Color = {1, 1, 1, 1};
}


void GuardianEnemy::HPFrameScaling()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	XMVECTOR tex2DPos[4];
	for (int i = 0; i < 4; i++)
	{
		tex2DPos[i] = {Position.x, Position.y, Position.z};
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetProjectionMatrix(), true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewPort(), false);

		HPFrame[i]->SetPosition({tex2DPos[i].m128_f32[0] - 80.0f, tex2DPos[i].m128_f32[1]});
	}
	if (RecvDamagef)
	{
		FrameScalingETime_Inner = 0.0f;
		if (!InnerFrameScalingF)
		{
			OldFrameX_Inner = OldFrameX;
		}
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
		FrameScalingETime += 0.05f;
		FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);

		if (FrameScalingETime >= 1.0f)
		{
			InnerFrameScalingF = true;
			RecvDamagef = false;
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;

		FrameScalingETime = 0.0f;
	}

	if (InnerFrameScalingF)
	{
		FrameScalingETime_Inner += 0.02f;
		FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, NowFrameX);
		if (FrameScalingETime_Inner >= 1.0f)
		{
			InnerFrameScalingF = false;
		}
	}
	else
	{
		FrameScalingETime_Inner = 0.0f;
	}
	HPFrame[3]->SetSize({FrameScl.x, 15});
	HPFrame[2]->SetSize({FrameScl_Inner.x, 15.0f});
	HPFrame[1]->SetSize({200.0f, 15.0f});
	HPFrame[0]->SetSize({200.0f, 15.0f});
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


	GuardianBomAttack::GetIns()->Upda();
	HPFrameScaling();
	ParameterSet_Obj();
}


void GuardianEnemy::AttackCoolTime()
{
}

void GuardianEnemy::FbxAnimationControl()
{
}


void GuardianEnemy::Death()
{
}

void GuardianEnemy::Draw()
{
	Draw_Obj();
	GuardianBomAttack::GetIns()->Draw();
}

void GuardianEnemy::EnemyHPDraw()
{
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();

	if (Collision::GetLength(Position, l_player->GetPosition()) > 40) { return; }

	Sprite::PreDraw();
	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->Draw();
	}
	Sprite::PostDraw();
}
