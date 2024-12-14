#pragma once
#include <cstdlib>
#include <cassert>
#include <foundation/PxAllocatorCallback.h>

/// <summary>
/// ȿ���� �޸� ����� ���� PhysX Memory Allocator.
/// Flax Engine ���.
/// </summary>

namespace Pg::Engine
{
	class PhysicsAllocator : public physx::PxAllocatorCallback
	{
		//virtual ~PhysicsAllocator() {}

		void* allocate(size_t size, const char* typeName, const char* filename, int line) override
		{
			assert(size < 1024 * 1024 * 1024); // ���ո��� Allocation Size ����.

			//Windows Only.
			//Alignment : 16BYTE���� �ϵ��ڵ� �� ��.
			size_t alignment = 16;
			void* ptr = _aligned_malloc((size_t)size, alignment);
			return ptr;
		}

		void deallocate(void* ptr) override
		{
			//Windows Only.
			_aligned_free(ptr);
		}
	};
}


