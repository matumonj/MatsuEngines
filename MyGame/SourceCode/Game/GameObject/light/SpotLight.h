#pragma once

#include <DirectXMath.h>

/// <summary>
/// �X�|�b�g���C�g
/// </summary>
class SpotLight
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

public: // �����o�֐�
	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	/// <param name="lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }

	/// <summary>
	/// ���C�g�������擾
	/// </summary>
	/// <returns>���C�g����</returns>
	const XMVECTOR& GetLightDir() { return lightdir; }

	/// <summary>
	/// ���C�g���W���Z�b�g
	/// </summary>
	/// <param name="lightpos">���C�g���W</param>
	void SetLightPos(const XMFLOAT3& lightpos) { this->lightpos = lightpos; }

	/// <summary>
	/// ���C�g���W���擾
	/// </summary>
	/// <returns>���C�g���W</returns>
	const XMFLOAT3& GetLightPos() { return lightpos; }

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name="lightcolor">���C�g�F</param>
	void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }

	/// <summary>
	/// ���C�g�F���擾
	/// </summary>
	/// <returns>���C�g�F</returns>
	const XMFLOAT3& GetLightColor() { return lightcolor; }

	/// <summary>
	/// ���C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="lightatten">���C�g���������W��</param>
	void SetLightAtten(const XMFLOAT3& lightAtten) { this->lightAtten = lightAtten; }

	/// <summary>
	/// ���C�g���������W�����擾
	/// </summary>
	/// <returns>���C�g���������W��</returns>
	const XMFLOAT3& GetLightAtten() { return lightAtten; }

	/// <summary>
	/// ���C�g�����p�x���Z�b�g
	/// </summary>
	/// <param name="lightFactorAngle">x:�����J�n�p�x y:�����I���p�x[degree]</param>
	void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}

	/// <summary>
	/// ���C�g�����p�x���擾
	/// </summary>
	/// <returns>���C�g���������W��</returns>
	const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos; }

	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	void SetActive(bool active) { this->active = active; }

	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
	bool IsActive() { return active; }

private: // �����o�ϐ�
	// ���C�g�����i�P�ʃx�N�g���j
	XMVECTOR lightdir = {0, -1, 0, 0};
	// ���C�g���W�i���[���h���W�n�j
	XMFLOAT3 lightpos = {0, 0, 0};
	// ���C�g�F
	XMFLOAT3 lightcolor = {1, 1, 1};
	// ���C�g���������W��
	XMFLOAT3 lightAtten = {0.0f, 0.0f, 0.0f};
	// ���C�g�����p�x
	XMFLOAT2 lightFactorAngleCos = {0.2f, 0.5f};
	// �L���t���O
	bool active = false;
};
