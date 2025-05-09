#include "Editor/OutlinerDockWidget.h"

#include "IO/ReadNode.h"

#include "Core/Node.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"

#include "ui_outlinerdockwidget.h"

OutlinerDockWidget::OutlinerDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::OutlinerDockWidget)
{
    ui->setupUi(this);

    connect(ui->toolButton, &QToolButton::clicked, this, &OutlinerDockWidget::AddCube_triggered);
}

OutlinerDockWidget::~OutlinerDockWidget()
{
    delete ui;
}

void OutlinerDockWidget::AddCube_triggered(bool)
{
    const QString& absolutePath = QCoreApplication::applicationDirPath();
    const QString& relativePath = "Models/cube.gltf";

    const QString& combined = QDir::cleanPath(absolutePath + QDir::separator() + relativePath);
    auto node = readNode(combined.toStdString());
    SceneManager::instance()->getCurrentScene()->addNode(node->getName(), node);
}