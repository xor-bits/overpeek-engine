#pragma once

#include <exception>
#include "engine/internal_libs.hpp"



namespace oe::utils
{
	class formatted_error : public std::runtime_error
	{
	public:
		template<typename FormatString, typename... Args>
		formatted_error(const FormatString &fmt, const Args &... args)
			: std::runtime_error(fmt::format(fmt, args...))
		{}
	};
}