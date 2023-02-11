#pragma once
#include"Object3d.h"
#include<array>
#include<memory>
class FootSwitch
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	FootSwitch();
	~FootSwitch();
private:
	//�X�C�b�`�̐�
	static constexpr auto SwitchSize = 4;
	//�X�C�b�`���f��
	std::array<std::unique_ptr<Object3d>, SwitchSize>FootSwitchObj;
	//���W
	std::array<XMFLOAT3, SwitchSize>FootSwitchPos;
	//�F
	std::array<XMFLOAT4, SwitchSize>FootSwitchColor;
	//�ӂ܂ꂽ��
	std::array<bool, SwitchSize>FootSwitchJudg;

	//�����񂾃X�C�b�`�̐�
	int ClearSwitchQuantity;
	//�S�ē��ݏI������
	bool AllClear;
public:
	void Init();
	void Upda();
	void Draw();

private:
	bool FootJudg(XMFLOAT3 switchpos);
	XMFLOAT4 SwitchChangeColor(XMFLOAT3 switchpos,bool& judg);
};

