#include "../Queue.h"
#include "../Semaphore.h"
#include "../CommandBuffer.h"
#include "../Swapchain.h"

namespace vkf {
	SubmitInfo::SubmitInfo(std::vector<std::shared_ptr<Semaphore>> const& waitSemaphores,
		std::vector<std::shared_ptr<CommandBuffer>> const&  cmdBuffers,
		std::vector<std::shared_ptr<Semaphore>> const& signalSemaphores,
		std::vector<VkPipelineStageFlags> pipelineStageFlagBits)
	:m_waitSemaphores(waitSemaphores),
	m_cmdBuffers(cmdBuffers),
	m_signalSemaphores(signalSemaphores),
	m_pipelineStageFlagBits(pipelineStageFlagBits){}

	Queue::Queue(VkQueue queue,uint32_t familyIndex)
		:queue(queue),
		familyIndex(familyIndex){}

	void Queue::submit(std::vector<SubmitInfo> const& submitInfos,VkFence fence) {
		std::vector<VkSubmitInfo> infos;
		std::vector<std::vector<VkSemaphore>> wSem;
		wSem.reserve(submitInfos.size());
		std::vector<std::vector<VkSemaphore>> sSem;
		sSem.reserve(submitInfos.size());
		std::vector<std::vector<VkCommandBuffer>> cmdBuff;
		cmdBuff.reserve(submitInfos.size());

		for (uint32_t i = 0; i < submitInfos.size();++i) {

					wSem.push_back(std::vector<VkSemaphore>());
					wSem.back().reserve(submitInfos[i].m_waitSemaphores.size());
				for (uint32_t j = 0; j < submitInfos[i].m_waitSemaphores.size(); ++j) {
					wSem[i].push_back(submitInfos[i].m_waitSemaphores[j]->getSemaphore());
				}

					sSem.push_back(std::vector<VkSemaphore>());
					sSem.back().reserve(submitInfos[i].m_signalSemaphores.size());
				for (uint32_t j = 0; j < submitInfos[i].m_signalSemaphores.size(); ++j) {
					sSem[i].push_back(submitInfos[i].m_signalSemaphores[j]->getSemaphore());
				}

				cmdBuff.push_back(std::vector<VkCommandBuffer>());
				cmdBuff.back().reserve(submitInfos[i].m_cmdBuffers.size());
			for (uint32_t j = 0; j < submitInfos[i].m_cmdBuffers.size(); ++j) {
					cmdBuff[i].push_back(submitInfos[i].m_cmdBuffers[j]->getCommandBuffer());
				
			}

			infos.push_back(VkSubmitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO,nullptr,wSem[i].size(),wSem[i].data(),submitInfos[i].m_pipelineStageFlagBits.data(),
				cmdBuff[i].size(),	cmdBuff[i].data(),	sSem[i].size(),sSem[i].data() });
		}
		vkQueueSubmit(queue, infos.size(), infos.data(), fence);
	}

	VkResult Queue::present(std::vector<std::shared_ptr<Semaphore>> waitSemaphores,std::vector<std::shared_ptr<Swapchain>>
	swapchains,uint32_t imageIndex) {
		VkPresentInfoKHR presentInfo;

		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.waitSemaphoreCount = waitSemaphores.size();

		std::vector<VkSemaphore> semaphores;
		for (auto s : waitSemaphores) {
			semaphores.push_back(s->getSemaphore());
		}

		presentInfo.pWaitSemaphores = semaphores.data();
		std::vector<VkSwapchainKHR> swp;
		for (auto s : swapchains) {
			swp.push_back(s->getSwapchain());
		}

		presentInfo.swapchainCount = swp.size();
		presentInfo.pSwapchains = swp.data();
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = NULL;
		VkResult res = vkQueuePresentKHR(queue, &presentInfo);
		return res;
	}

	void Queue::wait() {
		vkQueueWaitIdle(queue);
	}
}