#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Core/Exports.h"

class Node;
class Light;
class Camera;

class CORE_API Scene {
public:
    Scene();
    ~Scene();

    void addNode(const std::string& name, std::shared_ptr<Node> node);

    void removeNode(const std::string& name);

    std::shared_ptr<Node> getNode(const std::string& name);

    void addCamera(const std::string& name, std::shared_ptr<Camera> camera);

    void setCurrentCamera(const std::string& name);

    std::shared_ptr<Camera> getCurrentCamera();

    void addLight(const std::string& name, std::shared_ptr<Light> light);

    std::shared_ptr<Light> getLight(const std::string& name);

    void update(float deltaTime);

    void record();
private:
    std::shared_ptr<Node> rootNode;

    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;

    std::unordered_map<std::string, std::shared_ptr<Camera>> cameras;
    std::shared_ptr<Camera> currentCamera;

    std::unordered_map<std::string, std::shared_ptr<Light>> lights;
};

