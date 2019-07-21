#include "d3dx11effect.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXMath.h>
#include "SimpleMath.h"


#define SAFE_DELETE(p) if(p!=nullptr){delete p;p = nullptr; }