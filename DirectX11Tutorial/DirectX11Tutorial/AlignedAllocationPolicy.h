#pragma once

// 관련 링크
// https://msdn.microsoft.com/en-us/library/dn448573.aspx
//

template<size_t Alignment>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, Alignment);
	}

	static void operator delete(void* memory)
	{
		return _aligned_free(memory);
	}
};