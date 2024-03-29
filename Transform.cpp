#include "Transform.h"
#include "Macro.h"

Transform::Transform(Entity* entity_) :_hostEntity(entity_)
{
	Name = entity_->Name;
	_position = Vector3::Zero;
	_rotation = Quaternion::Identity;
	_scale = Vector3::One;
	_needUpdate = true;
	SetParent(nullptr);
};

Transform::~Transform()
{
	_hostEntity = nullptr;
	_parent = nullptr;
}

 Matrix Transform::GetWorldTransform()
{
	 if (_needUpdate)
	 {
		 _world = Matrix::Compose(_position, _rotation, _scale);
		 _needUpdate = false;
	 }
	 return _world;
}

 void Transform::SetParent(Transform * parent)
 {
	 _parent = parent;
	 if (_parent != nullptr)
	 {
		 _parent->_children.push_back(this);
	 }
	 SetScale(_scale);
	 SetRotation(_rotation);
	 SetPosition(_position);
 }

void Transform::SetPosition(const Vector3& worldPosition)
{
	_needUpdate = true;
	_position = worldPosition;
	if (_parent != nullptr)
	{
		Quaternion invertRotation;
		_parent->GetRotation().Inverse(invertRotation);
		_localPosition = Vector3::Transform(worldPosition - _parent->GetPosition(), invertRotation) /_parent->GetScale();
	}
	else
	{
		_localPosition = worldPosition;
	}
	UpdateChildrenTransform(true,false,false);
}

void Transform::SetRotation(const Quaternion& worldRotation)
{
	_needUpdate = true;
	_rotation = worldRotation;
	if (_parent != nullptr)
	{
		_localRotation = worldRotation / _parent->GetRotation();
	}
	else
	{
		_localRotation = worldRotation;
	}
	UpdateChildrenTransform(false,true,false);
}

void Transform::SetScale(const Vector3& worldScale)
{
	_needUpdate = true;
	_scale = worldScale;
	if (_parent != nullptr)
	{
		_localScale = worldScale / _parent->GetScale();
	}
	else
	{
		_localScale = worldScale;
	}
	UpdateChildrenTransform(false,false,true);
}

void Transform::SetLocalPosition(const Vector3& localPosition)
{
	_needUpdate = true;
	_localPosition = localPosition;
	UpdateChildrenTransform(true, false, false);
}

void Transform::SetLocalRotation(const Quaternion& localRotation)
{
	_needUpdate = true;
	_localRotation = localRotation;
	UpdateChildrenTransform(false, true, false);
}

void Transform::SetLocalScale(const Vector3& localScale)
{
	_needUpdate = true;
	_localScale = localScale;
	UpdateChildrenTransform(false, false, true);
}

Vector3 Transform::WorldToLocal(const Vector3& worldPos)
{
	Vector3 result;
	if (_parent != nullptr)
	{
		result = worldPos - _parent->GetPosition();
	}
	else
	{
		result = worldPos;
	}
	return result;
}

void Transform::UpdateChildrenTransform(bool updatePos, bool updateRot, bool updateSca)
{
	_needUpdate = true;
	if (updatePos)
	{
		UpdatePositionByLocal();
	}
	if (updateRot)
	{
		UpdateRotationByLocal();
	}
	if (updateSca)
	{
		UpdateScaleByLocal();
	}

	std::list<Transform*>::iterator start = _children.begin();
	while (start != _children.end())
	{
		(*start)->UpdateChildrenTransform(updatePos,updateRot,updateSca);
		start++;
	}
}