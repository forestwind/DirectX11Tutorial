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
	// 키 배열 초기화
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int input)
{
	// 키가 눌리면 true 저장
	m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	// 키가 해제되면 false 저장
	m_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
