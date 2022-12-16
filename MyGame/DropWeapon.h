#pragma once
#include"Sprite.h"
#include<memory>
#include<array>
#include<vector>
#include"Particle.h"
#include"Object3d.h"
class DropWeapon
{
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	enum Weapon {
		SWORD,
		AXE,
	//	WAND
	};
	enum DropPhase {
		NON,
		CREATEWEAPON,
		UPDATE,
		PICKUP
	};
private:
	static constexpr int WeaponSize = 2;
	std::array<std::unique_ptr<Object3d>, WeaponSize>WeaponObj;

	std::array<std::unique_ptr<Sprite>, WeaponSize>PickUptex;
	float PickUpTexAlpha;

	std::unique_ptr<Sprite>WeaponChestUpdateTex;
	float WeaponChestUpTex_Alpha;

	std::array<XMFLOAT3, WeaponSize>WeaponObjPos;

	std::array<XMFLOAT3, WeaponSize>WeaponObjRot;

	std::array<XMFLOAT3, WeaponSize>WeaponObjScl;

	std::array<DropPhase, WeaponSize>DPhase;

	Weapon WeaponIndex;

	std::array<float, WeaponSize>PickUpWeapon_EaseTime;

	std::unique_ptr<Particle>pickEffect;
public:

	static DropWeapon* GtIns();

	void Init();
	void Upda();
	void Draw();
	void Draw_PickTex();
	void Drop(Weapon weapon, XMFLOAT3 droppos);
	void DropWeaponBeha();

	void PickUpWeaponBeha(Weapon weapon);
	bool PickUpWeapon(Weapon weapon) { if (DPhase[weapon] == DropWeapon::PICKUP)return true; }
};

