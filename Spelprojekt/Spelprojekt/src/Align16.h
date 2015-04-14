#ifndef ALIGN16_H
#define ALIGN16_H

class Align_16
{
public:
	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}

	void* operator new[](size_t count)
	{
		return _aligned_malloc(count, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator delete[](void* p)
	{
		_aligned_free(p);
	}

};

#endif