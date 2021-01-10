#include "gl_command_buffer.hpp"
#include "gl_include.hpp"
#include "engine/internal_libs.hpp"



namespace oe::graphics
{
	template<typename ... Args>
	static std::string args_to_string(Args&& ... args)
	{
		constexpr std::string_view format_str_base = "{}";
		std::string format_str{};
		for (size_t i = 0; i < sizeof...(args); ++i)
			format_str += format_str_base;
		
		return fmt::format(format_str, std::forward<Args>(args)...);
	}

	template<typename FP, typename ... Args>
	static void oe_gl_api_call(FP func, [[maybe_unused]] std::string_view function_name, Args&& ... args)
	{
#if defined(OE_DEBUG_API_CALLS)
		spdlog::debug("Calling OpenGL fn: {} with args: [{}]", function_name, args_to_string(std::forward<Args>(args)...));
#endif
		func(std::forward<Args>(args)...);
	}

	void GLCommandEntry::run()
	{
		// no type validity checks as if they were invalid, something else has already went wrong
		switch (command)
		{
		default:
			spdlog::error("Invalid GL call");
			break;

		case gl_command::bind_buffer:
			oe_gl_api_call(glBindBuffer, "glBindBuffer", std::get<uint32_t>(arguments[0]), std::get<uint32_t>(arguments[1]));
			break;

		case gl_command::clear:
			oe_gl_api_call(glClear, "glClear", std::get<uint32_t>(arguments[0]));
			break;

		case gl_command::clear_color:
			oe_gl_api_call(glClearColor, "glClearColor", std::get<float>(arguments[0]), std::get<float>(arguments[1]), std::get<float>(arguments[2]), std::get<float>(arguments[3]));
			break;

		case gl_command::none:
			break;
		}
	}

	bool GLCommandBuffer::run()
	{
		if(m_command_queue.empty())
			return false;

		spdlog::debug(m_command_queue.size());

		auto command = m_command_queue.front();
		command.first.run();
		m_command_queue.pop();

		return !m_command_queue.empty();
	}
}