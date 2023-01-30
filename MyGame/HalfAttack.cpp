#include "HalfAttack.h"
#include"EnemyControl.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include "MobEnemy.h"

HalfAttack::~HalfAttack()
{
	//delete HalfAreaTex;
}

HalfAttack* HalfAttack::GetInstance()
{
	static HalfAttack instance;
	return &instance;
	
}

void HalfAttack::Init()
{
	SummonInit();
	phase = PHASEONE;

	summonEnemyCreate = true;
	
}

bool HalfAttack::SummonEnemy()
{
	if (phase == PHASETWO)
	{
		return true;
	}
	return false;
}
void (HalfAttack::* HalfAttack::actionTable[])() = {
	nullptr,
	&HalfAttack::BossLeaveGround,
	&HalfAttack::SummonUpdate,
	&HalfAttack::BossReturnGround,
	&HalfAttack::SummonAttackEnd,
};
void HalfAttack::Upda()
{
	//ボスのインスタンス持ってくる
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	if (boss == nullptr)return;

	(this->*actionTable[phase])();
}

void HalfAttack::SummonInit()
{
	//ボスが召喚するザコ敵
	for (int i = 0; i < EnemySize; i++)
	{
		SummonEnemys[i] = std::make_unique<MobEnemy>();
		SummonEnemys[i]->Initialize();
		SummonEnemys[i]->SetPosition({ 0, -20, 20 });

	}
	//敵生成フラグ
	summonEnemyCreate = false;
	//敵死亡フラグ
	SummonEnemysDeath = false;
	//敵登場済みフラグ
	SummonEnemysApper = false;

	SummonEPos = { 1.f, 1.f, 1.f };

	Shieldalpha = 0.0f;

	//盾テクスチャ
	Texture* l_shield[2];
	Texture::LoadTexture(101, L"Resources/2d/attackEffect/ma1b-skyblue.png");
	for (int i = 0; i < 2; i++)
	{
		l_shield[i] = Texture::Create(101, { 1, 1, 1 }, { 0, 0, 0 }, { 1, 1, 1, 1 });
		ShieldTex[i].reset(l_shield[i]);
		ShieldTex[i]->CreateTexture();
		ShieldTex[i]->SetAnchorPoint({ 0.5f, 0.5f });
		ShieldTex[i]->SetRotation({ 90, 0, 0 });
	}
	
}

void HalfAttack::BossLeaveGround()
{
	/*攻撃内容の処理なので後で攻撃専用のクラスに移す*/
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	//ボス座標
	XMFLOAT3 bosspos=boss->GetPosition();

	const double JumpEndTime = 1.2;
	const float JumpSpeed = 4.f;

	if (boss->GetNowMotion() != boss->BJUMP)return;

		if(boss->GetAnimationTime()>JumpEndTime)
		{
			bosspos.y+=JumpSpeed;
		}

	//ボスが一定以上上に移動したら
	bool nextphase = bosspos.y > 80.f;

	boss->SetPosition(bosspos);

	if(nextphase)
	{
		phase = PHASETWO;
	}
}

void HalfAttack::SummonUpdate()
{
	//カメラのインスタンス持ってくる
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	/*攻撃内容の処理なので後で攻撃専用のクラスに移す*/
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();

	
	SummonEPos.y += 0.1f; //徐々に上に

	for (int i = 0; i < EnemySize; i++)
	{
		if (SummonEnemys[i] == nullptr)
			{
				continue;
			}
		//登場し切るまで
			if (SummonEPos.y < 18.0f)
			{
				ShieldTexPos[0] = { boss->GetPosition().x + 30.f,19.f,boss->GetPosition().z };
				ShieldTexPos[1] = { boss->GetPosition().x -30.f,19.f,boss->GetPosition().z };
				//敵がプレイヤー座標まで現れたら
				Shieldalpha += 0.01f;
				//下から上に出てくる際は動き止めておく
				SummonEnemys[i]->SetMoveStop(true);
				SummonEnemys[0]->SetPosition({ShieldTexPos[0].x, SummonEPos.y,ShieldTexPos[0].z });
				SummonEnemys[1]->SetPosition({ ShieldTexPos[1].x, SummonEPos.y,ShieldTexPos[1].z });
			} else
			{
				Shieldalpha -= 0.05f;
				
				//動き止めていたのを解除
				SummonEnemys[i]->SetMoveStop(false);
			}
			//更新
			SummonEnemys[i]->SetColors({ 1.0f, 0.2f, 0.2f, 1.0f });

			SummonEnemys[i]->Update();
		}
	

	//ザコ敵両方とも倒したら
	if (SummonEnemys[0] == nullptr && SummonEnemys[1] == nullptr)
	{
		phase = PHASETHREE;
		//盾テクスチャ消す
		SummonEnemysDeath = true;
	}
	//盾テクスチャのアルファ値が一定以下なったらテクスチャインスタンスは消す
	if (Shieldalpha < -1.0f)
	{
		for (int i = 0; i < 2; i++)
		{
			Destroy_unique(ShieldTex[i]);
		}
	}
	//敵登場済みのフラグ=敵がプレイヤーのY座標まで上がってきたら
	SummonEnemysApper = SummonEPos.y >= 10.0f;

	for (int i = 0; i < 2; i++)
	{
		if (ShieldTex[i] == nullptr)
		{
			continue;
		}
		//テクスチャ回す
		ShieldRot++;
		ShieldTex[i]->SetBillboard(false);
		ShieldTex[i]->SetColor({ 1.0f, 1.0f, 1.0f, Shieldalpha });
		ShieldTex[i]->SetPosition({ ShieldTexPos[i] });
		ShieldTex[i]->SetScale({ 5.0f, 5.0f, 1.0f });
		ShieldTex[i]->SetRotation({ 90.f,0.f,ShieldRot });
		ShieldTex[i]->Update(camera);
	}

	//ザコ敵の開放処理
	for (int i = 0; i < 2; i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		if (SummonEnemys[i]->GetObjAlpha() <= 0)
		{
			Destroy_unique(SummonEnemys[i]);
		}
	}

	//盾テクスチャのアルファ値の上限と下限
	Shieldalpha = min(Shieldalpha, 1);
	Shieldalpha = max(Shieldalpha, 0);
	SummonEPos.y = min(SummonEPos.y, 18);
}

void HalfAttack::BossReturnGround()
{
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	XMFLOAT3 bosspos = boss->GetPosition();

	bosspos.y--;
	
	if(bosspos.y<14.7f)
	{
		phase = PHASEFOUR;
	}
	boss->SetPosition(bosspos);

}


void HalfAttack::Draw_SummonEnemyHP()
{
	for (int i = 0; i < SummonEnemys.size(); i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		if (SummonEnemys[i]->GetPosition().y > 17.f) {
			SummonEnemys[i]->EnemyHPDraw();
		}
	}
}

void HalfAttack::Draw()
{
	//召喚敵の描画
	Object3d::PreDraw();
	for (int i = 0; i < EnemySize; i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		SummonEnemys[i]->Draw();
	}
	Object3d::PostDraw();
	//ボスの貼るシールドテクスチャ
	Texture::PreDraw();

	for (int i = 0; i < 2; i++)
	{
		ShieldTex[i]->Draw();
	}

	Texture::PostDraw();
}

void HalfAttack::SummonEnemyResetParam()
{
	for (int i = 0; i < EnemySize; i++)
	{
		SummonEnemys[i].reset(new MobEnemy());
		SummonEnemys[i]->Initialize();

		SummonEnemys[i]->SetPosition({ 0, -20, 20 });
	}
	//敵生成フラグ
	summonEnemyCreate = false;
	//敵死亡フラグ
	SummonEnemysDeath = false;
	//敵登場済みフラグ
	SummonEnemysApper = false;

	SummonEPos = { 1, 1, 1 };
	Shieldalpha = 0.0f;

	phase = PHASEONE;
}

void HalfAttack::SummonAttackEnd()
{
	
}
