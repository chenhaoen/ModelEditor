#include <QApplication>

#include "Editor/MainWindow.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/FrameManager.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow window;

    window.show();
    const int result = app.exec();

    RenderingContextDriver::instance()->wait();
    FrameManager::destroy();
    RenderingContextDriver::destroyContext();

    return result;
}