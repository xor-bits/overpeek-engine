#pragma once

#include "engine/enum.hpp"

#include <numeric>



namespace oe::utils
{
	template<typename char_type>
	using color_string_part = std::tuple< std::basic_string_view<char_type>, oe::color >;
	template<typename char_type>
	using color_string_vec = std::vector<color_string_part<char_type>>;

	template<typename char_type>
	struct color_string
	{
		std::basic_string<char_type> string{};
		color_string_vec<char_type> vec;

		color_string(std::basic_string_view<char_type> str = {}, oe::color c = oe::colors::white)
			: string(str)
			, vec({})
		{
			if(!string.empty())
				vec.push_back({ string, c });
		}
		
		// usage: color_string({ string, color }, { string, color }, ... , { string, color })
		color_string(std::initializer_list<color_string_part<char_type>> args)
			: vec(args)
		{
			size_t string_size = std::accumulate(vec.cbegin(), vec.cend(), static_cast<size_t>(0), [](size_t since, const color_string_part<char_type>& part){ return since + std::get<0>(part).size(); });
			string.reserve(string_size); // single allocation for the string
			for (auto& part : vec)
			{
				auto end = string.cend();
				string.append(std::get<0>(part));
				const auto d = std::distance(end, string.cend());
				std::get<0>(part) = { &*end, static_cast<size_t>(std::max(static_cast<decltype(d)>(0), d)) };
			}
		}

		color_string(const color_string& copy)
		{
			operator=(copy);
		}

		color_string& operator=(const color_string& copy)
		{
			string = {};
			vec = copy.vec;
			if(copy.string.empty()) // is view
				return *this;
			
			// copy the string
			size_t string_size = std::accumulate(copy.vec.cbegin(), copy.vec.cend(), static_cast<size_t>(0), [](size_t since, const color_string_part<char_type>& part){ return since + std::get<0>(part).size(); });
			string.reserve(string_size); // single allocation for the string
			for (auto& part : vec)
			{
				auto end = string.cend();
				string.append(std::get<0>(part));
				const auto d = std::distance(end, string.cend());
				std::get<0>(part) = { &*end, static_cast<size_t>(std::max(static_cast<decltype(d)>(0), d)) };
			}
			return *this;
		}

		bool operator==(const color_string& other) const noexcept
		{
			return vec == other.vec;
		}
	};

	template<typename char_type>
	struct color_string_view : color_string<char_type>
	{
		color_string_view(std::basic_string_view<char_type> str = {}, oe::color c = oe::colors::white)
			: color_string<char_type>()
		{
			color_string<char_type>::string = {};
			std::get<0>(color_string<char_type>::vec.at(0)) = str;
			std::get<1>(color_string<char_type>::vec.at(0)) = c;
		}
		
		// usage: color_string_view({ string, color }, { string, color }, ... , { string, color })
		color_string_view(std::initializer_list<color_string_part<char_type>> args)
			: color_string<char_type>()
		{
			color_string<char_type>::string = {};
			color_string<char_type>::vec = { args };
		}
	};
}