#include <QApplication>

#include "Editor/MainWindow.h"

#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include "Core/Camera.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/FrameManager.h"
#include "RenderingContextDriver/PipelineManager.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    SceneManager::instance();
    auto scene = std::make_shared<Scene>();
    auto camera = std::make_shared<Camera>();
    scene->addCamera(MAIN_CAMERA_NAME, camera);
    SceneManager::instance()->addScene(MAIN_SCENE_NAME, scene);
    scene.reset();

    MainWindow window;
    window.show();
    const int result = app.exec();

    RenderingContextDriver::instance()->wait();
    FrameManager::destroy();
    PipelineManager::destroy();
    SceneManager::destroy();
    RenderingContextDriver::destroyContext();

    return result;
}