#include "CircleAttack.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"Nail.h"
#include"BossSpell.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"imgui.h"

#include"EnemyControl.h"
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
		ImpactAreaTex[i] = Texture::Create(23, { 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		ImpactAreaTex[i]->CreateTexture();
		ImpactAreaTex[i]->SetAnchorPoint({ 0.5f, 0.5f });
	}

	Direction[NORTH] = { 0.0f, 0.0f, 30.0f };
	Direction[SOUTH] = { 0.0f, 0.0f, -30.0f };
	Direction[EAST] = { 30.0f, 0.0f, 0.0f };
	Direction[WEST] = { -30.0f, 0.0f, 0.0f };
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
		ImpactAreaTex[i]->SetScale({ CircleSize.x, CircleSize.y, 3.0f });
		ImpactAreaTex[i]->SetRotation({ 90.0f, 0.0f, rotY });
		ImpactAreaTex[i]->SetColor({ 1.0f, 1.0f, 1.0f, TexAlpha });
	}
	ImpactAreaTex[0]->SetPosition({ Direction[Area1].x, -18, Direction[Area1].z });
	ImpactAreaTex[1]->SetPosition({ Direction[Area2].x, -18, Direction[Area2].z });

	//釘オブジェの更新
	for (int i = 0; i < NailObj.size(); i++)
	{
		NailObj[i]->SetScale({ 3.0f, 3.0f, 3.0f });
		NailObj[i]->Update({ 1.0f, 1.0f, 1.0f, 1.0f }, CameraControl::GetInstance()->GetCamera());
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
	const XMFLOAT2 DamageAreaTex_Max = { 8.0f, 8.0f };
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

	Direction[NORTH] = { 0.0f, 0.0f, 30.0f };
	Direction[SOUTH] = { 0.0f, 0.0f, -30.0f };
	Direction[EAST] = { 30.0f, 0.0f, 0.0f };
	Direction[WEST] = { -30.0f, 0.0f, 0.0f };

	NailObj[0]->SetPosition(Direction[Area1]);
	NailObj[1]->SetPosition(Direction[Area2]);

	CircleSize = { 0.0f, 0.0f };
	NailObj.clear();
}



void BomAttack::Init()
{
	Texture::LoadTexture(34, L"Resources/2d/BossAttackEffect/BomParticle.png");
	Texture::LoadTexture(35, L"Resources/2d/BossAttackEffect/DamageArea.png");
	damageAreaPos = {
	{DamageArea::UP,{40.0f,FieldBottomPosY + 4.0f,0.0f}},
	{DamageArea::BOTTOM,{60.0f,FieldBottomPosY + 4.0f,0.0f}},
	{DamageArea::LEFT,{80.0f,FieldBottomPosY + 4.0f,0.0f}},
	{DamageArea::RIGHT,{100.0f,FieldBottomPosY + 4.0f,0.0f}},
	};
	Phase_One();
	for (int i = 0; i < BomParticleSize; i++) {
		for (int j = 0; j < meteoSize; j++) {
			bom_particle_[j][i].TexAlpha = 0.0f;
		}
	}
	
}
void BomAttack::Phase_One()
{
	MteoObjAlpha = 1.0f;
	Texture* l_bomtex[meteoSize][BomParticleSize]{};
	Texture* l_damareatex[meteoSize]{};

	//爆発パーティクル
	for (int i = 0; i < BomParticleSize; i++)
	{
		for (int j = 0; j < meteoSize; j++) {
			l_bomtex[j][i] = Texture::Create(34, { 0,0,0 }, { 0,0,0 }, { 0,0,0,0 });
			bom_particle_[j][i].BomTex.reset(l_bomtex[j][i]);
			bom_particle_[j][i].BomTex->CreateTexture();
			bom_particle_[j][i].BomTex->SetAnchorPoint({ 0.5f, 0.5f });
			bom_particle_[j][i].TexAlpha = 1.0f;
		}
	}


	for (int i = 0; i < BomParticleSize; i++) {
		for (int j= 0; j< meteoSize; j++) {
			bom_particle_[j][i].CenterPos.y = 10.0f;
			}
	}
}
using namespace DirectX;

void BomAttack::Setting_DamageArea()
{
	float BossRotY= EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetRotation().y;
	 BossPos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();

	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(BossRotY+140));
	move = XMVector3TransformNormal(move, matRot);
				CenterPosi[0].x = BossPos.x + move.m128_f32[0] * 10;
				
				CenterPosi[0].z= BossPos.z + move.m128_f32[2]* 10;
			
				CenterPosi[1].x = BossPos.x + move.m128_f32[0] * 150.0f;
				CenterPosi[1].z = BossPos.z + move.m128_f32[2] * 150.0f;

				CenterPosi[2].x = BossPos.x + move.m128_f32[0] * 300;
				CenterPosi[2].z = BossPos.z + move.m128_f32[2] * 300;

				CenterPosi[3].x = BossPos.x + move.m128_f32[0] * 450;
				CenterPosi[3].z = BossPos.z + move.m128_f32[2] * 450;

				CenterPosi[4].x = BossPos.x + move.m128_f32[0] * 600;
				CenterPosi[4].z = BossPos.z + move.m128_f32[2] * 600;
				
				CenterPosi[5].x = BossPos.x + move.m128_f32[0] * 750;
				CenterPosi[5].z = BossPos.z + move.m128_f32[2] * 750;
				
				CenterPosi[6].x = BossPos.x + move.m128_f32[0] * 900;
				CenterPosi[6].z = BossPos.z + move.m128_f32[2] * 900;
				for (int i = 0; i < meteoSize; i++) {
					CenterPosi[i].y = 10.0f;
}
}

void BomAttack::ParamSet()
{
	for (int i = 0; i < BomParticleSize; i++)
	{
		for (int j = 0; j < meteoSize; j++) {
			bom_particle_[j][i].BomTex->SetBillboard(TRUE);
			bom_particle_[j][i].BomTex->SetPosition(bom_particle_[j][i].TexPos);
			bom_particle_[j][i].BomTex->SetColor({ 1.0f, 0.8f, 0.8f,bom_particle_[j][i].TexAlpha});
			bom_particle_[j][i].BomTex->SetScale(bom_particle_[j][i].TexScl);
			bom_particle_[j][i].BomTex->Update(CameraControl::GetInstance()->GetCamera());
		}
	}

	
}

void BomAttack::BomParticleUpda()
{
	//ダメージ範囲設定
	Setting_DamageArea();

	if (_Bphase == bParticlePhase::SETPOSITION) {
	
		for (int i = 0; i < BomParticleSize; i++) {
			for (int j = 0; j < meteoSize; j++) {
			bom_particle_[j][i].TexScl = { 0.0f,0.0f,0.0f };
				bom_particle_[j][i].TexAlpha = 1.0f;
				bom_particle_[j][i].Speed = 0.0f;
				
				bom_particle_[j][i].Angle = rand() % 360;
			}
		bom_particle_[0][i].TexPos = CenterPosi[0];
		bom_particle_[1][i].TexPos = CenterPosi[1];
		bom_particle_[2][i].TexPos = CenterPosi[2];
		bom_particle_[3][i].TexPos = CenterPosi[3];
		bom_particle_[4][i].TexPos = CenterPosi[4];
		bom_particle_[5][i].TexPos = CenterPosi[5];
		bom_particle_[6][i].TexPos = CenterPosi[6];
		}

		_Bphase = bParticlePhase::UPDATE;
	}

	if (_Bphase == bParticlePhase::UPDATE) {
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetFbxTime() < 1085.00f / 60.00f) {
			AttackJudg = false;
		}
		for (int i = 0; i < BomParticleSize; i++)
		{
			bom_particle_[0][i].TexScl.x += 0.1f;
			bom_particle_[0][i].TexScl.y += 0.1f;
			bom_particle_[0][i].TexPos.x += bom_particle_[0][i].Speed * cos(bom_particle_[0][i].Angle);//360度に広がるようにする
			bom_particle_[0][i].TexPos.y += bom_particle_[0][i].Speed * sin(bom_particle_[0][i].Angle);//360度に広がるようにする
			bom_particle_[0][i].Speed += 0.002f;//徐々にスピードを速く
			bom_particle_[0][i].TexAlpha -= 0.01f;

			for (int j = 1; j < meteoSize; j++) {
				if (bom_particle_[j - 1][i].TexScl.x > 2.0f) {
					bom_particle_[j][i].TexScl.x += 0.1f;
					bom_particle_[j][i].TexScl.y += 0.1f;
					bom_particle_[j][i].TexPos.x += bom_particle_[j][i].Speed * cos(bom_particle_[j][i].Angle);//360度に広がるようにする
					bom_particle_[j][i].TexPos.y += bom_particle_[j][i].Speed * sin(bom_particle_[j][i].Angle);//360度に広がるようにする
					bom_particle_[j][i].Speed += 0.002f;//徐々にスピードを速く
					bom_particle_[j][i].TexAlpha -= 0.01f;
				}
				bom_particle_[j][i].TexScl.x = min(bom_particle_[j][i].TexScl.x, 4.0f);
				bom_particle_[j][i].TexScl.y = min(bom_particle_[j][i].TexScl.y, 4.0f);
			}
		}
	}
}

void BomAttack::Upda()
{

	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetFbxTime()>1085.00f/60.00f&&!AttackJudg) {
		_Bphase = bParticlePhase::SETPOSITION;
		//Phase_One();
		AttackJudg = true;
	}


	BomParticleUpda();
	ParamSet();



}

void BomAttack::Phase_Two()
{
	
}

void BomAttack::Phase_Three()
{
	constexpr  float MaxTexSize = 9.0f;
	AttackJudg = false;

	}

void BomAttack::Phase_End(int indexMeteo/*meteosize*/, int indexPar/*parSize*/)
{
	bom_particle_[indexMeteo][indexPar].BomTex.reset(nullptr);
	
}
void BomAttack::Draw()
{
	//ダメージエリアとパーティクル描画
	Texture::PreDraw();
	
	for (int i = 0; i < BomParticleSize; i++)
	{
		for (int j = 0; j < meteoSize; j++) {
			bom_particle_[j][i].BomTex->Draw();
		}
	}

	Texture::PostDraw();

	ImGui::Begin("w");
	ImGui::SliderFloat("angle", &attackangle, 0, 360);
	ImGui::Text("%f", bom_particle_[DamageArea::LEFT][0].TexAlpha);
	ImGui::Text("CenPos%f", CenterPosi[0].z);
	ImGui::Text("CenPos2%f", CenterPosi[1].z);
	//ImGui::Text("CenPs%f", CenterPosi[2].z);
	ImGui::Text("Phase%f",BossPos.z);
	ImGui::End();
}
