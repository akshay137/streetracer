#pragma once
#ifndef KATHA_TYPE_BITFIELD_H__
#define KATHA_TYPE_BITFIELD_H__ 1

#include "primitive.hpp"

namespace katha
{
	template <uint16_t MaxBits, typename FieldType = uint8_t>
	struct BitField
	{
		constexpr static inline uint64_t QUANTITY = static_cast<uint64_t>(MaxBits);
		constexpr static inline uint64_t FIELD_CAPACITY = sizeof(FieldType) * 8;

		static constexpr uint64_t GetBucketSize()
		{
			constexpr uint64_t FULL = QUANTITY / FIELD_CAPACITY;
			constexpr uint64_t PARTIAL = QUANTITY % FIELD_CAPACITY;
			
			return FULL + (PARTIAL ? 1 : 0);
		}

		constexpr static inline uint32_t BUCKET_SIZE = GetBucketSize();
		FieldType bits[BUCKET_SIZE] = {};

		constexpr bool has(const uint32_t bit) const
		{
			const uint32_t array_index = bit / FIELD_CAPACITY;
			if (array_index >= BUCKET_SIZE)
			{
				// we don't have this bit at all
				return false;
			}

			const uint32_t bit_index = bit % FIELD_CAPACITY;

			const FieldType mask = static_cast<FieldType>(1) << bit_index;
			return 0 != (bits[array_index] & mask);
		}

		template <typename E>
		constexpr bool hasEnum(const E e) const { return has(static_cast<uint32_t>(e)); }

		template <typename E, typename... Enums>
		constexpr bool hasEnum(const E e1, const E e2, Enums... enums)
		{
			const bool first = hasEnum(e1);
			const bool rest = hasEnum(e2, enums...);
			return first || rest;
		}

		constexpr void set(const uint32_t bit)
		{
			const uint32_t array_index = bit / FIELD_CAPACITY;
			if (array_index >= BUCKET_SIZE)
			{
				// we don't have this bit at all
				return;
			}

			const uint32_t bit_index = bit % FIELD_CAPACITY;

			bits[array_index] |= static_cast<FieldType>(1) << bit_index;
		}

		template <typename E>
		constexpr void setEnum(const E e) { set(static_cast<uint32_t>(e)); }

		template <typename E, typename... Enums>
		constexpr void setEnum(const E e1, const E e2, Enums... enums)
		{
			setEnum(e1);
			setEnum(e2, enums...);
		}

		constexpr void unset(const uint32_t bit)
		{
			const uint32_t array_index = bit / FIELD_CAPACITY;
			if (array_index >= BUCKET_SIZE)
			{
				// we don't have this bit at all
				return;
			}

			const uint32_t bit_index = bit % FIELD_CAPACITY;

			bits[array_index] &= ~(static_cast<FieldType>(1) << bit_index);
		}

		template <typename E>
		constexpr void unsetEnum(const E e) { unset(static_cast<uint32_t>(e)); }

		constexpr bool toggle(const uint32_t bit)
		{
			if (has(bit))
			{
				unset(bit);
				return false;
			}

			set(bit);
			return true;
		}

		template <typename E>
		constexpr void toggleEnum(const E e) { toggle(static_cast<uint32_t>(e)); }

		template <typename... Enum>
		static constexpr BitField<MaxBits, FieldType> FromEnum(Enum... enums)
		{
			BitField<MaxBits, FieldType> field = {};
			field.setEnum(enums...);
			return field;
		}

		static constexpr BitField<MaxBits, FieldType> None()
		{
			return {};
		}
	};

	// shorthand for enums,
	// enum must have a `__MAX` that has highest value
	// all values must be positive
	// otherwise behavior is undefined
	template <typename E, typename FieldType = uint8_t>
	using EField = BitField<static_cast<uint16_t>(E::__MAX), FieldType>;
}

#endif