#include <chrono>

#include "Core/Camera.h"

#include "RenderingContextDriver/RenderingContextDriver.h"

// ���캯��
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch), front(glm::vec3(-2.0f, -2.0f, -2.0f)),
    fov(45.0f), aspectRatio(16.0f / 9.0f), nearPlane(0.1f), farPlane(100.0f), isPerspective(true) {
    updateCameraVectors();
}

Camera::~Camera()
{
    RenderingContextDriver::instance()->freeBuffer(m_buffer);
}

// ��ȡ��ͼ����
glm::mat4 Camera::getViewMatrix() const {
     return glm::lookAt(position, position + front, up);
}

// ��ȡͶӰ����
glm::mat4 Camera::getProjectionMatrix() const {
   return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

// ����͸��ͶӰ����
void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->isPerspective = true;
}

// �ƶ������
void Camera::move(glm::vec3 offset) {
    position += offset;
}

// ��ת�����
void Camera::rotate(float yaw, float pitch) {
    this->yaw += yaw;
    this->pitch += pitch;

    // ���Ƹ����ǣ������������ת
    if (this->pitch > 89.0f) this->pitch = 89.0f;
    if (this->pitch < -89.0f) this->pitch = -89.0f;

    updateCameraVectors();
}

// ������Ұ
void Camera::zoom(float amount) {
    fov -= amount;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

// ��ȡ�����λ��
glm::vec3 Camera::getPosition() const {
    return position;
}

// ��ȡ�����ǰ������
glm::vec3 Camera::getFront() const {
    return front;
}

void Camera::record()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    const Extent2D& surfaceExtent = RenderingContextDriver::instance()->getSurfaceExtent(SurfaceID());
    
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    ubo.view = getViewMatrix();
    ubo.view = glm::lookAt(glm::vec3(2.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    ubo.proj = glm::perspective(glm::radians(45.0f), surfaceExtent.width / (float)surfaceExtent.height, 0.1f, 10.0f);
    
    ubo.proj[1][1] *= -1;
    
    RenderingContextDriver::instance()->updateUniformBuffer(m_buffer, &ubo, sizeof(UniformBufferObject));
}

BoundUniform Camera::getUniform() const
{
    BoundUniform boundUniform;
    boundUniform.binding = 0;
    boundUniform.type = UNIFORM_TYPE_UNIFORM_BUFFER;
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

// ���������������ϵ
void Camera::updateCameraVectors() {
    // ����ǰ������
    //glm::vec3 newFront;
    //newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //newFront.y = sin(glm::radians(pitch));
    //newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //front = glm::normalize(newFront);
    //
    //// ������������������
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}