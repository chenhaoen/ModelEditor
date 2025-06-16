#include "Editor/MainWindow.h"
#include "Editor/VulkanWindow.h"
#include "Editor/OutlinerDockWidget.h"
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

    m_outlinerWidget = new OutlinerDockWidget();
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_outlinerWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Fill_triggered(bool)
{
    SceneManager::instance()->getCurrentScene()->setFillMode(FillMode::SOLID);
}

void MainWindow::on_action_Mesh_triggered(bool)
{
    SceneManager::instance()->getCurrentScene()->setFillMode(FillMode::WIREFRAME);
}

void MainWindow::on_action_Point_triggered(bool)
{
    SceneManager::instance()->getCurrentScene()->setFillMode(FillMode::POINT);
}

void MainWindow::on_actionSceneShoot_triggered(bool)
{
    SceneManager::instance()->getCurrentScene();
}

void MainWindow::on_actionCube_triggered(bool)
{
    const QString& appFilePath = qApp->applicationFilePath();

    std::filesystem::path filePath = appFilePath.toStdString();
    std::filesystem::path dirPath = filePath.parent_path();

    auto node = readNode(dirPath.string() + "/models/sphere.gltf");
    m_outlinerWidget->addNode(node);
}

void MainWindow::on_action_Open_triggered(bool)
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open Model File"), "", "*.obj;*.gltf");

    if (fileName.isEmpty())
    {
        return;
    }

    auto node = readNode(fileName.toStdString());
    SceneManager::instance()->getCurrentScene()->addNode(node->getName(), node);
}