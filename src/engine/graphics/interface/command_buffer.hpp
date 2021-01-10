#pragma once



namespace oe::graphics
{
	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;
		virtual bool run() = 0; // return true if not empty
	protected:
		ICommandBuffer() = default;
		ICommandBuffer(const ICommandBuffer& /* copy */) = delete;
		ICommandBuffer(ICommandBuffer&&      /* move */) = delete;
	};
}