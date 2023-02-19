#include "FrontCircleAttack.h"

#include <algorithm>

#include"EnemyControl.h"
#include"CameraControl.h"
#include "PlayerControl.h"

#include"mHelper.h"
FrontCircleAttack* FrontCircleAttack::GetIns()
{
	static FrontCircleAttack instance;

	return &instance;
}

void FrontCircleAttack::Init()
{
	Texture::LoadTexture(26, L"Resources/2d/icon/missileicon.png");
	
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	for (auto i = 0; i < nail_objses_.size(); i++) {
		//ミサイル爆発時のエフェクト
		nail_objses_[i] .PireEffect= std::make_unique<Particle>();
		nail_objses_[i].PireEffect->Init(Particle::SMOKE);
		nail_objses_[i].PireEffect->SetParScl({ 8.f, 8.f });
		nail_objses_[i].PireEffect->SetParColor({ 1.f, 1.f, 1.f, 1.f });

		//テクスチャ
		nail_objses_[i].AOETexs.reset(Texture::Create(26));
		nail_objses_[i].AOETexs->CreateTexture();
		nail_objses_[i].AOETexs->SetAnchorPoint({ 0.5f,0.5f });
		//オブジェ
		nail_objses_[i].Obj.reset(new Object3d());
		nail_objses_[i].Obj->Initialize(camera);
		nail_objses_[i].Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
		nail_objses_[i].TexAlpha = 0.f;
	}
	_phase = PHASE_NON;
}


void (FrontCircleAttack::* FrontCircleAttack::actionTable[])() = {
	&FrontCircleAttack::NonInit,//待機
	&FrontCircleAttack::SetDamageArea,//エリア指定
	&FrontCircleAttack::PireNail,//釘突き出す
	&FrontCircleAttack::DestNail,//引っ込む
	&FrontCircleAttack::AttackEnd//終了
};

void FrontCircleAttack::Upda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//フェーズごとの処理
	(this->*actionTable[_phase])();

	for (auto i = 0; i < nail_objses_.size(); i++) {
		if (nail_objses_[i].AOETexs != nullptr) {
			//テクスチャ
			nail_objses_[i].TexRotY++;
			nail_objses_[i].AOETexs->SetBillboard(false);
			nail_objses_[i].AOETexs->SetPosition({ nail_objses_[i].TexPos.x,nail_objses_[i].TexPos.y + 4.f,nail_objses_[i].TexPos.z });
			nail_objses_[i].AOETexs->SetRotation({ 90.f,0.f,nail_objses_[i].TexRotY});
			nail_objses_[i].AOETexs->SetScale(nail_objses_[i].TexScl);
			nail_objses_[i].AOETexs->SetColor({ 1.f,1.f,1.f,nail_objses_[i].TexAlpha });
			nail_objses_[i].AOETexs->Update(camera);
		}
		//オブジェ
		if (nail_objses_[i].Obj != nullptr) {
			nail_objses_[i].Obj->SetRotation({ 180.f,0.f,0.f });
			nail_objses_[i].Obj->SetPosition(nail_objses_[i].ObjPos);
			nail_objses_[i].Obj->SetScale({ 8.f,8.f,8.f });
			nail_objses_[i].Obj->SetUVf(true);
			nail_objses_[i].Obj->Update(camera);
		}

		//pa-texikuru
		if (nail_objses_[i].PireEffect != nullptr) {
			nail_objses_[i].PireEffect->Upda_B(false);
		}

		nail_objses_[i].TexAlpha = std::clamp(nail_objses_[i].TexAlpha, 0.f, 1.f);
	}
}
#include "imgui.h"
void FrontCircleAttack::Draw()
{
	//テクスチャ
	Texture::PreDraw();
	for (auto i = 0; i < nail_objses_.size(); i++) {
		if (nail_objses_[i].AOETexs == nullptr)continue;
		nail_objses_[i].AOETexs->Draw();
	}
	Texture::PostDraw();

	//釘オブジェ
	Object3d::PreDraw();
	if (_phase != PHASE_FOUR && _phase != PHASE_NON) {
		for (auto i = 0; i < nail_objses_.size(); i++) {
			if (nail_objses_[i].Obj == nullptr)continue;
			nail_objses_[i].Obj->Draw();
		}
	}
	Object3d::PostDraw();

	//パーティクルエフェクト
	for (auto i = 0; i < nail_objses_.size(); i++) {
		if (nail_objses_[i].PireEffect == nullptr)continue;
		nail_objses_[i].PireEffect->Draw();
	}
}

void FrontCircleAttack::NonInit()
{
	for (auto i = 0; i < nail_objses_.size(); i++)
	{
		nail_objses_[i].TexAlpha = 0.f;
		nail_objses_[i].PosYMovingEaseT = 0.f;
		nail_objses_[i].TexSclingEaseT = 0.f;
	}
}


void FrontCircleAttack::Finalize()
{
	//開放処理
	for(auto i=0;i<nail_objses_.size();i++)
	{
		Destroy_unique(nail_objses_[i].PireEffect);
		Destroy_unique(nail_objses_[i].AOETexs);
		Destroy_unique(nail_objses_[i].Obj);
	}
}

void FrontCircleAttack::SetDamageArea()
{
	//プレイヤー座標
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//ボス座標
	XMFLOAT3 bpos = { EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition() };

	//アルファ増減値
	constexpr float AlphaVal = 0.02f;

	//最初は[0]要素参照
	float prevAlpha = nail_objses_[0].TexAlpha;

	if (nail_objses_[0].TexAlpha == 0.f) {
		//テクスチャ、オブジェ座標セット
		nail_objses_[0].TexPos = { ppos.x,bpos.y,ppos.z };
		nail_objses_[0].ObjPos = { nail_objses_[0].TexPos.x,NailMinPosY,nail_objses_[0].TexPos.z };
	}
	
		for (auto i = 1; i < nail_objses_.size(); i++) {
		//テクスチャランダム
		if (nail_objses_[i].TexAlpha> 0.f)continue;
		nail_objses_[i].TexPos = { float(rand() % 100 - 50)*1.2f,bpos.y, float(rand() % 100 - 50)*1.2f };
		nail_objses_[i].ObjPos = { nail_objses_[i].TexPos.x,-20.f,nail_objses_[i].TexPos.z};

	}
		nail_objses_[0].TexScl = { 8.f,8.f,2.f };
	//[0]アルファ値加算
	nail_objses_[0].TexAlpha+= AlphaVal;

	//一つ前のテクスチャアルファ値が0.6f上回ったら次のテクスチャのアルファ値上げる
	for (auto i = 1; i < nail_objses_.size(); i++)
	{
		if (prevAlpha >= 0.6f)
		{
			//テクスチャ広げる
			nail_objses_[i].TexSclingEaseT += 0.02f;
			nail_objses_[i].TexScl.x = Easing::EaseOut(nail_objses_[i].TexSclingEaseT, 0.f, 8.f);
			nail_objses_[i].TexScl.y = Easing::EaseOut(nail_objses_[i].TexSclingEaseT, 0.f, 8.f);

			//最大値と最小値
			nail_objses_[i].TexSclingEaseT = std::clamp(nail_objses_[i].TexSclingEaseT, 0.f, 1.f);

			//アルファ値上げる
			nail_objses_[i].TexAlpha += AlphaVal;
			//代入してる値置き換え
			prevAlpha = nail_objses_[i].TexAlpha;
		}
	}

	//次のフェーズへ
	if(nail_objses_[nail_objses_.size()-1].TexAlpha>=1.f)
	{
		for (auto i = 0; i < nail_objses_.size(); i++)
		{
			nail_objses_[i].PireEffect->SetParF(1);
			nail_objses_[i].PireEffect->CreateParticle(true, { nail_objses_[i].ObjPos.x, 10.f,nail_objses_[i].ObjPos.z });
		}
		_phase = Phase::PHASE_TWO;
	}

	
}
void FrontCircleAttack::PireNail()
{
	//アルファ値の増減
	constexpr float l_TexAlphaFeedVal = 0.03f;
	
	for(auto i = 0; i < nail_objses_.size(); i++)
	{
		//イージング進める
		nail_objses_[i].PosYMovingEaseT += l_TexAlphaFeedVal;
		//釘突き出す
		if(nail_objses_[i].PosYMovingEaseT<=1.f)
		nail_objses_[i].ObjPos.y = Easing::EaseOut(nail_objses_[i].PosYMovingEaseT, NailMinPosY, NailMaxPosY);
	}

	//釘とあたったらダメージ
	if(ColNail_Player()==true)
	{
		PlayerControl::GetIns()->GetPlayer()->RecvDamage(Damage);
	}

	//次のフェーズへ
	if (nail_objses_[0].PosYMovingEaseT >= 2.f) {
		_phase = PHASE_THREE;
	}

}

void FrontCircleAttack::DestNail()
{
	//釘位置が下るスピード
	constexpr float l_PosYMovingVal = 0.05f;
	//テクスチャフェード値
	constexpr float l_TexAlphaFeedVal = 0.02f;

	for (auto i = 0; i < nail_objses_.size(); i++)
	{
		nail_objses_[i].PosYMovingEaseT -= l_PosYMovingVal;

		//釘下げる
		if (nail_objses_[i].PosYMovingEaseT >=0.f&& nail_objses_[i].PosYMovingEaseT<=1.f)
			nail_objses_[i].ObjPos.y = Easing::EaseOut(nail_objses_[i].PosYMovingEaseT, NailMinPosY, NailMaxPosY);

		//テクスチャ薄く
		nail_objses_[i].TexAlpha = Easing::EaseOut(nail_objses_[i].TexAlpha, 0.f, 1.f);
	}
	//次のフェーズへ
	if (nail_objses_[0].PosYMovingEaseT <= 0.f)
		_phase = PHASE_FOUR;

}

void FrontCircleAttack::AttackEnd()
{
	for (auto i = 0; i < nail_objses_.size(); i++)
	{
		nail_objses_[i].TexAlpha = 0.f;
		nail_objses_[i].PosYMovingEaseT = 0.f;
		nail_objses_[i].TexSclingEaseT = 0.f;
	}
	_phase = PHASE_NON;
}


bool FrontCircleAttack::ColNail_Player()
{
	//プレイヤー座標
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	for(auto i=0;i<nail_objses_.size();i++)
	{
		bool L_colJudg=Collision::GetLength(ppos, nail_objses_[i].ObjPos) < 20.f;

		if(L_colJudg)
		{
			return true;
		}
	}
	return false;
}


