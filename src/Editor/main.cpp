#include "Editor/MainWindow.h"

#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/FrameManager.h"
#include "Core/PipelineManager.h"
#include "Core/Shader/ShaderManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    SceneManager::instance();
    PipelineManager::instance();
    ShaderManager::instance();
    auto scene = std::make_shared<Scene>();
    auto camera = std::make_shared<Camera>();
    scene->addCamera(MAIN_CAMERA_NAME, camera);
    SceneManager::instance()->addScene(MAIN_SCENE_NAME, scene);
    scene.reset();
    camera.reset();

    QFile f("E:/code/QDarkStyleSheet/qdarkstyle/dark/darkstyle.qss");

    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

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