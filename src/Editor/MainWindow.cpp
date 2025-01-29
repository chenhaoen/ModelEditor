#include <QWindow>

#include "Editor/MainWindow.h"
#include "Editor/VulkanWindow.h"
#include "ui_mainwindow.h"

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
