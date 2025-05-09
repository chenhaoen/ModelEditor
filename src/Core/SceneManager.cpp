#include "Core/SceneManager.h"
#include "Core/Scene.h"

SceneManager* SceneManager::g_instance = nullptr;

// 构造函数
SceneManager::SceneManager() : currentScene(nullptr) {}

// 析构函数
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
        currentScene = scene; // 如果没有当前场景，设置为第一个添加的场景
    }
}

void SceneManager::removeScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        if (currentScene == it->second) {
            currentScene = nullptr; // 如果移除的是当前场景，清空当前场景
        }
        scenes.erase(it);
    }
}

// 切换场景
void SceneManager::switchToScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        currentScene = it->second;
    }
    else {
        // 场景不存在，可以抛出异常或记录日志
        throw std::runtime_error("Scene not found: " + name);
    }
}

// 获取当前场景
std::shared_ptr<Scene> SceneManager::getCurrentScene() {
    return currentScene;
}

// 更新当前场景
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

// 渲染当前场景
void SceneManager::record() {
    if (currentScene) {
        currentScene->record();
    }
}

void SceneManager::clear()
{
    scenes.clear();
}
