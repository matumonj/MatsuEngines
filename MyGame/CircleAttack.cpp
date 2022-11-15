#include "CircleAttack.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"Nail.h"
#include"BossSpell.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"imgui.h"

CircleAttack::~CircleAttack()
{
	//delete ImpactAreaTex;
}

CircleAttack* CircleAttack::GetInstance()
{
	static CircleAttack instance;
	return &instance;
}

void CircleAttack::Initialize()
{
	Texture::LoadTexture(23, L"Resources/AOE.png");

	for (int i = 0; i < 2; i++)
	{
		ImpactAreaTex[i] = Texture::Create(23, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
		ImpactAreaTex[i]->CreateTexture();
		ImpactAreaTex[i]->SetAnchorPoint({0.5f, 0.5f});
	}

	Direction[NORTH] = {0.0f, 0.0f, 30.0f};
	Direction[SOUTH] = {0.0f, 0.0f, -30.0f};
	Direction[EAST] = {30.0f, 0.0f, 0.0f};
	Direction[WEST] = {-30.0f, 0.0f, 0.0f};
}

void CircleAttack::ActionJudg()
{
	switch (fase)
	{
	case FASENON:
		break;
	case FASEONE:
		//釘刺さります
		PierceNail();
		break;
	case FASETWO:
		//ダメージエリアの円広がります
		DamageAreaTexSet();
		break;
	case FASETHREE:
		//地面から釘出てきます
		ProtrudeNail();
		break;
	case FASEFOUR:
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
	for (int i = 0; i < 2; i++)
	{
		ImpactAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		ImpactAreaTex[i]->SetScale({CircleSize.x, CircleSize.y, 3.0f});
		ImpactAreaTex[i]->SetRotation({90.0f, 0.0f, rotY});
		ImpactAreaTex[i]->SetColor({1.0f, 1.0f, 1.0f, TexAlpha});
	}
	ImpactAreaTex[0]->SetPosition({Direction[Area1].x, -18, Direction[Area1].z});
	ImpactAreaTex[1]->SetPosition({Direction[Area2].x, -18, Direction[Area2].z});

	//釘オブジェの更新
	for (int i = 0; i < NailObj.size(); i++)
	{
		NailObj[i]->SetScale({3.0f, 3.0f, 3.0f});
		NailObj[i]->Update({1.0f, 1.0f, 1.0f, 1.0f}, CameraControl::GetInstance()->GetCamera());
	}
	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void CircleAttack::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < 2; i++)
	{
		ImpactAreaTex[i]->Draw();
	}
	Texture::PostDraw();
	for (int i = 0; i < NailObj.size(); i++)
	{
		Object3d::PreDraw();
		NailObj[i]->Draw();
		Object3d::PostDraw();
	}
	if (fase == FASETHREE)
	{
		Nail::GetInstance()->Draw();
	}
}

void CircleAttack::CollisonNailPlayer()
{
	const int Damage = 20;

	if (Collision::GetLength(NailObj[0]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
	}
	if (Collision::GetLength(NailObj[1]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
	}
}

void CircleAttack::PierceNail()
{
	TexAlpha = 1.0f;
	BossSpell::GetInstance()->SetStartSpell(BossSpell::CIRCLE, true);
	if (BossSpell::GetInstance()->GetEndSpell(BossSpell::CIRCLE))
	{
		fase = FASETWO;
	}
	//釘生成
	NailObj.resize(2);
	for (int i = 0; i < NailObj.size(); i++)
	{
		NailObj[i] = std::make_unique<Object3d>();
		//フィールドにモデル割り当て
		NailObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		NailObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
	}
	//ビルボード切る(標準がtrueなので、、)
	ImpactAreaTex[0]->SetBillboard(false);
	ImpactAreaTex[1]->SetBillboard(false);

	//Direction.y->釘のY座標と同じ
	if (Direction[Area1].y > -17.0f)
	{
		Direction[Area1].y--;
		Direction[Area2].y--;
	}
	//座標合わせる
	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);
}

void CircleAttack::DamageAreaTexSet()
{
	const float EaseC = 0.01f;
	const XMFLOAT2 DamageAreaTex_Max = {8.0f, 8.0f};
	if (Nail::GetInstance()->GetEndAction_Circle())
	{
		Nail::GetInstance()->SetEndAction_Circle(false);
	}
	//ダメージエリアの円ひろがる
	CircleAreaTime += EaseC;
	CircleSize.x = Easing::EaseOut(CircleAreaTime, 0.0f, DamageAreaTex_Max.x);
	CircleSize.y = Easing::EaseOut(CircleAreaTime, 0.0f, DamageAreaTex_Max.y);
	AttackCount = 0;

	if (CircleAreaTime >= 1.0f)
	{
		fase = FASETHREE; //円の大きさが最大超えたら次へ
	}
}

void CircleAttack::ProtrudeNail()
{
	Nail::GetInstance()->CircleAttack(Area1, Area2);

	CollisonNailPlayer();

	CircleAreaTime = 0;
	Direction[Area1].y++;
	Direction[Area2].y++;

	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);
	TexAlpha -= 0.01f;
	if (TexAlpha <= 0.0f)
	{
		fase = FASEFOUR;
	}
}

void CircleAttack::EndAttackAction()
{
	TexAlpha = 0.5f;
	BossSpell::GetInstance()->SetEndSpell(BossSpell::CIRCLE, false);

	Direction[NORTH] = {0.0f, 0.0f, 30.0f};
	Direction[SOUTH] = {0.0f, 0.0f, -30.0f};
	Direction[EAST] = {30.0f, 0.0f, 0.0f};
	Direction[WEST] = {-30.0f, 0.0f, 0.0f};

	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);

	CircleSize = {0.0f, 0.0f};
	NailObj.clear();
}



void BomAttack::Init()
{
	Texture::LoadTexture(34, L"Resources/2d/BossAttackEffect/BomParticle.jpg");
	Texture::LoadTexture(35, L"Resources/2d/BossAttackEffect/BomDamageArea.jpg");
	_phase = Phase::PHASE_ONE;
	Phase_One();
}	

void BomAttack::Upda()
{
	for (int i = 0; i < BomParticleSize; i++)
	{
		bom_particle_[i].BomTex->SetBillboard(TRUE);
		bom_particle_[i].BomTex->SetPosition({40.0f,0.0f,40.0f});
		bom_particle_[i].BomTex->SetColor({ 1.0f, 1.0f, 1.0f, bom_particle_[i].TexAlpha });
		bom_particle_[i].BomTex->SetScale({ 2.0f,2.0f,1.0f });
		bom_particle_[i].BomTex->Update(CameraControl::GetInstance()->GetCamera());
	}

	if(Input::GetInstance()->TriggerButton(Input::X))
	{
	
	}
	Phase_Two();
	for(int i=0;i<meteoSize;i++)
	{
		MeteoRock[i]->SetScale({ 3.0f,3.0f,3.0f });
		MeteoRock[i]->SetPosition(MeteoPos[i]);
		MeteoRock[i]->Update({1,1,1,1},CameraControl::GetInstance()->GetCamera());

		MeteoDamageAreaTex[i]->SetScale(DAreaTexSize[i]);
		MeteoDamageAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
}

void BomAttack::Setting_DamageArea()
{
	damageAreaPos={
		{DamageArea::UP,{0.0f,0.0f,40.0f}},
		{DamageArea::BOTTOM,{0.0f,0.0f,-40.0f}},
		{DamageArea::LEFT,{-40.0f,0.0f,0.0f}},
		{DamageArea::RIGHT,{40.0f,0.0f,0.0f}},
	};

	MeteoDamageAreaTex[DamageArea::UP]->SetPosition({ damageAreaPos.at(DamageArea::UP) });
	MeteoDamageAreaTex[DamageArea::BOTTOM]->SetPosition({ damageAreaPos.at(DamageArea::BOTTOM) });
	MeteoDamageAreaTex[DamageArea::LEFT]->SetPosition({ damageAreaPos.at(DamageArea::LEFT) });
	MeteoDamageAreaTex[DamageArea::RIGHT]->SetPosition({ damageAreaPos.at(DamageArea::RIGHT) });
}

void BomAttack::Phase_One()
{

	Texture* l_bomtex[BomParticleSize]{};
	Texture* l_damareatex[meteoSize]{};
	for (int i = 0; i < BomParticleSize; i++)
	{
		l_bomtex[i] = Texture::Create(34, { 0,0,0 }, { 0,0,0 }, { 0,0,0,0 });
		bom_particle_[i].BomTex.reset(l_bomtex[i]);
		bom_particle_[i].BomTex->CreateTexture();
		bom_particle_[i].BomTex->SetAnchorPoint({ 0.5f, 0.5f });
		bom_particle_[i].TexAlpha = 1.0f;
	}
	for (int i = 0; i < meteoSize; i++)
	{
		MeteoRock[i] = std::make_unique<Object3d>();
		MeteoRock[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		MeteoRock[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD));
		
		l_damareatex[i] = Texture::Create(35, { 0,0,0 }, { 0,0,0 }, { 0,0,0,0 });
		MeteoDamageAreaTex[i].reset(l_damareatex[i]);
		MeteoDamageAreaTex[i]->CreateTexture();
		MeteoDamageAreaTex[i]->SetAnchorPoint({ 0.5f, 0.5f });
		MeteoDamageAreaTex[i]->SetRotation({ 90.0f,0.0f,0.0f });
	}

	Setting_DamageArea();

	_phase = Phase::PHASE_TWO;
}

void BomAttack::Phase_Two()
{
	constexpr  float MaxTexSize = 5.0f;
	MeteoPos[DamageArea::UP].x = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::UP).x);
	MeteoPos[DamageArea::BOTTOM].x = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::BOTTOM).x);
	MeteoPos[DamageArea::LEFT].x = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::LEFT).x);
	MeteoPos[DamageArea::RIGHT].x = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::RIGHT).x);

	MeteoPos[DamageArea::UP].z = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::UP).z);
	MeteoPos[DamageArea::BOTTOM].z = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::BOTTOM).z);
	MeteoPos[DamageArea::LEFT].z = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::LEFT).z);
	MeteoPos[DamageArea::RIGHT].z = Easing::EaseOut(MeteoFallT, 0.0f, damageAreaPos.at(DamageArea::RIGHT).z);

	for(int i=0;i<meteoSize;i++)
	{
		if(DAreaTexSize[0].x >= MaxTexSize)
		{
			MeteoPos[i].y -= 1.5f;
		}
		//エリア拡大
		DAreaTexSize[i].x += 0.1f;
		DAreaTexSize[i].y += 0.1f;

		//上限値
		DAreaTexSize[i].x = min(DAreaTexSize[i].x, MaxTexSize);
		DAreaTexSize[i].y = min(DAreaTexSize[i].y, MaxTexSize);
	}

}

void BomAttack::Draw()
{
	Texture::PreDraw();
	for(int i=0;i<meteoSize;i++)
	{
		MeteoDamageAreaTex[i]->Draw();
	}
	for (int i = 0; i < BomParticleSize; i++)
	{
		bom_particle_[i].BomTex->Draw();
	}

	Texture::PostDraw();
}
