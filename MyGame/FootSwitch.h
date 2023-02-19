#pragma once
#include"Object3d.h"
#include<array>
#include<memory>

#include<fstream>
#include<string>
#include<sstream>
#include"ObjectManager.h"
#include "Particle.h"

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
	FootSwitch()
	{
	};

	~FootSwitch()
	{
	};

	static FootSwitch* GetIns();
private:
	static constexpr int CEffectSize = 3;

	struct SwitchParam
	{
		//�X�C�b�`���f��
		std::unique_ptr<Object3d> Obj = nullptr;
		//����X�C�b�`���f��
		std::unique_ptr<Object3d> FrameObj = nullptr;
		//���W
		XMFLOAT3 Pos = {};
		//�F
		XMFLOAT4 Color = {};
		//�F�ς�鑬�x�C�[�W���O�p
		float EaseCount = 0.f;
		//�ӂ܂ꂽ��
		bool FootSwitchJudg = false;
		bool SwitchClearJudg = false;

		//�`���[�W�p�~�`���f��
		std::array<std::unique_ptr<Object3d>, CEffectSize> CEffectObj = {nullptr};
		std::array<XMFLOAT3, CEffectSize> CEffectPos = {};
		std::array<float, CEffectSize> CEffectAlpha = {1.f};
		std::array<bool, CEffectSize> CeffectCreateF = {false};
	};

	//�X�C�b�`�̐�
	static constexpr auto SwitchSize = 4;
	//�X�C�b�`�p�����[�^
	std::array<SwitchParam, SwitchSize> switch_param_;
	//�傫��
	static constexpr XMFLOAT3 FootSwitchScl = {3.f, 3.f, 3.f};
	static constexpr XMFLOAT3 FootSwitchFrameScl = {5.f, 2.5f, 5.f};

	//�����񂾃X�C�b�`�̐�
	int ClearSwitchQuantity;
	//�S�ē��ݏI������
	bool AllClear;
public:
	//������
	void Init();
	//�X�V
	void Upda();
	//�`��
	void Draw();


	//4�̃X�C�b�`�̒��S�_��Ԃ�
	//�󔠂̍��W�p
	XMFLOAT3 Switchs_CenterPos();
	//�ӂ܂ꂽ�X�C�b�`�̐�
	const int GetClearSwicthsQuantity() { return ClearSwitchQuantity; }
private:
	//���܂ꂽ����
	bool FootJudg(XMFLOAT3 switchpos);
	//���܂ꂽ���̐F�ς�
	XMFLOAT4 SwitchChangeColor(XMFLOAT3 switchpos, bool& judg, bool& clearjudg, float& ColoEaseCount);

	void FootEffectUpda();
private:
	//CSV�ǂݍ��ݕ���
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;
	void LoadCSV();
};
