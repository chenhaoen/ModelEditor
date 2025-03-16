#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

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

typedef struct Offset2D {
	int32_t    x;
	int32_t    y;
} Offset2D;

typedef struct Extent2D {
	uint32_t    width;
	uint32_t    height;
} Extent2D;

typedef struct Rect2D {
	Offset2D    offset;
	Extent2D    extent;
} Rect2D;

typedef struct Viewport {
	float    x;
	float    y;
	float    width;
	float    height;
	float    minDepth;
	float    maxDepth;
} Viewport;

enum PipelineType
{
	Model,
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

enum UniformType {
	UNIFORM_TYPE_SAMPLER, // For sampling only (sampler GLSL type).
	UNIFORM_TYPE_SAMPLER_WITH_TEXTURE, // For sampling only, but includes a texture, (samplerXX GLSL type), first a sampler then a texture.
	UNIFORM_TYPE_TEXTURE, // Only texture, (textureXX GLSL type).
	UNIFORM_TYPE_IMAGE, // Storage image (imageXX GLSL type), for compute mostly.
	UNIFORM_TYPE_TEXTURE_BUFFER, // Buffer texture (or TBO, textureBuffer type).
	UNIFORM_TYPE_SAMPLER_WITH_TEXTURE_BUFFER, // Buffer texture with a sampler(or TBO, samplerBuffer type).
	UNIFORM_TYPE_IMAGE_BUFFER, // Texel buffer, (imageBuffer type), for compute mostly.
	UNIFORM_TYPE_UNIFORM_BUFFER, // Regular uniform buffer (or UBO).
	UNIFORM_TYPE_STORAGE_BUFFER, // Storage buffer ("buffer" qualifier) like UBO, but supports storage, for compute mostly.
	UNIFORM_TYPE_INPUT_ATTACHMENT, // Used for sub-pass read/write, for mobile mostly.
	UNIFORM_TYPE_MAX
};

struct BoundUniform {
	UniformType type = UNIFORM_TYPE_MAX;
	uint32_t binding = 0xffffffff; // Binding index as specified in shader.
	std::vector<ID> ids;
	// Flag to indicate  that this is an immutable sampler so it is skipped when creating uniform
	// sets, as it would be set previously when creating the pipeline layout.
	bool immutable_sampler = false;
};