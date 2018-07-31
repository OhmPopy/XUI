#pragma once

#include "CVulkanUtility.h"
#include "CVulkanSharedDefines.h"

#include <list>
#include <queue>

namespace irr
{
    namespace video
    {
        class VulkanCmdBuffer;
        class VulkanSemaphore;
        class VulkanSwapChain;

        /** @addtogroup Vulkan
         *  @{
         */

         /** Wrapper for the Vulkan device queue. */
        class VulkanQueue
        {
        public:
            VulkanQueue(VulkanDevice& device, VkQueue queue, GpuQueueType type, uint32_t index);

            /** Returns the internal handle to the Vulkan queue object. */
            VkQueue getHandle() const { return mQueue; }

            /** Returns the device that owns the queue. */
            VulkanDevice& getDevice() const { return mDevice; }

            /** Returns the type of the queue. */
            GpuQueueType getType() const { return mType; }

            /** Returns the unique index of the queue, for its type. */
            uint32_t getIndex() const { return mIndex; }

            /**
             * Checks if anything is currently executing on this queue.
             *
             * @note	This status is only updated after a VulkanCommandBufferManager::refreshStates() call.
             */
            bool isExecuting() const;

            /** Submits the provided command buffer on the queue. */
            void submit(VulkanCmdBuffer* cmdBuffer, VulkanSemaphore** waitSemaphores, uint32_t semaphoresCount);

            /**
             * Stores information about a submit internally, but doesn't actually execute it. The intended use is to queue
             * multiple submits and execute them all at once using submitQueued(), ensuring better performance than queuing them
             * all individually.
             */
            void queueSubmit(VulkanCmdBuffer* cmdBuffer, VulkanSemaphore** waitSemaphores, uint32_t semaphoresCount);

            /** Submits all previously queued commands buffers, as recorded by queueSubmit(). */
            void submitQueued();

            /**
             * Presents the back buffer of the provided swap chain.
             *
             * @param[in]	swapChain			Swap chain whose back buffer to present.
             * @param[in]	waitSemaphores		Optional semaphores to wait on before presenting the queue.
             * @param[in]	semaphoresCount		Number of semaphores in the @p semaphores array.
             */
            void present(VulkanSwapChain* swapChain, VulkanSemaphore** waitSemaphores, uint32_t semaphoresCount);

            /** Blocks the calling thread until all operations on the queue finish. */
            void waitIdle() const;

            /**
             * Checks if any of the active command buffers finished executing on the queue and updates their states
             * accordingly.
             *
             * @param[in]	forceWait	Set to true if the system should wait until all command buffers finish executing.
             * @param[in]	queueEmpty	Set to true if the caller guarantees the queue will be empty (e.g. on shutdown). This
             *							allows the system to free all needed resources.
             */
            void refreshStates(bool forceWait, bool queueEmpty = false);

            /** Returns the last command buffer that was submitted on this queue. */
            VulkanCmdBuffer* getLastCommandBuffer() const { return mLastCommandBuffer; }

        protected:
            /**
             * Generates a submit-info structure that can be used for submitting the command buffer to the queue, but doesn't
             * perform the actual submit.
             */
            void getSubmitInfo(VkCommandBuffer* cmdBuffer, VkSemaphore* signalSemaphores, uint32_t numSignalSemaphores,
                VkSemaphore* waitSemaphores, uint32_t numWaitSemaphores, VkSubmitInfo& submitInfo);

            /**
             * Prepares a list of semaphores that can be provided to submit or present calls. *
             *
             * @param[in]		inSemaphores	External wait semaphores that need to be waited on.
             * @param[out]		outSemaphores	All semaphores (external ones, and possibly additional ones), as Vulkan handles.
             * @param[in, out]	semaphoresCount	Number of semaphores in @p inSemaphores when calling. When method returns this
             *									will contain number of semaphores in @p outSemaphores.
             */
            void prepareSemaphores(VulkanSemaphore** inSemaphores, VkSemaphore* outSemaphores, uint32_t& semaphoresCount);

            /** Information about a single submitted command buffer. */
            struct SubmitInfo
            {
                SubmitInfo(VulkanCmdBuffer* cmdBuffer, uint32_t submitIdx, uint32_t numSemaphores, uint32_t numCommandBuffers)
                    : cmdBuffer(cmdBuffer), submitIdx(submitIdx), numSemaphores(numSemaphores)
                    , numCommandBuffers(numCommandBuffers)
                { }

                VulkanCmdBuffer* cmdBuffer;
                uint32_t submitIdx;
                uint32_t numSemaphores;
                uint32_t numCommandBuffers;
            };

            VulkanDevice& mDevice;
            VkQueue mQueue;
            GpuQueueType mType;
            uint32_t mIndex;
            VkPipelineStageFlags mSubmitDstWaitMask[_MAX_UNIQUE_QUEUES];

            std::vector<SubmitInfo> mQueuedBuffers;
            std::vector<VulkanSemaphore*> mQueuedSemaphores;

            std::list<SubmitInfo> mActiveSubmissions;
            std::queue<VulkanCmdBuffer*> mActiveBuffers;
            std::queue<VulkanSemaphore*> mActiveSemaphores;
            VulkanCmdBuffer* mLastCommandBuffer;
            bool mLastCBSemaphoreUsed;
            uint32_t mNextSubmitIdx;

            std::vector<VkSemaphore> mSemaphoresTemp;
        };

        /** @} */
    }
}