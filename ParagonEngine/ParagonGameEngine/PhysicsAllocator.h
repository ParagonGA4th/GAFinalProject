#pragma once
#include <cstdlib>
#include <cassert>
#include <foundation/PxAllocatorCallback.h>

/// <summary>
/// 효율적 메모리 사용을 위한 PhysX Memory Allocator.
/// Flax Engine 기반.
/// </summary>

namespace Pg::Engine
{
	class PhysicsAllocator : public physx::PxAllocatorCallback
	{
		//virtual ~PhysicsAllocator() {}

		void* allocate(size_t size, const char* typeName, const char* filename, int line) override
		{
			assert(size < 1024 * 1024 * 1024); // 불합리한 Allocation Size 제한.

			//Windows Only.
			//Alignment : 16BYTE으로 하드코딩 할 것.
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


