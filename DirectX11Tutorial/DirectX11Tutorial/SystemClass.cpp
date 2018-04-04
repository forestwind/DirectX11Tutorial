#include "stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SystemClass.h"

SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass &)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	// ������ â ����, ���� ���� ���� �ʱ�ȭ
	int screenWidth = 0;
	int screenHeight = 0;

	// ������ ���� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	// Input ���� ��ü ����
	m_Input = new InputClass;
	
	if (!m_Input)
	{
		return false;
	}

	// Input ��ü �ʱ�ȭ
	m_Input->Initialize();


	// �׷��� ������ ��ü ����
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// �׷��� ��ü �ʱ�ȭ
	return m_Graphics->Initialize(screenWidth,screenHeight,m_hwnd);
}

void SystemClass::Shutdown()
{
	// �׷��� ��ü ��ȯ
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// ��ǲ ��ü ��ȯ
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}


	// ������ ���� ó��
	ShutdownWindows();
}

void SystemClass::Run()
{

	// �޽��� ����ü ���� �� �ʱ�ȭ
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		// ������ �޽��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���� �޽����� �޽��� ���� Ż��
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �� �ܿ��� ������ �Լ� ó��
			if (!Frame())
				break;
		}
	}
}

bool SystemClass::Frame()
{
	// ESC Ű ���� �� ���� ���� ó��
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// �׷��� ��ü ������ ó��
	return m_Graphics->Frame();
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{

	switch (umsg)
	{
		// Ű �ٿ� ó�� 
		case WM_KEYDOWN:
		{
			// Ű flag�� ��ǲ ��ü�� ó���ϰ� ����
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Ű �� ó�� 
		case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// �⺻ �޽��� ó��
		default: 
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}



void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	// �ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;


	// �� ���α׷��� �ν��Ͻ� ���
	m_hinstance = GetModuleHandle(NULL);

	// ���α׷� �̸� ����
	m_applicationName = L"Dx11Demo";

	// ������ Ŭ���� ����
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ���� ���
	RegisterClassEx(&wc);


	//����� ȭ���� �ػ� �б�
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;
	
	// Ǯ ��ũ�� ������� ����
	if (FULL_SCREEN)
	{
		// ����� ȭ�� �ش絵�� ����ũ�� �ػ󵵷� ������ ������ 32��Ʈ ����
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ������ ���� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		// ������ ����� ��� 800 * 600 ����
		screenWidth = 800;
		screenHeight = 600;

		// ������ â�� ����,������ �� ��� ��ġ
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}


	// ������ ���� �� �ڵ� ���
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ�� ����
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

}

void SystemClass::ShutdownWindows()
{
	// Ǯ��ũ�� ���� ���� �ʱ�ȭ
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// ������ â ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���α׷� �ν��Ͻ� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// ������ ���� Ȯ��
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}

		//������ �������� Ȯ��
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}

		// �ý��� Ŭ������ �޽��� ó���� �ѱ��
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}