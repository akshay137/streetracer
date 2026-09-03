#pragma once
#ifndef KATHA_TYPE_BITFIELD_H__
#define KATHA_TYPE_BITFIELD_H__ 1

#include "primitive.hpp"

namespace katha
{
	template <uint16_t MaxBits, typename FieldType = uint8_t>
	struct BitField
	{
		constexpr static uint64_t calculate_bucket_size(const uint64_t quantity, const uint64_t capacity)
		{
			const uint64_t full = quantity / capacity;
			const uint64_t partial = quantity % capacity;
			return full + (partial ? 1 : 0);
		}

		constexpr static inline uint32_t BUCKET_SIZE = calculate_bucket_size(MaxBits, sizeof(FieldType) * 8);
		FieldType bits[BUCKET_SIZE] = {};

		bool has(const uint32_t bit) const
		{
			const uint32_t array_index = bit / (sizeof(FieldType) * 8);
			if (array_index >= BUCKET_SIZE)
			{
				// we don't have this bit at all
				return false;
			}

			const uint32_t bit_index = bit % (sizeof(FieldType) * 8);

			const FieldType mask = static_cast<FieldType>(1) << bit_index;
			return 0 != (bits[array_index] & mask);
		}

		template <typename E>
		inline bool hasEnum(const E e) const { return has(static_cast<uint32_t>(e)); }

		template <typename E, typename... Enums>
		bool hasEnum(const E e1, const E e2, Enums... enums)
		{
			const bool first = hasEnum(e1);
			const bool rest = hasEnum(e2, enums...);
			return first || rest;
		}

		void set(const uint32_t bit)
		{
			const uint32_t array_index = bit / (sizeof(FieldType) * 8);
			if (array_index >= BUCKET_SIZE)
			{
				// we don't have this bit at all
				return;
			}

			const uint32_t bit_index = bit % (sizeof(FieldType) * 8);

			bits[array_index] |= static_cast<FieldType>(1) << bit_index;
		}

		template <typename E>
		inline void setEnum(const E e) { set(static_cast<uint32_t>(e)); }

		template <typename E, typename... Enums>
		void setEnum(const E e1, const E e2, Enums... enums)
		{
			setEnum(e1);
			setEnum(e2, enums...);
		}

		void unset(const uint32_t bit)
		{
			const uint32_t array_index = bit / (sizeof(FieldType) * 8);
			if (array_index >= BUCKET_SIZE)
			{
				// we don't have this bit at all
				return;
			}

			const uint32_t bit_index = bit % (sizeof(FieldType) * 8);

			bits[array_index] &= ~(static_cast<FieldType>(1) << bit_index);
		}

		template <typename E>
		inline void unsetEnum(const E e) { unset(static_cast<uint32_t>(e)); }

		bool toggle(const uint32_t bit)
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
		inline void toggleEnum(const E e) { toggle(static_cast<uint32_t>(e)); }

		template <typename... Enum>
		static BitField<MaxBits, FieldType> FromEnum(Enum... enums)
		{
			BitField<MaxBits, FieldType> field = {};
			field.setEnum(enums...);
			return field;
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