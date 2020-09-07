#pragma once
#ifdef BUILD_VULKAN

#include <vector>
#include <vulkan/vulkan.hpp>

#include "vk_logical_device.hpp"
#include "vk_swapchain.hpp"



namespace oe::graphics {

	class VKSyncObjects {
	public:
		std::vector<vk::Semaphore> m_image_available_semaphores;
		std::vector<vk::Semaphore> m_render_finished_semaphores;
		std::vector<vk::Fence> m_in_flight_fences;
		std::vector<vk::Fence> m_images_in_flight;
		size_t currentFrame = 0;

		const VKLogicalDevice* m_logical_device;
		const VKSwapChain* m_swap_chain;

	public:
		VKSyncObjects(const VKSwapChain* swap_chain, const VKLogicalDevice* logical_device) : m_swap_chain(swap_chain), m_logical_device(logical_device) {
			m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
			m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
			m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
			m_images_in_flight.resize(m_swap_chain->m_swap_chain_render_targets.size(), nullptr);

			vk::SemaphoreCreateInfo semaphoreInfo = {};

			vk::FenceCreateInfo fenceInfo = {};
			fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				m_image_available_semaphores[i] = m_logical_device->m_logical_device.createSemaphore(semaphoreInfo);
				m_render_finished_semaphores[i] = m_logical_device->m_logical_device.createSemaphore(semaphoreInfo);

				m_in_flight_fences[i] = m_logical_device->m_logical_device.createFence(fenceInfo);
			}
		}

		~VKSyncObjects() {
			m_logical_device->m_logical_device.waitIdle();
			// semaphores
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				m_logical_device->m_logical_device.destroySemaphore(m_image_available_semaphores[i]);
				m_logical_device->m_logical_device.destroySemaphore(m_render_finished_semaphores[i]);
				m_logical_device->m_logical_device.destroyFence(m_in_flight_fences[i]);
			}
		}
	};

}
#endif