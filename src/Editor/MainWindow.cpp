#include <QWindow>
#include <QFileDialog>

#include "Editor/MainWindow.h"
#include "Editor/VulkanWindow.h"
#include "ui_mainwindow.h"

#include "IO/ReadNode.h"

#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include "Core/Node.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWindow *vulkanWindow = new VulkanWindow();
    QWidget *wrapper = QWidget::createWindowContainer(vulkanWindow,this);
    setCentralWidget(wrapper);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered(bool)
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open Model File"), "", "*.obj");

    if (fileName.isEmpty())
    {
        return;
    }

    auto node = readNode(fileName.toStdString());
    SceneManager::instance()->getCurrentScene()->addNode(node->getName(), node);
}