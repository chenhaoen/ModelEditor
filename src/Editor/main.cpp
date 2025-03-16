#include <QApplication>

#include "Editor/MainWindow.h"

#include "Core/SceneManager.h"
#include "Core/Scene.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/FrameManager.h"
#include "RenderingContextDriver/PipelineManager.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    SceneManager::instance();
    SceneManager::instance()->addScene(MAIN_SCENE_NAME, std::make_shared<Scene>());

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