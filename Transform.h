#pragma once
#include <list>
#include <iostream>
#include "Entity.h"
#include "SimpleMath.h"

using namespace DirectX;

class Entity;
class Transform
{
public:
	Transform(Entity* entity_);
	~Transform();
	
	inline const Transform* GetParent() { return _parent; };
	inline const Vector3& GetPosition() { return _position; };
	inline const Quaternion& GetRotation() { return _rotation; };
	inline const Vector3& GetScale() { return _scale; };

	inline const Vector3& GetLocalPosition() { return _localPosition; };
	inline const Quaternion& GetLocalRotation() { return _localRotation; };
	inline const Vector3& GetLocalScale() { return _localScale; };

	Matrix GetWorldTransform();

	void SetParent(Transform * parent);
	void SetPosition(const Vector3& worldPosition);
	void SetRotation(const Quaternion& worldRotation);
	void SetScale(const Vector3& worldScale);
	void SetLocalPosition(const Vector3& localPosition);
	void SetLocalRotation(const Quaternion& localRotation);
	void SetLocalScale(const Vector3& localScale);
	Vector3 WorldToLocal(const Vector3& worldPos);
public:
	std::string Name;
private:
	void UpdateChildrenTransform(bool updatePos,bool updateRot,bool updateSca);
	inline void UpdatePositionByLocal() { _parent != nullptr ? (_position = Vector3::Transform(_parent->GetScale()*_localPosition, _parent->GetRotation()) + _parent->GetPosition()) : (_position = _localPosition); };
	inline void UpdateRotationByLocal() { _parent != nullptr ? (_rotation = _localRotation * _parent->GetRotation()) : (_rotation = _localRotation); };
	inline void UpdateScaleByLocal() { _parent != nullptr ? (_scale = _localScale * _parent->GetScale()) : (_scale = _localScale); };
private:
	std::list<Transform*> _children;
	Entity * _hostEntity;
	Transform * _parent;
	bool _needUpdate;
	Matrix _world;
	Vector3 _position;//虽然维护本地和世界两个有个麻烦。否则外部普通的get调用，一级一级的去算比较费
	Quaternion _rotation;
	Vector3 _scale;
	Vector3 _localPosition;
	Quaternion _localRotation;
	Vector3 _localScale;
};