#pragma once
#include"Object3d.h"
#include<array>
#include<memory>

#include<fstream>
#include<string>
#include<sstream>
#include"ObjectManager.h"
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
	FootSwitch(){};
	~FootSwitch(){};

	static FootSwitch* GetIns();
private:
	//�X�C�b�`�̐�
	static constexpr auto SwitchSize = 4;
	//�X�C�b�`���f��
	std::array<std::unique_ptr<Object3d>, SwitchSize>FootSwitchObj;
	//����X�C�b�`���f��
	std::array<std::unique_ptr<Object3d>, SwitchSize>FootSwitchFrameObj;
	//���W
	std::array<XMFLOAT3, SwitchSize>FootSwitchPos;
	//�傫��
	static constexpr XMFLOAT3 FootSwitchScl={3.f,3.f,3.f};
	static constexpr XMFLOAT3 FootSwitchFrameScl = { 5.f,2.5f,5.f };
	//�F
	std::array<XMFLOAT4, SwitchSize>FootSwitchColor;
	//�F�ς�鑬�x�C�[�W���O�p
	std::array<float, SwitchSize>FootSwitchEaseCount;

	//�ӂ܂ꂽ��
	std::array<bool, SwitchSize>FootSwitchJudg;
	std::array<bool, SwitchSize>SwitchClearJudg;
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
	XMFLOAT4 SwitchChangeColor(XMFLOAT3 switchpos,bool& judg, bool& clearjudg, float& ColoEaseCount);

private:
	//CSV�ǂݍ��ݕ���
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;
	void LoadCSV();


};

