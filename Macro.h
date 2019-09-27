#include "d3dx11effect.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXMath.h>
#include "SimpleMath.h"


#define SAFE_DELETE(p) if(p!=nullptr){delete p;p = nullptr; }
#define SAFE_RELEASE(p) if(p!=nullptr){p->Release();p = nullptr; }
#define PI 3.14159f