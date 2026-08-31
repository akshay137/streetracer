#pragma once
#ifndef KATHA_TYPE_BITFIELD_H__
#define KATHA_TYPE_BITFIELD_H__ 1

#include "primitive.hpp"

namespace katha
{
	template <uint16_t MaxBits, typename FieldType = uint8_t>
	struct bitfield_t
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
		inline bool has_enum(const E e) const { return has(static_cast<uint32_t>(e)); }

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

		template <typename... Bits>
		void set(const uint32_t bit, Bits... bits)
		{
			set(bit);
			set(bits...);
		}

		template <typename E>
		inline void set_enum(const E e) { set(static_cast<uint32_t>(e)); }

		template <typename E, typename... Enums>
		void set_enum(const E e, Enums... enums)
		{
			set_enum(e);
			set_enum(enums...);
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
		inline void unset_enum(const E e) { unset(static_cast<uint32_t>(e)); }

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
		inline void toggle_enum(const E e) { toggle(static_cast<uint32_t>(e)); }

		template <typename... Enum>
		static bitfield_t<MaxBits, FieldType> from_enum(Enum... enums)
		{
			bitfield_t<MaxBits, FieldType> field = {};
			field.set_enum(enums...);
			return field;
		}
	};

	// shorthand for enums,
	// enum must have a `__max` that has highest value
	// all values must be positive
	// otherwise behavior is undefined
	template <typename E, typename FieldType = uint8_t>
	using efield_t = bitfield_t<static_cast<uint16_t>(E::__max), FieldType>;
}

#endif