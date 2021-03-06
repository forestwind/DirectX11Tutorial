#include "stdafx.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
//#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = (D3DClass*)_aligned_malloc(sizeof(D3DClass), 16);
	if (!m_Direct3D)
	{
		return false;
	}
	
	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 0.0f, -30.0f);

	// 모델 객체 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// 모델 객체 초기화
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),(char*)"../DirectX11Tutorial/data/stone01.tga"))
	{
		MessageBox(hwnd, L"Could not Initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// ColorShader 객체 생성
	//m_ColorShader = new ColorShaderClass;
	//if (!m_ColorShader)
	//{
	//	return false;
	//}

	//// ColorShader 객체 초기화
	//if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	//{
	//	MessageBox(hwnd, L"Could not Initialize the colorShader object.", L"Error", MB_OK);
	//	return false;
	//}

	// TextureShader 객체 생성
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// TextureShader 객체 초기화
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(),hwnd))
	{
		MessageBox(hwnd, L"Could not Initialize the Texture Shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// ColorShader 객체 반환
	//if (m_ColorShader)
	//{
	//	m_ColorShader->Shutdown();
	//	delete m_ColorShader;
	//	m_ColorShader = 0;
	//}

	// TextureShader 객체 반환
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// 모델 객체 반환
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// 카메라 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		_aligned_free(m_Direct3D);
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	// 그래픽 렌더링 처리
	return Render();
}

bool GraphicsClass::Render()
{
	// 씬을 그리기 위해 버퍼 지우기
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬 생성
	m_Camera->Render();

	// 카메라 및 d3d객체에서 월드, 뷰, 투영 행렬을 가져옴
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 드로잉 준비
	m_Model->Render(m_Direct3D->GetDeviceContext());


	// 색상 쉐이더를 사용하여 모델을 렌더링
	/*if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}*/

	// 텍스쳐 쉐이더를 사용하여 모델 렌더링
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture()))
	{
		return false;
	}

	// 버퍼의 내용을 화면에 출력
	m_Direct3D->EndScene();

	return true;
}
