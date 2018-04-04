#include "stdafx.h"
#include "InputClass.h"


InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &)
{
}


InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	// Ű �迭 �ʱ�ȭ
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ������ true ����
	m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	// Ű�� �����Ǹ� false ����
	m_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
