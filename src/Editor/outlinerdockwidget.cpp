#include "Editor/OutlinerDockWidget.h"
#include "Editor/NodeListModel.h"


#include "Core/Node.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"

#include "ui_outlinerdockwidget.h"

OutlinerDockWidget::OutlinerDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::OutlinerDockWidget)
{
    ui->setupUi(this);

    m_nodelistModel = new NodeListModel(SceneManager::instance()->getCurrentScene()->getNodeTree(), this);
    ui->listView->setModel(m_nodelistModel);
}

OutlinerDockWidget::~OutlinerDockWidget()
{
    delete ui;
}

void OutlinerDockWidget::addNode(std::shared_ptr<Node> node)
{
    m_nodelistModel->addNode(node);
}
