//-------------------------------------------------------------------------------------
// SimpleMath.cpp -- Simplified C++ Math wrapper for DirectXMath
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//-------------------------------------------------------------------------------------

//#include "pch.h"
#include "SimpleMath.h"

/****************************************************************************
*
* Constants
*
****************************************************************************/

namespace DirectX
{
		const Vector2 Vector2::Zero = { 0.f, 0.f };
		const Vector2 Vector2::One = { 1.f, 1.f };
		const Vector2 Vector2::UnitX = { 1.f, 0.f };
		const Vector2 Vector2::UnitY = { 0.f, 1.f };

		const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
		const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
		const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
		const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
		const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
		const Vector3 Vector3::Up = { 0.f, 1.f, 0.f };
		const Vector3 Vector3::Down = { 0.f, -1.f, 0.f };
		const Vector3 Vector3::Right = { 1.f, 0.f, 0.f };
		const Vector3 Vector3::Left = { -1.f, 0.f, 0.f };
		const Vector3 Vector3::Forward = { 0.f, 0.f, -1.f };
		const Vector3 Vector3::Backward = { 0.f, 0.f, 1.f };

		const Vector4 Vector4::Zero = { 0.f, 0.f, 0.f, 0.f };
		const Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };
		const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
		const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
		const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
		const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

		const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f };

		const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };
}


/****************************************************************************
*
* Viewport
*
****************************************************************************/

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
static_assert(sizeof(DirectX::Viewport) == sizeof(D3D11_VIEWPORT), "Size mismatch");
static_assert(offsetof(DirectX::Viewport, x) == offsetof(D3D11_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, y) == offsetof(D3D11_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, width) == offsetof(D3D11_VIEWPORT, Width), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, height) == offsetof(D3D11_VIEWPORT, Height), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, minDepth) == offsetof(D3D11_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, maxDepth) == offsetof(D3D11_VIEWPORT, MaxDepth), "Layout mismatch");
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
static_assert(sizeof(DirectX::Viewport) == sizeof(D3D12_VIEWPORT), "Size mismatch");
static_assert(offsetof(DirectX::Viewport, x) == offsetof(D3D12_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, y) == offsetof(D3D12_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, width) == offsetof(D3D12_VIEWPORT, Width), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, height) == offsetof(D3D12_VIEWPORT, Height), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, minDepth) == offsetof(D3D12_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(offsetof(DirectX::Viewport, maxDepth) == offsetof(D3D12_VIEWPORT, MaxDepth), "Layout mismatch");
#endif

RECT DirectX::Viewport::ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight)
{
	float safew = (float(backBufferWidth) + 19.f) / 20.f;
	float safeh = (float(backBufferHeight) + 19.f) / 20.f;

	RECT rct;
	rct.left = static_cast<LONG>(safew);
	rct.top = static_cast<LONG>(safeh);
	rct.right = static_cast<LONG>(float(backBufferWidth) - safew + 0.5f);
	rct.bottom = static_cast<LONG>(float(backBufferHeight) - safeh + 0.5f);

	return rct;
}