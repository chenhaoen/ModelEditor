#include "Core/Scene.h"
#include "Core/Node.h"
#include "Core/Camera.h"

// ���캯��
Scene::Scene() {
    rootNode = std::make_shared<Node>("Root");
}

// ��������
Scene::~Scene() {
    // �Զ��ͷ�����ָ��������Դ
}

// ��ӽڵ�
void Scene::addNode(const std::string& name, std::shared_ptr<Node> node) {
    nodes[name] = node;
    rootNode->addChild(node);
}

// �Ƴ��ڵ�
void Scene::removeNode(const std::string& name) {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        rootNode->removeChild(it->second);
        nodes.erase(it);
    }
}

// ��ȡ�ڵ�
std::shared_ptr<Node> Scene::getNode(const std::string& name) {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        return it->second;
    }
    return nullptr;
}

// ��������
void Scene::addCamera(const std::string& name, std::shared_ptr<Camera> camera) {
    cameras[name] = camera;
    if (!currentCamera) {
        currentCamera = camera;
    }
}

// ���õ�ǰ�����
void Scene::setCurrentCamera(const std::string& name) {
    auto it = cameras.find(name);
    if (it != cameras.end()) {
        currentCamera = it->second;
    }
}

// ��ȡ��ǰ�����
std::shared_ptr<Camera> Scene::getCurrentCamera() {
    return currentCamera;
}

// ��ӵƹ�
void Scene::addLight(const std::string& name, std::shared_ptr<Light> light) {
    lights[name] = light;
}

// ��ȡ�ƹ�
std::shared_ptr<Light> Scene::getLight(const std::string& name) {
    auto it = lights.find(name);
    if (it != lights.end()) {
        return it->second;
    }
    return nullptr;
}

// ���³���
void Scene::update(float deltaTime) {
    // �������нڵ�
    //rootNode->update();

    // ���������
    if (currentCamera) {
        //currentCamera->
    }

    // ���µƹ�
    for (auto& light : lights) {
        // ���磺����ʱ����߼����µƹ�����
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
