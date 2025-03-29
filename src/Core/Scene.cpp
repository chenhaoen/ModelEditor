#include "Core/Scene.h"
#include "Core/Node.h"
#include "Core/Camera.h"

// 构造函数
Scene::Scene() {
    rootNode = std::make_shared<Node>("Root");
}

// 析构函数
Scene::~Scene() {
    // 自动释放智能指针管理的资源
}

// 添加节点
void Scene::addNode(const std::string& name, std::shared_ptr<Node> node) {
    nodes[name] = node;
    rootNode->addChild(node);
}

// 移除节点
void Scene::removeNode(const std::string& name) {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        rootNode->removeChild(it->second);
        nodes.erase(it);
    }
}

// 获取节点
std::shared_ptr<Node> Scene::getNode(const std::string& name) {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        return it->second;
    }
    return nullptr;
}

// 添加摄像机
void Scene::addCamera(const std::string& name, std::shared_ptr<Camera> camera) {
    cameras[name] = camera;
    if (!currentCamera) {
        currentCamera = camera;
    }
}

// 设置当前摄像机
void Scene::setCurrentCamera(const std::string& name) {
    auto it = cameras.find(name);
    if (it != cameras.end()) {
        currentCamera = it->second;
    }
}

// 获取当前摄像机
std::shared_ptr<Camera> Scene::getCurrentCamera() {
    return currentCamera;
}

// 添加灯光
void Scene::addLight(const std::string& name, std::shared_ptr<Light> light) {
    lights[name] = light;
}

// 获取灯光
std::shared_ptr<Light> Scene::getLight(const std::string& name) {
    auto it = lights.find(name);
    if (it != lights.end()) {
        return it->second;
    }
    return nullptr;
}

// 更新场景
void Scene::update(float deltaTime) {
    // 更新所有节点
    //rootNode->update();

    // 更新摄像机
    if (currentCamera) {
        //currentCamera->
    }

    // 更新灯光
    for (auto& light : lights) {
        // 例如：根据时间或逻辑更新灯光属性
    }
}

void Scene::compile()
{
    if (currentCamera) {
        currentCamera->compile();
    }

    rootNode->compile();
}

void Scene::record()
{
    if (currentCamera) {
        currentCamera->record();
    }

    rootNode->record();
}
