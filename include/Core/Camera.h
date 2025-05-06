#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Compilable.h"
#include "Core/Exports.h"

#include "Core/RenderingContextDriver/Commons.h"

class CORE_API Camera : public Compilable {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f),
        glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f),
        float yaw = -90.0f, float pitch = 0.0f);

    ~Camera();

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

    void move(glm::vec3 offset);

    void rotate(float yaw, float pitch);

    void rotate(const glm::vec3& delta);

    void zoom(float amount);

    glm::vec3 getPosition() const;

    void compile() override;

    void record() override;

    BoundUniform getUniform() const;
private:

    void updateCameraVectors();

    glm::vec3 position; 
    glm::vec3 target;
    glm::vec3 front;
    glm::vec3 up;       
    glm::vec3 right;    
    glm::vec3 worldUp;  
    glm::vec3 rotation = glm::vec3(15.0, 0.0, 0.0);

    float yaw;  
    float pitch;

    float fov;          
    float aspectRatio;  
    float nearPlane;    
    float farPlane;     
    bool isPerspective; 

    BufferID m_buffer;
};



