#pragma once

#include "engine/enum.hpp"
#include "engine/internal_libs.hpp"
#include <utility>

#ifndef __has_include
#error __has_include not supported
#else
#  if __has_include(<filesystem>)
#    include <filesystem>
namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#  endif
#endif



namespace oe::graphics { class Instance; }
namespace oe {

	class Engine {
	public:
		EngineInfo engine_info;
		std::unique_ptr<oe::graphics::Instance> instance;
		RasterizerInfo rasterizer_info{};
		std::chrono::high_resolution_clock::time_point engine_init_tp;
		static size_t draw_calls;

	private:
		static Engine* singleton;
		Engine();

	public:
		Engine(const Engine&) = delete;
		static Engine& getSingleton() {
			if (!singleton) singleton = new Engine();
			return *singleton;
		}

		void init(const EngineInfo& engine_info);
		void deinit();

		void terminate();
		void __error(const std::string& error_msg, int line, const std::string& file);

		[[nodiscard]] constexpr inline const RasterizerInfo& getRasterizerInfo() const noexcept { return rasterizer_info; }
		void setRasterizerInfo(const RasterizerInfo& i);
	};

	template <typename T> struct is_container : std::false_type {};
	template <typename T, typename alloc> struct is_container<std::vector<T, alloc>> : std::true_type {};
	template <typename T, size_t S> struct is_container<std::array<T, S>> : std::true_type {};
	template <typename T, size_t S> struct is_container<gsl::span<T, S>> : std::true_type {};
}



// https://stackoverflow.com/a/52738638
template< typename CharT, std::size_t Length >
struct str_array
{
    constexpr CharT const* c_str()              const { return data_; }
    constexpr CharT const* data()               const { return data_; }
    constexpr CharT operator[]( std::size_t i ) const { return data_[ i ]; }
    constexpr CharT const* begin()              const { return data_; }
    constexpr CharT const* end()                const { return data_ + Length; }
    constexpr std::size_t size()                const { return Length; }
    // TODO: add more members of std::basic_string

    CharT data_[ Length + 1 ];  // +1 for null-terminator
};

namespace detail {
    template< typename ResT, typename SrcT >
    constexpr ResT static_cast_ascii( SrcT x )
    {
        if( !( x >= 0 && x <= 127 ) )
            throw std::out_of_range( "Character value must be in basic ASCII range (0..127)" );
        return static_cast<ResT>( x );
    }

    template< typename ResElemT, typename SrcElemT, std::size_t N, std::size_t... I >
    constexpr str_array< ResElemT, N - 1 > do_str_array_cast( const SrcElemT(&a)[N], std::index_sequence<I...> )
    {
        return { static_cast_ascii<ResElemT>( a[I] )..., 0 };
    }
} //namespace detail

template< typename ResElemT, typename SrcElemT, std::size_t N, typename Indices = std::make_index_sequence< N - 1 > >
constexpr str_array< ResElemT, N - 1 > str_array_cast( const SrcElemT(&a)[N] )
{
    return detail::do_str_array_cast< ResElemT >( a, Indices{} );
}

template<typename str, typename formatter, typename arg_joiner, typename FormatContext>
auto arg_join_contexted(const str& begin, const str& end, formatter& T_formatter, arg_joiner& value, FormatContext& ctx)
{
	auto it = value.begin;
	auto out = ctx.out();
	out = std::copy(begin.begin(), begin.end(), out);
	ctx.advance_to(out);
	if (it != value.end) {
		out = T_formatter.format(*it++, ctx);
		while (it != value.end) {
			out = std::copy(value.sep.begin(), value.sep.end(), out);
			ctx.advance_to(out);
			out = T_formatter.format(*it++, ctx);
		}
	}
	out = std::copy(end.begin(), end.end(), out);
	ctx.advance_to(out);
	return out;
}

// all glm vec:s to all char types
template <class chr_type, int dim, class T>
struct fmt::formatter<glm::vec<dim, T>, chr_type> {
	formatter<T, chr_type> T_formatter;

	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return T_formatter.parse(ctx);
	}

	template <typename FormatContext>
	auto format(const glm::vec<dim, T>& d, FormatContext& ctx) {
		constexpr auto begin = str_array_cast<chr_type>("[ ");
		constexpr auto end = str_array_cast<chr_type>(" ]");
		constexpr auto sep = ", ";

		const std::vector<T> dv(&d.x, &d.x + dim);
		const auto value = fmt::join(dv, sep);

		return arg_join_contexted(begin, end, T_formatter, value, ctx);
	}
};

template <class chr_type, class T>
struct fmt::formatter<glm::qua<T>, chr_type> {
	formatter<glm::vec<3, T>, chr_type> vec3_formatter;

	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return vec3_formatter.parse(ctx);
	}

	template <typename FormatContext>
	auto format(const glm::qua<T>& d, FormatContext& ctx) {
		constexpr auto begin = str_array_cast<chr_type>("[ <");
		constexpr auto mid = str_array_cast<chr_type>(">, ");
		constexpr auto end = str_array_cast<chr_type>(" ]");

		const glm::vec<3, T> dv { d.x, d.y, d.z };

		auto out = ctx.out();
		out = std::copy(begin.begin(), begin.end(), out);
		ctx.advance_to(out);
		
		out = vec3_formatter.T_formatter.format(d.w, ctx);
		
		out = std::copy(mid.begin(), mid.end(), out);
		ctx.advance_to(out);

		out = vec3_formatter.format(dv, ctx);
		
		out = std::copy(end.begin(), end.end(), out);
		ctx.advance_to(out);

		return out;
	}
};

template <class chr_type, int dim, class T>
struct fmt::formatter<glm::mat<dim, dim, T>, chr_type> {
    formatter<glm::vec<dim, T>, chr_type> vec_formatter;

	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return vec_formatter.parse(ctx);
	}

	template <typename FormatContext>
	auto format(const glm::mat<dim, dim, T>& d, FormatContext& ctx) {
		constexpr auto begin = str_array_cast<chr_type>("[ ");
		constexpr auto end = str_array_cast<chr_type>(" ]");
		constexpr auto sep = str_array_cast<chr_type>(", ");
		
		std::vector<glm::vec<dim, T>> dv;
		dv.reserve(dim);
		for(int i = 0; i < dim; i++)
		{
			dv.push_back(d[i]);
		}
		const auto value = fmt::join(dv, sep.c_str());

		return arg_join_contexted(begin, end, vec_formatter, value, ctx);
	}
};

// format fs::path
template <>
struct fmt::formatter<fs::path> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const fs::path& p, FormatContext& ctx) {
		return format_to(ctx.out(), "{}", p.generic_string());
	}
};
template <>
struct fmt::formatter<fs::path::const_iterator> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const fs::path::const_iterator& p, FormatContext& ctx) {
		return format_to(ctx.out(), "{}", p->generic_string());
	}
};