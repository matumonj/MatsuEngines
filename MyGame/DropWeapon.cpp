#include "DropWeapon.h"
#include"mHelper.h"
#include"CameraControl.h"
#include"PlayerControl.h"

DropWeapon* DropWeapon::GtIns()
{
	static DropWeapon ins;
	return &ins;
}

void DropWeapon::Init()
{
	Sprite::LoadTexture(100, L"Resources/2d/icon/PickupAxe.png");
	Sprite::LoadTexture(101, L"Resources/2d/icon/SwordPickup.png");
	Sprite::LoadTexture(102, L"Resources/2d/icon/addweaponnav.png");

	Sprite* l_Tex2 = Sprite::Create(102, {0.f, 0.f});
	Sprite* l_Tex[WeaponSize];

	l_Tex[AXE] = Sprite::Create(100, {0.f, 0.f});
	l_Tex[SWORD] = Sprite::Create(101, {0.f, 0.f});
	//l_Tex[Weapon::SWORD] = Sprite::Create(102, { 0,0 });


	for (int i = 0; i < WeaponSize; i++)
	{
		PickUptex[i].reset(l_Tex[i]);
		PickUptex[i]->SetAnchorPoint({0.5f, 0.5f});
	}
	WeaponChestUpdateTex.reset(l_Tex2);
	WeaponChestUpdateTex->SetAnchorPoint({0.5f, 0.5f});

	pickEffect = std::make_unique<Particle>();
	pickEffect->Init(65);
}

void DropWeapon::Drop(Weapon weapon, XMFLOAT3 droppos)
{
	WeaponIndex = weapon;

	WeaponObjPos[WeaponIndex] = droppos;

	WeaponObjScl[WeaponIndex] = {4.0f, 4.0f, 4.0f};

	DPhase[WeaponIndex] = CREATEWEAPON;
}


void DropWeapon::DropWeaponBeha()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	if (DPhase[WeaponIndex] == NON)
	{
		WeaponChestUpTex_Alpha -= 0.02f;
	}
	if (DPhase[WeaponIndex] == CREATEWEAPON)
	{
		WeaponObj[WeaponIndex] = std::make_unique<Object3d>();
		//ÉÇÉfÉãäÑÇËìñÇƒ
		WeaponObj[WeaponIndex]->Initialize(camera);
		if (WeaponIndex == AXE)
		{
			WeaponObj[WeaponIndex]->SetModel(Model::CreateFromOBJ("axe"));
			PickUpTexAlpha[AXE] = 1.0f;
		}
		if (WeaponIndex == SWORD)
		{
			WeaponObj[WeaponIndex]->SetModel(Model::CreateFromOBJ("Wand"));
			PickUpTexAlpha[SWORD] = 1.0f;
		}

		DPhase[WeaponIndex] = UPDATE;
	}

	if (DPhase[WeaponIndex] == UPDATE)
	{
		XMFLOAT3 PPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

		WeaponObjRot[WeaponIndex].x = -60.0f;
		WeaponObjRot[WeaponIndex].y += 2.0f;
		WeaponObjRot[WeaponIndex].z = -90.0f;

		if (Collision::GetLength(WeaponObjPos[WeaponIndex], PPos) < 60)
		{
			if (Input::GetInstance()->TriggerButton(Input::A))
			{
				DPhase[WeaponIndex] = PICKUP;
			}
		}
	}
	else if (DPhase[WeaponIndex] == PICKUP)
	{
		WeaponChestUpTex_Alpha += 0.02f;
		if (WeaponIndex == AXE)
		{
			PickUpTexAlpha[AXE] -= 0.02f;
		}
		if (WeaponIndex == SWORD)
		{
			PickUpTexAlpha[SWORD] -= 0.02f;
		}
		PickUpWeaponBeha(WeaponIndex);

		if (WeaponChestUpTex_Alpha >= 2.0f)
		{
			DPhase[WeaponIndex] = NON;
		}
	}
	pickEffect->Upda_B();
}

void DropWeapon::PickUpWeaponBeha(Weapon weapon)
{
	XMFLOAT3 PPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	if (PickUpWeapon_EaseTime[weapon] >= 1.0f)
	{
		return;
	}
	if (PickUpWeapon_EaseTime[weapon] >= 0.8f)
	{
		pickEffect->SetParF(1);

		pickEffect->CreateParticle(true, PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	}
	PickUpWeapon_EaseTime[weapon] += 0.05f;


	WeaponObjPos[weapon].x = Easing::EaseOut(PickUpWeapon_EaseTime[weapon], WeaponObjPos[weapon].x, PPos.x);

	WeaponObjPos[weapon].z = Easing::EaseOut(PickUpWeapon_EaseTime[weapon], WeaponObjPos[weapon].z, PPos.z);

	WeaponObjScl[weapon].x = Easing::EaseOut(PickUpWeapon_EaseTime[weapon], 4.0f, 0.0f);
	WeaponObjScl[weapon].y = Easing::EaseOut(PickUpWeapon_EaseTime[weapon], 4.0f, 0.0f);
	WeaponObjScl[weapon].z = Easing::EaseOut(PickUpWeapon_EaseTime[weapon], 4.0f, 0.0f);
}

void DropWeapon::Upda()
{
	DropWeaponBeha();

	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	//3D->2Dïœä∑ 3à¯êîè¡Ç∑
	XMVECTOR tex2DPos[WeaponSize];

	for (int i = 0; i < WeaponSize; i++)
	{
		if (WeaponObj[i] == nullptr)
		{
			continue;
		}
		WeaponObj[i]->SetPosition(WeaponObjPos[i]);
		WeaponObj[i]->SetScale(WeaponObjScl[i]);
		WeaponObj[i]->SetRotation(WeaponObjRot[i]);
		//ÉÇÉfÉãäÑÇËìñÇƒ
		WeaponObj[i]->Update({1.f, 1.f, 1.f, 1.f}, camera);

		tex2DPos[i] = {WeaponObjPos[i].x, WeaponObjPos[i].y, WeaponObjPos[i].z};
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], CameraControl::GetInstance()->GetCamera()->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], CameraControl::GetInstance()->GetCamera()->GetProjectionMatrix(),
		                              true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], CameraControl::GetInstance()->GetCamera()->GetViewPort(), false);

		PickUptex[i]->SetPosition({tex2DPos[i].m128_f32[0], tex2DPos[i].m128_f32[1]});
		PickUptex[i]->SetSize({300.f, 300.f});
		PickUptex[i]->setcolor({1.f, 1.f, 1.f, PickUpTexAlpha[i]});
	}
	WeaponChestUpdateTex->SetPosition({840.0f, 300.0f});
	WeaponChestUpdateTex->SetSize({800.0f, 400.0f});
	WeaponChestUpdateTex->setcolor({1.f, 1.f, 1.f, WeaponChestUpTex_Alpha});

	WeaponChestUpTex_Alpha = min(WeaponChestUpTex_Alpha, 3.0f);
	WeaponChestUpTex_Alpha = max(WeaponChestUpTex_Alpha, 0.0f);
}

void DropWeapon::Draw()
{
	Object3d::PreDraw();
	for (int i = 0; i < WeaponSize; i++)
	{
		if (WeaponObj[i] == nullptr)
		{
			continue;
		}
		WeaponObj[i]->Draw();
	}
	Object3d::PostDraw();
	pickEffect->Draw();
}

void DropWeapon::Draw_PickTex()
{
	XMFLOAT3 PPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	Sprite::PreDraw();

	
		for (int i = 0; i < WeaponSize; i++)
		{
			if (Collision::GetLength(WeaponObjPos[i], PPos) < 60)
			{
			if (WeaponObj[i] == nullptr)
			{
				continue;
			}

			PickUptex[i]->Draw();
		}
	}
	WeaponChestUpdateTex->Draw();
	Sprite::PostDraw();
}
