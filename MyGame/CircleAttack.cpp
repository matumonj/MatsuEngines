#include "CircleAttack.h"
#include"mHelper.h"
#include"Nail.h"
#include "CameraControl.h"
#include"Collision.h"
#include "Enemy.h"
#include "EnemyControl.h"
#include"imgui.h"
#include "PlayerControl.h"


CircleAttack::~CircleAttack()
{
	//delete ImpactAreaTex;
}

CircleAttack* CircleAttack::GetIns()
{
	static CircleAttack instance;
	return &instance;
}

void CircleAttack::Init()
{
	Texture::LoadTexture(23, L"Resources/2d/icon/enemyicon.png");
	Texture::LoadTexture(24, L"Resources/2d/gauge/bosshp.png");
	ImpactAreaTex = Texture::Create(23);
	ImpactAreaTex->CreateTexture();
	ImpactAreaTex->SetAnchorPoint({0.5f, 0.5f});

	AllAreaTex.reset(Texture::Create(24));
	AllAreaTex->CreateTexture();
	AllAreaTex->SetAnchorPoint({0.5f, 0.5f});

	RingObj.reset(new Object3d());
	RingObj->Initialize(CameraControl::GetIns()->GetCamera());
	RingObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::RING));

	_phase = PHASE_NON;
	CircleSize = {0, 0};
	TexAlpha = 0.0f;
}

void CircleAttack::Upda()
{
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
	switch (_phase)
	{
	case PHASE_NON:
		break;
	case PHASE_ONE:
		DamageAreaTexSet();
	//釘刺さります
		break;
	case PHASE_TWO:
		//ダメージエリアの円広がります

		break;
	case PHASE_THREE:
		//地面から釘出てきます
		break;
	case PHASE_FOUR:

		//攻撃終了です
		EndAttackAction();
		break;
	default:
		break;
	}
	rotY += 1.0f;
	if (rotY > 360.0f)
	{
		rotY = 0.0f;
	}
	//ダメージエリアテクスチャの各種パラメータ
	DamageAreaTexSet();
	ImpactAreaTex->Update(CameraControl::GetIns()->GetCamera());
	ImpactAreaTex->SetScale({CircleSize.x, CircleSize.y, 3.0f});
	ImpactAreaTex->SetBillboard(false);
	ImpactAreaTex->SetRotation({90.0f, 0.0f, rotY});
	ImpactAreaTex->SetColor({1.0f, 0.3f, 0.3f, TexAlpha});


	AllAreaTex->Update(CameraControl::GetIns()->GetCamera());
	AllAreaTex->SetScale({100.f, 100.f, 3.0f});
	AllAreaTex->SetBillboard(false);
	AllAreaTex->SetRotation({90.0f, 0.0f, 0.f});
	AllAreaTex->SetColor({0.7f, 0.7f, 0.7f, 0.7f});

	RingObj->SetRotation({0.f, 0.f, 0.f});
	RingObj->SetPosition({boss->GetPosition().x, 18, boss->GetPosition().z});
	RingObj->SetScale({CircleSize.x * 2.f, 5.f, CircleSize.y * 2.f});
	RingObj->SetUVf(true);
	RingObj->SetColor({1.f, 0.3f, 0.3f, TexAlpha});
	RingObj->Update(CameraControl::GetIns()->GetCamera());
	ImpactAreaTex->SetPosition({boss->GetPosition().x, 18, boss->GetPosition().z});
	AllAreaTex->SetPosition({boss->GetPosition().x, 18, boss->GetPosition().z});

	//釘オブジェの更新

	Nail::GetIns()->Update();
}


void CircleAttack::Draw()
{
	Texture::PreDraw();

	//AllAreaTex->Draw();
	//ImpactAreaTex->Draw();

	Texture::PostDraw();
	Object3d::PreDraw();
	//RingObj->Draw();
	Object3d::PostDraw();
}

void CircleAttack::CollisonNailPlayer()
{
	const int Damage = 20;
}


void CircleAttack::DamageAreaTexSet()
{
	const float EaseC = 0.01f;
	const XMFLOAT2 DamageAreaTex_Max = {10.0f, 10.0f};

	//ダメージエリアの円ひろがる
	CircleAreaTime += EaseC;

	if (CircleAreaTime <= 1.0f)
	{
		TexAlpha = Easing::EaseOut(CircleAreaTime, 1.0f, 0.f);
		CircleSize.x += 0.2f;
		CircleSize.y += 0.2f;
	}
	if (CircleAreaTime >= 1.0f)
	{
		CircleSize = {0.f, 0.f};
		CircleAreaTime = 0.f; //円の大きさが最大超えたら次へ
	}
}


void CircleAttack::EndAttackAction()
{
	TexAlpha = 0.5f;
	CircleSize = {0.0f, 0.0f};
}
