#pragma once
#ifndef KATHA_TYPE_FLAGS_H__
#define KATHA_TYPE_FLAGS_H__ 1

#include "primitive.hpp"

namespace katha
{
	template <typename FlagType=uint32_t>
	struct flags_t
	{
		FlagType flags = 0;

		bool has(const FlagType flag) const
		{
			const FlagType bit = 1 << static_cast<FlagType>(flag);
			return bit == (flags & bit);
		}

		void set(const FlagType flag)
		{
			const FlagType bit = 1 << static_cast<FlagType>(flag);
			flags |= bit;
		}

		void unset(const FlagType flag)
		{
			const FlagType bit = 1 << static_cast<FlagType>(flag);
			flags &= ~bit;
		}

		// returns updated value
		bool toggle(const FlagType flag)
		{
			if (has(flag))
			{
				unset(flag);
				return false;
			}
			
			set(flag);
			return true;
		}
	};

	template <typename EnumType, typename FlagType=uint32_t>
	struct eflags_t
	{
		flags_t<FlagType> flags = {};

		bool has(const EnumType eflag) const
		{
			return flags.has(static_cast<FlagType>(eflag));
		}

		void set(const EnumType eflag)
		{
			flags.set(static_cast<FlagType>(eflag));
		}

		void unset(const EnumType eflag)
		{
			flags.unset(static_cast<FlagType>(eflag));
		}

		// returns updated value
		bool toggle(const EnumType eflag)
		{
			return flags.toggle(static_cast<FlagType>(eflag));
		}
	};
}

#endif