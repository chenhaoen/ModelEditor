#include "Core/SceneManager.h"
#include "Core/Scene.h"

SceneManager* SceneManager::g_instance = nullptr;

// ���캯��
SceneManager::SceneManager() : currentScene(nullptr) {}

// ��������
SceneManager::~SceneManager() {
    currentScene.reset();
}

SceneManager* SceneManager::instance() {
    if (g_instance == nullptr)
    {
        g_instance = new SceneManager();
    }

    return g_instance;
}

void SceneManager::destroy()
{
    delete g_instance;
}

void SceneManager::addScene(const std::string& name, std::shared_ptr<Scene> scene) {
    scenes[name] = scene;
    if (!currentScene) {
        currentScene = scene; // ���û�е�ǰ����������Ϊ��һ����ӵĳ���
    }
}

void SceneManager::removeScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        if (currentScene == it->second) {
            currentScene = nullptr; // ����Ƴ����ǵ�ǰ��������յ�ǰ����
        }
        scenes.erase(it);
    }
}

// �л�����
void SceneManager::switchToScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        currentScene = it->second;
    }
    else {
        // ���������ڣ������׳��쳣���¼��־
        throw std::runtime_error("Scene not found: " + name);
    }
}

// ��ȡ��ǰ����
std::shared_ptr<Scene> SceneManager::getCurrentScene() {
    return currentScene;
}

// ���µ�ǰ����
void SceneManager::update(float deltaTime) {
    if (currentScene) {
        currentScene->update(deltaTime);
    }
}

void SceneManager::compile() {
    if (currentScene) {
        currentScene->compile();
    }
}

// ��Ⱦ��ǰ����
void SceneManager::record() {
    if (currentScene) {
        currentScene->record();
    }
}

void SceneManager::clear()
{
    scenes.clear();
}
