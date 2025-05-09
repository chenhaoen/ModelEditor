#ifndef OUTLINERDOCKWIDGET_H
#define OUTLINERDOCKWIDGET_H

namespace Ui {
	class OutlinerDockWidget;
}

class OutlinerDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	explicit OutlinerDockWidget(QWidget* parent = nullptr);
	~OutlinerDockWidget();

private slots:
	void AddCube_triggered(bool);

private:
	Ui::OutlinerDockWidget* ui;
};

#endif // OUTLINERDOCKWIDGET_H
