#pragma once

#include "commands.hpp"
#include "sync_objects.hpp"



namespace oe::vulkan {

	void frame(const Commands* commands, SyncObjects* sync_objects) {
		sync_objects->m_logical_device->m_logical_device.waitForFences(sync_objects->m_in_flight_fences, true, UINT64_MAX);

		uint32_t imageIndex;
		imageIndex = sync_objects->m_logical_device->m_logical_device.acquireNextImageKHR(commands->m_swap_chain->m_swap_chain, UINT64_MAX, sync_objects->m_image_available_semaphores[sync_objects->currentFrame], nullptr).value;

		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (sync_objects->m_images_in_flight[imageIndex]) {
			sync_objects->m_logical_device->m_logical_device.waitForFences(sync_objects->m_images_in_flight[imageIndex], true, UINT64_MAX);
		}
		// Mark the image as now being in use by this frame
		sync_objects->m_images_in_flight[imageIndex] = sync_objects->m_images_in_flight[sync_objects->currentFrame];

		// submit info
		vk::SubmitInfo submitInfo = {};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commands->m_command_buffers[imageIndex];

		// waiting for semaphores
		vk::Semaphore waitSemaphores[] = { sync_objects->m_image_available_semaphores[sync_objects->currentFrame] };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// semaphores
		vk::Semaphore signalSemaphores[] = { sync_objects->m_render_finished_semaphores[sync_objects->currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		sync_objects->m_logical_device->m_logical_device.resetFences(sync_objects->m_in_flight_fences[sync_objects->currentFrame]);

		// submitting queue
		sync_objects->m_logical_device->m_graphics_queue.submit(submitInfo, sync_objects->m_in_flight_fences[sync_objects->currentFrame]);

		// presentation
		vk::PresentInfoKHR presentInfo = {};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		vk::SwapchainKHR swapChains[] = { commands->m_swap_chain->m_swap_chain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional

		// present
		sync_objects->m_logical_device->m_present_queue.presentKHR(presentInfo);

		sync_objects->currentFrame = (sync_objects->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

}