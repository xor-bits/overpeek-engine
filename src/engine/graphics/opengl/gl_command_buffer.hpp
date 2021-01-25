#pragma once

#include "engine/graphics/interface/command_buffer.hpp"
#include "engine/utility/ts_queue.hpp"

#include <variant>



namespace oe::graphics
{
	enum class gl_command
	{
		// actual commands
		bind_buffer,
		clear,
		clear_color,
		
		// command count
		COUNT,
		// special case
		none,
	};



	class GLCommandEntry
	{
	public:
		constexpr static size_t max_possible_arg_count = 4;
		using possible_types = std::variant<uint32_t, float>;

		template<typename ... Args>
		constexpr GLCommandEntry(gl_command _command, Args&& ... _args)
			: command(_command)
			, arguments({ std::forward<Args>(_args)... })
			/* , argument_count(sizeof...(_args)) */
		{}

		void run();

	private:
		gl_command command = gl_command::none;
		std::array<possible_types, max_possible_arg_count> arguments{};
		/* size_t argument_count = 0; */
	};



	class GLCommandBuffer : public ICommandBuffer
	{
	public:
		GLCommandBuffer() = default;
		~GLCommandBuffer() override = default;

		// run api calls, main thread only
		bool run() override;

		// api calls, (thread safe)ified lol

		inline void bindBuffer(uint32_t target_glenum, uint32_t buffer_gluint)
		{ m_command_queue.emplace(gl_command::bind_buffer, target_glenum, buffer_gluint); }
		inline void clear(uint32_t mask_glbitfield)
		{ m_command_queue.emplace(gl_command::clear, mask_glbitfield); }
		inline void clearColor(float red, float green, float blue, float alpha)
		{ m_command_queue.emplace(gl_command::clear_color, red, green, blue, alpha); }

	private:
		oe::utils::ts_queue<GLCommandEntry> m_command_queue;
	};
}