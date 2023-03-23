#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;

	void SetAttribute(unsigned short attribute)
	{
		this->attribute = attribute;
	}

	void AddAttribute(unsigned short attribute)
	{
		this->attribute |= attribute;
	}

	void RemoveAttribute(unsigned short attribute)
	{
		this->attribute &= !attribute;
	}

protected:
	//�����蔻�葮��
	unsigned short attribute = 0b1111111111111111;

public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	void SetObject(Object3d* object)
	{
		this->object3d = object;
	}

	Object3d* GetObject3d()
	{
		return object3d;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info)
	{
		object3d->OnCollision(info);
	}

protected:
	Object3d* object3d = nullptr;
	// �`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};
