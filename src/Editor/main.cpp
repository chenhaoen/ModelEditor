#include <QApplication>

#include "Editor/MainWindow.h"

#include "VulkanContext/VulkanContext.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow window;

    window.show();
    const int result = app.exec();

    VulkanContext::instance()->destroy();

    return result;
}