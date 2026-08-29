#pragma once
#ifndef KATHA_CORE_H__
#define KATHA_CORE_H__ 1

#include "core/enums.hpp"
#include "core/types.hpp"
#include "core/constants.hpp"

namespace katha
{
	struct config_t;

	extern void* allocate(const size_t size, const source_t& source = source_t::current());
	extern void release(void* memory, const source_t& source = source_t::current());

	template <typename T>
	static T* alloc(const size_t num, const source_t& source = source_t::current())
	{
		return (T*)allocate(sizeof(T) * num, source);
	}
}

#endif