#include "Core/Camera.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

// 构造函数
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch), target(0.0f, 0.0f, 0.0f),
    fov(45.0f), aspectRatio(16.0f / 9.0f), nearPlane(0.1f), farPlane(100.0f), isPerspective(true) {
    updateCameraVectors();
}

Camera::~Camera()
{
    RenderingContextDriver::instance()->freeBuffer(m_buffer);
}

// 获取视图矩阵
glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 result;
    glm::mat4 rotM = glm::mat4(1.0f);
    glm::mat4 transM;

    rotM = glm::rotate(rotM, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 translation = position;
    transM = glm::translate(glm::mat4(1.0f), translation);
    result =  transM * rotM;
    return result;
}

// 获取投影矩阵
glm::mat4 Camera::getProjectionMatrix() const {
   return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

// 设置透视投影参数
void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->isPerspective = true;
}

// 移动摄像机
void Camera::move(glm::vec3 offset) {
    position += offset;
}

// 旋转摄像机
void Camera::rotate(float yaw, float pitch) {
    //this->yaw += yaw * 0.1;
    //this->pitch += pitch * 0.1;

    updateCameraVectors();
}

void Camera::rotate(const glm::vec3& delta)
{
   // std::cout << "x: " << rotation.x << ", y: " << rotation.y << std::endl;
    this->rotation += delta;
}

// 缩放视野
void Camera::zoom(float yOffset) {
    float zoomSpeed = 0.5f;

    glm::vec3 direction = glm::normalize(target - position);
    position += direction * yOffset * zoomSpeed;

    // 限制最小距离
    float minDistance = 0.5f;
    if (glm::distance(position, target) < minDistance) {
        position = target - direction * minDistance;
    }
}

// 获取摄像机位置
glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::record()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    const Extent2D& surfaceExtent = RenderingContextDriver::instance()->getSurfaceExtent(SurfaceID());
    
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.model =  glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
    
    ubo.view = getViewMatrix();
    float radius = 10.0f;
    float camX = sin(time) * radius;
    float camZ = cos(time) * radius;
    //ubo.view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    
    ubo.proj = glm::perspective(glm::radians(fov), surfaceExtent.width / (float)surfaceExtent.height, 0.1f, 1000.0f);
    
    ubo.proj[1][1] *= -1;
    
    RenderingContextDriver::instance()->updateUniformBuffer(m_buffer, &ubo, sizeof(UniformBufferObject));
}

BoundUniform Camera::getUniform() const
{
    BoundUniform boundUniform;
    boundUniform.binding = 0;
    boundUniform.type = UniformType::UNIFORM_TYPE_UNIFORM_BUFFER;
    boundUniform.ids.push_back(m_buffer);
    boundUniform.ids.push_back(0);
    boundUniform.ids.push_back(sizeof(UniformBufferObject));
    return boundUniform;
}

void Camera::compile()
{
    if (m_compiled)
    {
        return;
    }

    m_compiled = true;

    m_buffer = RenderingContextDriver::instance()->createUniformBuffer();
}

// 更新摄像机的坐标系
void Camera::updateCameraVectors() {
    // 计算前向向量
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(position);
    
    // 计算右向量和上向量
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}