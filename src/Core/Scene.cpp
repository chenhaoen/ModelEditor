#include "Core/Scene.h"
#include "Core/Node.h"
#include "Core/Camera.h"
#include "Core/RenderingContextDriver/RenderingContextDriver.h"
#include "Core/SkyboxNode.h"
#include "Core/GridNode.h"
#include "Core/BackgroundNode.h"
#include "Core/NodeTree.h"

#include "IO/ReadNode.h"

// ���캯��
Scene::Scene()
    :m_fillMode(FillMode::SOLID)
{
    rootNode = std::make_shared<Node>("Root");

    m_nodeTree = std::make_shared<NodeTree>();
}

// ��������
Scene::~Scene() {
    // �Զ��ͷ�����ָ��������Դ
}

// ��ӽڵ�
void Scene::addNode(const std::string& name, std::shared_ptr<Node> node) {
    nodes[name] = node;
    rootNode->addChild(node);

    m_nodeTree->addNode(node);
}

// �Ƴ��ڵ�
void Scene::removeNode(const std::string& name) {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        rootNode->removeChild(it->second);
        nodes.erase(it);
    }
}

std::shared_ptr<NodeTree> Scene::getNodeTree()
{
    return m_nodeTree;
}

void Scene::setSkyBox(std::shared_ptr<SkyboxNode> node)
{
    m_skyboxNode = node;
}

void Scene::setGrid(std::shared_ptr<GridNode> node)
{
    m_gridNode = node;
}

void Scene::setBackground(std::shared_ptr<BackgroundNode> node)
{
    m_backgroundNode = node;
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

void Scene::setFillMode(FillMode fillMode)
{
    m_fillMode = fillMode;
}

FillMode Scene::getFillMode() const
{
    return m_fillMode;
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

    if (m_skyboxNode)
    {
        m_skyboxNode->compile();
    }

    if (m_gridNode)
    {
        m_gridNode->compile();
    }

    if (m_backgroundNode)
    {
        m_backgroundNode->compile();
    }

    m_nodeTree->compile();
}

void Scene::record()
{
    if (currentCamera) {
        currentCamera->record();
    }

    if (m_backgroundNode)
    {
        m_backgroundNode->record();
    }

    if (m_skyboxNode)
    {
        m_skyboxNode->record();
    }

    if (m_gridNode)
    {
        m_gridNode->record();
    }


    m_nodeTree->record();
   // rootNode->record();
}
