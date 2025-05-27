#pragma once

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

enum class PipelineName
{
	Model,
	Skybox,
	Grids,
	GridsGraphics,
};

enum class PipelineType
{
	Graphics,
	Compute
};

enum class BufferType
{
	Vertex,
	Index,
};

enum class DrawMode {
	Indexed,
	NonIndexed
};

enum class PrimitiveType {
	Triangles,
	Lines,
	Points
};

enum class FillMode {
	SOLID,      // 实体填充
	WIREFRAME,  // 线框模式
	POINT,      // 点模式
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 texCoord;
};

struct UniformBufferObject {
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view = glm::mat4(1.0);
	glm::mat4 proj = glm::mat4(1.0);
};

enum class UniformType {
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

enum class BufferUsageBits : uint32_t {
	BUFFER_USAGE_TRANSFER_FROM_BIT = (1 << 0),
	BUFFER_USAGE_TRANSFER_TO_BIT = (1 << 1),
	BUFFER_USAGE_TEXEL_BIT = (1 << 2),
	BUFFER_USAGE_UNIFORM_BIT = (1 << 4),
	BUFFER_USAGE_STORAGE_BIT = (1 << 5),
	BUFFER_USAGE_INDEX_BIT = (1 << 6),
	BUFFER_USAGE_VERTEX_BIT = (1 << 7),
	BUFFER_USAGE_INDIRECT_BIT = (1 << 8),
	BUFFER_USAGE_DEVICE_ADDRESS_BIT = (1 << 17),
};

inline BufferUsageBits operator|(BufferUsageBits a, BufferUsageBits b) {
	return static_cast<BufferUsageBits>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline BufferUsageBits operator&(BufferUsageBits a, BufferUsageBits b) {
	return static_cast<BufferUsageBits>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline BufferUsageBits operator~(BufferUsageBits a) {
	return static_cast<BufferUsageBits>(~static_cast<uint32_t>(a));
}

inline BufferUsageBits& operator|=(BufferUsageBits& a, BufferUsageBits b) {
	a = a | b;
	return a;
}

struct BoundUniform {
	UniformType type = UniformType::UNIFORM_TYPE_MAX;
	uint32_t binding = 0xffffffff; // Binding index as specified in shader.
	std::vector<ID> ids;
	// Flag to indicate  that this is an immutable sampler so it is skipped when creating uniform
	// sets, as it would be set previously when creating the pipeline layout.
	bool immutable_sampler = false;
};

enum class RenderPrimitive {
	RENDER_PRIMITIVE_POINTS,
	RENDER_PRIMITIVE_LINES,
	RENDER_PRIMITIVE_LINES_WITH_ADJACENCY,
	RENDER_PRIMITIVE_LINESTRIPS,
	RENDER_PRIMITIVE_LINESTRIPS_WITH_ADJACENCY,
	RENDER_PRIMITIVE_TRIANGLES,
	RENDER_PRIMITIVE_TRIANGLES_WITH_ADJACENCY,
	RENDER_PRIMITIVE_TRIANGLE_STRIPS,
	RENDER_PRIMITIVE_TRIANGLE_STRIPS_WITH_AJACENCY, // TODO: Fix typo in "ADJACENCY" (in 5.0).
	RENDER_PRIMITIVE_TRIANGLE_STRIPS_WITH_RESTART_INDEX,
	RENDER_PRIMITIVE_TESSELATION_PATCH,
	RENDER_PRIMITIVE_MAX
};

enum class PolygonCullMode {
	POLYGON_CULL_DISABLED,
	POLYGON_CULL_FRONT,
	POLYGON_CULL_BACK,
	POLYGON_CULL_MAX
};

enum class PolygonFrontFace {
	POLYGON_FRONT_FACE_CLOCKWISE,
	POLYGON_FRONT_FACE_COUNTER_CLOCKWISE,
};

enum class StencilOperation {
	STENCIL_OP_KEEP,
	STENCIL_OP_ZERO,
	STENCIL_OP_REPLACE,
	STENCIL_OP_INCREMENT_AND_CLAMP,
	STENCIL_OP_DECREMENT_AND_CLAMP,
	STENCIL_OP_INVERT,
	STENCIL_OP_INCREMENT_AND_WRAP,
	STENCIL_OP_DECREMENT_AND_WRAP,
	STENCIL_OP_MAX
};

enum class LogicOperation {
	LOGIC_OP_CLEAR,
	LOGIC_OP_AND,
	LOGIC_OP_AND_REVERSE,
	LOGIC_OP_COPY,
	LOGIC_OP_AND_INVERTED,
	LOGIC_OP_NO_OP,
	LOGIC_OP_XOR,
	LOGIC_OP_OR,
	LOGIC_OP_NOR,
	LOGIC_OP_EQUIVALENT,
	LOGIC_OP_INVERT,
	LOGIC_OP_OR_REVERSE,
	LOGIC_OP_COPY_INVERTED,
	LOGIC_OP_OR_INVERTED,
	LOGIC_OP_NAND,
	LOGIC_OP_SET,
	LOGIC_OP_MAX
};

enum class BlendFactor {
	BLEND_FACTOR_ZERO,
	BLEND_FACTOR_ONE,
	BLEND_FACTOR_SRC_COLOR,
	BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	BLEND_FACTOR_DST_COLOR,
	BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	BLEND_FACTOR_SRC_ALPHA,
	BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	BLEND_FACTOR_DST_ALPHA,
	BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	BLEND_FACTOR_CONSTANT_COLOR,
	BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	BLEND_FACTOR_CONSTANT_ALPHA,
	BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
	BLEND_FACTOR_SRC_ALPHA_SATURATE,
	BLEND_FACTOR_SRC1_COLOR,
	BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
	BLEND_FACTOR_SRC1_ALPHA,
	BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,
	BLEND_FACTOR_MAX
};

enum class BlendOperation {
	BLEND_OP_ADD,
	BLEND_OP_SUBTRACT,
	BLEND_OP_REVERSE_SUBTRACT,
	BLEND_OP_MINIMUM,
	BLEND_OP_MAXIMUM, // Yes, this one is an actual operator.
	BLEND_OP_MAX
};

enum class TextureSamples {
	TEXTURE_SAMPLES_1,
	TEXTURE_SAMPLES_2,
	TEXTURE_SAMPLES_4,
	TEXTURE_SAMPLES_8,
	TEXTURE_SAMPLES_16,
	TEXTURE_SAMPLES_32,
	TEXTURE_SAMPLES_64,
	TEXTURE_SAMPLES_MAX,
};

enum class CompareOperator {
	COMPARE_OP_NEVER,
	COMPARE_OP_LESS,
	COMPARE_OP_EQUAL,
	COMPARE_OP_LESS_OR_EQUAL,
	COMPARE_OP_GREATER,
	COMPARE_OP_NOT_EQUAL,
	COMPARE_OP_GREATER_OR_EQUAL,
	COMPARE_OP_ALWAYS,
	COMPARE_OP_MAX
};

enum class DataFormat {
	DATA_FORMAT_R4G4_UNORM_PACK8,
	DATA_FORMAT_R4G4B4A4_UNORM_PACK16,
	DATA_FORMAT_B4G4R4A4_UNORM_PACK16,
	DATA_FORMAT_R5G6B5_UNORM_PACK16,
	DATA_FORMAT_B5G6R5_UNORM_PACK16,
	DATA_FORMAT_R5G5B5A1_UNORM_PACK16,
	DATA_FORMAT_B5G5R5A1_UNORM_PACK16,
	DATA_FORMAT_A1R5G5B5_UNORM_PACK16,
	DATA_FORMAT_R8_UNORM,
	DATA_FORMAT_R8_SNORM,
	DATA_FORMAT_R8_USCALED,
	DATA_FORMAT_R8_SSCALED,
	DATA_FORMAT_R8_UINT,
	DATA_FORMAT_R8_SINT,
	DATA_FORMAT_R8_SRGB,
	DATA_FORMAT_R8G8_UNORM,
	DATA_FORMAT_R8G8_SNORM,
	DATA_FORMAT_R8G8_USCALED,
	DATA_FORMAT_R8G8_SSCALED,
	DATA_FORMAT_R8G8_UINT,
	DATA_FORMAT_R8G8_SINT,
	DATA_FORMAT_R8G8_SRGB,
	DATA_FORMAT_R8G8B8_UNORM,
	DATA_FORMAT_R8G8B8_SNORM,
	DATA_FORMAT_R8G8B8_USCALED,
	DATA_FORMAT_R8G8B8_SSCALED,
	DATA_FORMAT_R8G8B8_UINT,
	DATA_FORMAT_R8G8B8_SINT,
	DATA_FORMAT_R8G8B8_SRGB,
	DATA_FORMAT_B8G8R8_UNORM,
	DATA_FORMAT_B8G8R8_SNORM,
	DATA_FORMAT_B8G8R8_USCALED,
	DATA_FORMAT_B8G8R8_SSCALED,
	DATA_FORMAT_B8G8R8_UINT,
	DATA_FORMAT_B8G8R8_SINT,
	DATA_FORMAT_B8G8R8_SRGB,
	DATA_FORMAT_R8G8B8A8_UNORM,
	DATA_FORMAT_R8G8B8A8_SNORM,
	DATA_FORMAT_R8G8B8A8_USCALED,
	DATA_FORMAT_R8G8B8A8_SSCALED,
	DATA_FORMAT_R8G8B8A8_UINT,
	DATA_FORMAT_R8G8B8A8_SINT,
	DATA_FORMAT_R8G8B8A8_SRGB,
	DATA_FORMAT_B8G8R8A8_UNORM,
	DATA_FORMAT_B8G8R8A8_SNORM,
	DATA_FORMAT_B8G8R8A8_USCALED,
	DATA_FORMAT_B8G8R8A8_SSCALED,
	DATA_FORMAT_B8G8R8A8_UINT,
	DATA_FORMAT_B8G8R8A8_SINT,
	DATA_FORMAT_B8G8R8A8_SRGB,
	DATA_FORMAT_A8B8G8R8_UNORM_PACK32,
	DATA_FORMAT_A8B8G8R8_SNORM_PACK32,
	DATA_FORMAT_A8B8G8R8_USCALED_PACK32,
	DATA_FORMAT_A8B8G8R8_SSCALED_PACK32,
	DATA_FORMAT_A8B8G8R8_UINT_PACK32,
	DATA_FORMAT_A8B8G8R8_SINT_PACK32,
	DATA_FORMAT_A8B8G8R8_SRGB_PACK32,
	DATA_FORMAT_A2R10G10B10_UNORM_PACK32,
	DATA_FORMAT_A2R10G10B10_SNORM_PACK32,
	DATA_FORMAT_A2R10G10B10_USCALED_PACK32,
	DATA_FORMAT_A2R10G10B10_SSCALED_PACK32,
	DATA_FORMAT_A2R10G10B10_UINT_PACK32,
	DATA_FORMAT_A2R10G10B10_SINT_PACK32,
	DATA_FORMAT_A2B10G10R10_UNORM_PACK32,
	DATA_FORMAT_A2B10G10R10_SNORM_PACK32,
	DATA_FORMAT_A2B10G10R10_USCALED_PACK32,
	DATA_FORMAT_A2B10G10R10_SSCALED_PACK32,
	DATA_FORMAT_A2B10G10R10_UINT_PACK32,
	DATA_FORMAT_A2B10G10R10_SINT_PACK32,
	DATA_FORMAT_R16_UNORM,
	DATA_FORMAT_R16_SNORM,
	DATA_FORMAT_R16_USCALED,
	DATA_FORMAT_R16_SSCALED,
	DATA_FORMAT_R16_UINT,
	DATA_FORMAT_R16_SINT,
	DATA_FORMAT_R16_SFLOAT,
	DATA_FORMAT_R16G16_UNORM,
	DATA_FORMAT_R16G16_SNORM,
	DATA_FORMAT_R16G16_USCALED,
	DATA_FORMAT_R16G16_SSCALED,
	DATA_FORMAT_R16G16_UINT,
	DATA_FORMAT_R16G16_SINT,
	DATA_FORMAT_R16G16_SFLOAT,
	DATA_FORMAT_R16G16B16_UNORM,
	DATA_FORMAT_R16G16B16_SNORM,
	DATA_FORMAT_R16G16B16_USCALED,
	DATA_FORMAT_R16G16B16_SSCALED,
	DATA_FORMAT_R16G16B16_UINT,
	DATA_FORMAT_R16G16B16_SINT,
	DATA_FORMAT_R16G16B16_SFLOAT,
	DATA_FORMAT_R16G16B16A16_UNORM,
	DATA_FORMAT_R16G16B16A16_SNORM,
	DATA_FORMAT_R16G16B16A16_USCALED,
	DATA_FORMAT_R16G16B16A16_SSCALED,
	DATA_FORMAT_R16G16B16A16_UINT,
	DATA_FORMAT_R16G16B16A16_SINT,
	DATA_FORMAT_R16G16B16A16_SFLOAT,
	DATA_FORMAT_R32_UINT,
	DATA_FORMAT_R32_SINT,
	DATA_FORMAT_R32_SFLOAT,
	DATA_FORMAT_R32G32_UINT,
	DATA_FORMAT_R32G32_SINT,
	DATA_FORMAT_R32G32_SFLOAT,
	DATA_FORMAT_R32G32B32_UINT,
	DATA_FORMAT_R32G32B32_SINT,
	DATA_FORMAT_R32G32B32_SFLOAT,
	DATA_FORMAT_R32G32B32A32_UINT,
	DATA_FORMAT_R32G32B32A32_SINT,
	DATA_FORMAT_R32G32B32A32_SFLOAT,
	DATA_FORMAT_R64_UINT,
	DATA_FORMAT_R64_SINT,
	DATA_FORMAT_R64_SFLOAT,
	DATA_FORMAT_R64G64_UINT,
	DATA_FORMAT_R64G64_SINT,
	DATA_FORMAT_R64G64_SFLOAT,
	DATA_FORMAT_R64G64B64_UINT,
	DATA_FORMAT_R64G64B64_SINT,
	DATA_FORMAT_R64G64B64_SFLOAT,
	DATA_FORMAT_R64G64B64A64_UINT,
	DATA_FORMAT_R64G64B64A64_SINT,
	DATA_FORMAT_R64G64B64A64_SFLOAT,
	DATA_FORMAT_B10G11R11_UFLOAT_PACK32,
	DATA_FORMAT_E5B9G9R9_UFLOAT_PACK32,
	DATA_FORMAT_D16_UNORM,
	DATA_FORMAT_X8_D24_UNORM_PACK32,
	DATA_FORMAT_D32_SFLOAT,
	DATA_FORMAT_S8_UINT,
	DATA_FORMAT_D16_UNORM_S8_UINT,
	DATA_FORMAT_D24_UNORM_S8_UINT,
	DATA_FORMAT_D32_SFLOAT_S8_UINT,
	DATA_FORMAT_BC1_RGB_UNORM_BLOCK,
	DATA_FORMAT_BC1_RGB_SRGB_BLOCK,
	DATA_FORMAT_BC1_RGBA_UNORM_BLOCK,
	DATA_FORMAT_BC1_RGBA_SRGB_BLOCK,
	DATA_FORMAT_BC2_UNORM_BLOCK,
	DATA_FORMAT_BC2_SRGB_BLOCK,
	DATA_FORMAT_BC3_UNORM_BLOCK,
	DATA_FORMAT_BC3_SRGB_BLOCK,
	DATA_FORMAT_BC4_UNORM_BLOCK,
	DATA_FORMAT_BC4_SNORM_BLOCK,
	DATA_FORMAT_BC5_UNORM_BLOCK,
	DATA_FORMAT_BC5_SNORM_BLOCK,
	DATA_FORMAT_BC6H_UFLOAT_BLOCK,
	DATA_FORMAT_BC6H_SFLOAT_BLOCK,
	DATA_FORMAT_BC7_UNORM_BLOCK,
	DATA_FORMAT_BC7_SRGB_BLOCK,
	DATA_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
	DATA_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
	DATA_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
	DATA_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
	DATA_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
	DATA_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
	DATA_FORMAT_EAC_R11_UNORM_BLOCK,
	DATA_FORMAT_EAC_R11_SNORM_BLOCK,
	DATA_FORMAT_EAC_R11G11_UNORM_BLOCK,
	DATA_FORMAT_EAC_R11G11_SNORM_BLOCK,
	DATA_FORMAT_ASTC_4x4_UNORM_BLOCK,
	DATA_FORMAT_ASTC_4x4_SRGB_BLOCK,
	DATA_FORMAT_ASTC_5x4_UNORM_BLOCK,
	DATA_FORMAT_ASTC_5x4_SRGB_BLOCK,
	DATA_FORMAT_ASTC_5x5_UNORM_BLOCK,
	DATA_FORMAT_ASTC_5x5_SRGB_BLOCK,
	DATA_FORMAT_ASTC_6x5_UNORM_BLOCK,
	DATA_FORMAT_ASTC_6x5_SRGB_BLOCK,
	DATA_FORMAT_ASTC_6x6_UNORM_BLOCK,
	DATA_FORMAT_ASTC_6x6_SRGB_BLOCK,
	DATA_FORMAT_ASTC_8x5_UNORM_BLOCK,
	DATA_FORMAT_ASTC_8x5_SRGB_BLOCK,
	DATA_FORMAT_ASTC_8x6_UNORM_BLOCK,
	DATA_FORMAT_ASTC_8x6_SRGB_BLOCK,
	DATA_FORMAT_ASTC_8x8_UNORM_BLOCK,
	DATA_FORMAT_ASTC_8x8_SRGB_BLOCK,
	DATA_FORMAT_ASTC_10x5_UNORM_BLOCK,
	DATA_FORMAT_ASTC_10x5_SRGB_BLOCK,
	DATA_FORMAT_ASTC_10x6_UNORM_BLOCK,
	DATA_FORMAT_ASTC_10x6_SRGB_BLOCK,
	DATA_FORMAT_ASTC_10x8_UNORM_BLOCK,
	DATA_FORMAT_ASTC_10x8_SRGB_BLOCK,
	DATA_FORMAT_ASTC_10x10_UNORM_BLOCK,
	DATA_FORMAT_ASTC_10x10_SRGB_BLOCK,
	DATA_FORMAT_ASTC_12x10_UNORM_BLOCK,
	DATA_FORMAT_ASTC_12x10_SRGB_BLOCK,
	DATA_FORMAT_ASTC_12x12_UNORM_BLOCK,
	DATA_FORMAT_ASTC_12x12_SRGB_BLOCK,
	DATA_FORMAT_G8B8G8R8_422_UNORM,
	DATA_FORMAT_B8G8R8G8_422_UNORM,
	DATA_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
	DATA_FORMAT_G8_B8R8_2PLANE_420_UNORM,
	DATA_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
	DATA_FORMAT_G8_B8R8_2PLANE_422_UNORM,
	DATA_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
	DATA_FORMAT_R10X6_UNORM_PACK16,
	DATA_FORMAT_R10X6G10X6_UNORM_2PACK16,
	DATA_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
	DATA_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
	DATA_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
	DATA_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
	DATA_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
	DATA_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
	DATA_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
	DATA_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
	DATA_FORMAT_R12X4_UNORM_PACK16,
	DATA_FORMAT_R12X4G12X4_UNORM_2PACK16,
	DATA_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
	DATA_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
	DATA_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
	DATA_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
	DATA_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
	DATA_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
	DATA_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
	DATA_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
	DATA_FORMAT_G16B16G16R16_422_UNORM,
	DATA_FORMAT_B16G16R16G16_422_UNORM,
	DATA_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
	DATA_FORMAT_G16_B16R16_2PLANE_420_UNORM,
	DATA_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
	DATA_FORMAT_G16_B16R16_2PLANE_422_UNORM,
	DATA_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
	DATA_FORMAT_MAX,
};

enum class VertexFrequency {
	VERTEX_FREQUENCY_VERTEX,
	VERTEX_FREQUENCY_INSTANCE,
};

struct VertexAttribute {
	uint32_t location = 0; // Shader location.
	uint32_t offset = 0;
	DataFormat format = DataFormat::DATA_FORMAT_MAX;
};

struct VertexBinding
{
	uint32_t binding = 0;
	uint32_t stride = 0;
	VertexFrequency frequency = VertexFrequency::VERTEX_FREQUENCY_VERTEX;
	std::vector< VertexAttribute> vertexAttributes;
};

struct PipelineRasterizationState {
	bool enable_depth_clamp = false;
	bool discard_primitives = false;
	bool wireframe = false;
	PolygonCullMode cull_mode = PolygonCullMode::POLYGON_CULL_DISABLED;
	PolygonFrontFace front_face = PolygonFrontFace::POLYGON_FRONT_FACE_CLOCKWISE;
	bool depth_bias_enabled = false;
	float depth_bias_constant_factor = 0.0f;
	float depth_bias_clamp = 0.0f;
	float depth_bias_slope_factor = 0.0f;
	float line_width = 1.0f;
	uint32_t patch_control_points = 1;
};

struct PipelineMultisampleState {
	TextureSamples sample_count = TextureSamples::TEXTURE_SAMPLES_1;
	bool enable_sample_shading = false;
	float min_sample_shading = 0.0f;
	std::vector<uint32_t> sample_mask;
	bool enable_alpha_to_coverage = false;
	bool enable_alpha_to_one = false;
};

struct VertexInputState
{
	std::vector<VertexBinding> bindings;
};

struct PipelineDepthStencilState {
	bool enable_depth_test = false;
	bool enable_depth_write = false;
	CompareOperator depth_compare_operator = CompareOperator::COMPARE_OP_ALWAYS;
	bool enable_depth_range = false;
	float depth_range_min = 0;
	float depth_range_max = 0;
	bool enable_stencil = false;

	struct StencilOperationState {
		StencilOperation fail = StencilOperation::STENCIL_OP_ZERO;
		StencilOperation pass = StencilOperation::STENCIL_OP_ZERO;
		StencilOperation depth_fail = StencilOperation::STENCIL_OP_ZERO;
		CompareOperator compare = CompareOperator::COMPARE_OP_ALWAYS;
		uint32_t compare_mask = 0;
		uint32_t write_mask = 0;
		uint32_t reference = 0;
	};

	StencilOperationState front_op;
	StencilOperationState back_op;
};

struct PipelineColorBlendState {
	bool enable_logic_op = false;
	LogicOperation logic_op = LogicOperation::LOGIC_OP_CLEAR;

	struct Attachment {
		bool enable_blend = false;
		BlendFactor src_color_blend_factor = BlendFactor::BLEND_FACTOR_ZERO;
		BlendFactor dst_color_blend_factor = BlendFactor::BLEND_FACTOR_ZERO;
		BlendOperation color_blend_op = BlendOperation::BLEND_OP_ADD;
		BlendFactor src_alpha_blend_factor = BlendFactor::BLEND_FACTOR_ZERO;
		BlendFactor dst_alpha_blend_factor = BlendFactor::BLEND_FACTOR_ZERO;
		BlendOperation alpha_blend_op = BlendOperation::BLEND_OP_ADD;
		bool write_r = true;
		bool write_g = true;
		bool write_b = true;
		bool write_a = true;
	};

	static PipelineColorBlendState create_disabled(int p_attachments = 1) {
		PipelineColorBlendState bs;
		for (int i = 0; i < p_attachments; i++) {
			bs.attachments.push_back(Attachment());
		}
		return bs;
	}

	static PipelineColorBlendState create_blend(int p_attachments = 1) {
		PipelineColorBlendState bs;
		for (int i = 0; i < p_attachments; i++) {
			Attachment ba;
			ba.enable_blend = true;
			ba.src_color_blend_factor = BlendFactor::BLEND_FACTOR_SRC_ALPHA;
			ba.dst_color_blend_factor = BlendFactor::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			ba.src_alpha_blend_factor = BlendFactor::BLEND_FACTOR_SRC_ALPHA;
			ba.dst_alpha_blend_factor = BlendFactor::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

			bs.attachments.push_back(ba);
		}
		return bs;
	}

	std::vector<Attachment> attachments; // One per render target texture.
	std::array<unsigned char, 4> blend_constant;
};

enum PipelineDynamicStateFlags {
	DYNAMIC_STATE_LINE_WIDTH = (1 << 0),
	DYNAMIC_STATE_DEPTH_BIAS = (1 << 1),
	DYNAMIC_STATE_BLEND_CONSTANTS = (1 << 2),
	DYNAMIC_STATE_DEPTH_BOUNDS = (1 << 3),
	DYNAMIC_STATE_STENCIL_COMPARE_MASK = (1 << 4),
	DYNAMIC_STATE_STENCIL_WRITE_MASK = (1 << 5),
	DYNAMIC_STATE_STENCIL_REFERENCE = (1 << 6),
};