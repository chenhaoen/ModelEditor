#pragma once

#include <stdint.h>

struct ID {
	uint64_t id = 0;
	inline ID() = default;
	inline ID(uint64_t p_id) :
		id(p_id) {}
};

#define DEFINE_ID(m_name)                                                         \
	struct m_name##ID : public ID {                                               \
		inline explicit operator bool() const {                          \
			return id != 0;                                                       \
		}                                                                         \
		inline m_name##ID &operator=(m_name##ID p_other) {               \
			id = p_other.id;                                                      \
			return *this;                                                         \
		}                                                                         \
		inline bool operator<(const m_name##ID &p_other) const {         \
			return id < p_other.id;                                               \
		}                                                                         \
		inline bool operator==(const m_name##ID &p_other) const {        \
			return id == p_other.id;                                              \
		}                                                                         \
		inline bool operator!=(const m_name##ID &p_other) const {        \
			return id != p_other.id;                                              \
		}                                                                         \
		inline m_name##ID(const m_name##ID &p_other) : ID(p_other.id) {} \
		inline explicit m_name##ID(uint64_t p_int) : ID(p_int) {}        \
		inline explicit m_name##ID(void *p_ptr) : ID((uint64_t)p_ptr) {} \
		inline m_name##ID() = default;                                   \
	};

// Id types declared before anything else to prevent cyclic dependencies between the different concerns.
DEFINE_ID(Buffer);
DEFINE_ID(Texture);
DEFINE_ID(Sampler);
DEFINE_ID(VertexFormat);
DEFINE_ID(CommandQueue);
DEFINE_ID(CommandQueueFamily);
DEFINE_ID(CommandPool);
DEFINE_ID(CommandBuffer);
DEFINE_ID(SwapChain);
DEFINE_ID(Framebuffer);
DEFINE_ID(Shader);
DEFINE_ID(UniformSet);
DEFINE_ID(Pipeline);
DEFINE_ID(RenderPass);
DEFINE_ID(QueryPool);
DEFINE_ID(Fence);
DEFINE_ID(Semaphore);
DEFINE_ID(Surface);

struct Extent2D
{
	uint32_t width;
	uint32_t height;
};