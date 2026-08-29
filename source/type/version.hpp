#pragma once
#ifndef KATHA_TYPE_VERSION_H__
#define KATHA_TYPE_VERSION_H__ 1

#include "primitive.hpp"

namespace katha
{
	struct version_t
	{
		uint32_t major:8 = 0;
		uint32_t minor:8 = 0;
		uint32_t patch:16 = 0;

		constexpr version_t() = default;
		constexpr version_t(
			const uint32_t major,
			const uint32_t minor,
			const uint32_t patch
		): major{major}, minor{minor}, patch{patch}
		{}

		constexpr uint32_t packed() const
		{
			uint32_t result = (major << 24) | (minor << 16) | (patch);
			return result;
		}

		constexpr version_t(const uint32_t version)
		{
			major = version >> 24;
			minor = (version >> 16) & 0xFF;
			patch = version & 0xFFFF;
		}

		constexpr bool operator < (const version_t& rhs) const
		{
			return packed() < rhs.packed();
		}

		constexpr bool operator > (const version_t& rhs) const
		{
			return packed() > rhs.packed();
		}

		constexpr bool operator == (const version_t& rhs) const
		{
			return packed() == rhs.packed();
		}
	};
}

#endif