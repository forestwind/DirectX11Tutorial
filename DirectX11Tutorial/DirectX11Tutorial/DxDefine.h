#pragma once


/////////////
// LINKING //
/////////////

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")


/////////////
// INCLUDE //
/////////////

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;


///////////////////////////////////////////
// warning C4316 ó���� 2017������ �߻�����//
//////////////////////////////////////////
#include "AlignedAllocationPolicy.h"