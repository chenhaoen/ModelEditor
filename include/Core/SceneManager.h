#pragma once

#include "Core/Exports.h"

inline const std::string MAIN_SCENE_NAME;

inline const std::string MAIN_CAMERA_NAME;

class Scene;
class CORE_API SceneManager {
public:
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    static SceneManager* instance();
    static void destroy();

    void addScene(const std::string& name, std::shared_ptr<Scene> scene);

    void removeScene(const std::string& name);

    void switchToScene(const std::string& name);

    std::shared_ptr<Scene> getCurrentScene();

    void update(float deltaTime);

    void compile();

    void record();

    void clear();
private:
    SceneManager();
    ~SceneManager();

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> currentScene;

    static SceneManager* g_instance;
};


