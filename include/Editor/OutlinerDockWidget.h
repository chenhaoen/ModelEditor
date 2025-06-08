#ifndef OUTLINERDOCKWIDGET_H
#define OUTLINERDOCKWIDGET_H

namespace Ui {
	class OutlinerDockWidget;
}

class Node;
class NodeListModel;

class OutlinerDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	explicit OutlinerDockWidget(QWidget* parent = nullptr);
	~OutlinerDockWidget();

	void addNode(std::shared_ptr<Node>);

private:
	Ui::OutlinerDockWidget* ui;

	NodeListModel* m_nodelistModel;
};

#endif // OUTLINERDOCKWIDGET_H
