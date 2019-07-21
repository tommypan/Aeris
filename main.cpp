#include <windows.h>
#include <windowsx.h>
#include <memory>
#include "Scene.h"
#include "RenderSetting.h"

using namespace std;

Scene* demo = nullptr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEX wcex;
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(wcex);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = L"LightDemo";
	wcex.lpszMenuName = nullptr;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcex))
		return 0;


	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW, L"LightDemo", L"LightDemo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nShowCmd);


	bool result =  RenderSetting::GetIntance()->InitDirect3D(hInstance, hwnd);
	if (!result)
		return 0;
	demo = Scene::GetInstance();
	demo->LoadContent();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		demo->Update(0.0f);
		demo->Render();		
	}
	demo->UnLoadContent();
	delete demo;
	demo = nullptr;
	RenderSetting::GetIntance()->ShutDown();
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paintStruct);
		EndPaint(hWnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		demo->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		demo->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		demo->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}