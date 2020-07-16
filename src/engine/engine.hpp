#pragma once

#include "engine/enum.hpp"
#include <utility>



namespace oe::graphics { class Instance; }
namespace oe {

	class Engine {
	public:
		EngineInfo engine_info;
		oe::graphics::Instance* instance;
		depth_functions current_depth;
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

		void init(EngineInfo engine_info);
		void deinit();

		void terminate();
		void __error(std::string error_msg, int line, std::string file);

		// set blending mode
		void blending(oe::modes mode = oe::modes::enable) const;

		// set depth function or disable it
		void depth(depth_functions func = depth_functions::always);
		depth_functions getDepth() const;

		// also known as vertical sync
		void swapInterval(unsigned int interval = 0) const;

		// stop drawing any side of primitives
		void culling(culling_modes c = culling_modes::neither) const;

		// width of all lines in pixels
		void lineWidth(float w = 1.0f) const;

		// radius of all points in pixels
		void pointRadius(float w = 1.0f) const;

		// primitive draw mode
		void polygonMode(polygon_mode p = polygon_mode::fill) const;
	
	};
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
		constexpr auto sep = str_array_cast<chr_type>(", ");

		const std::vector<T> dv(&d.x, &d.x + dim);
		const auto value = fmt::join(dv, sep.c_str());

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
		constexpr auto sep = str_array_cast<chr_type>(", ");

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

// (input) -> ("input: {} line file", input)
#define quickDebug(x) spdlog::debug(#x ## ": {} at line {} in file {}", x, __LINE__, __FILE__)

#define oe_error_terminate(error_msg, ...) oe::Engine::getSingleton().__error(fmt::format(error_msg, ##__VA_ARGS__), __LINE__, __FILE__)
#define oe_terminate() oe::Engine::getSingleton().terminate()

// #define OE_DEBUGGING
#ifdef OE_DEBUGGING
#define oe_debug_call(a) spdlog::debug("oe_debug_call: {}, file: {}", a, __FILE__)
#else
#define oe_debug_call(a)
#endif