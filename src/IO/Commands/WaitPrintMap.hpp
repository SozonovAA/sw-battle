#pragma once

#include <iosfwd>
#include <cstdint>

namespace sw::io
{
	struct WaitPrintMap
	{
		constexpr static const char* Name = "WAIT_PRINT";

		uint32_t ticks {};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("ticks", ticks);
		}
	};
}