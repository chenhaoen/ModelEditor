#pragma once

#include <QWindow>

class VulkanWindow : public QWindow
{
public:
	VulkanWindow();

protected:
	void exposeEvent(QExposeEvent*) override;

	void resizeEvent(QResizeEvent*) override;

	void closeEvent(QCloseEvent*) override;

	void wheelEvent(QWheelEvent*) override;

	void mouseMoveEvent(QMouseEvent*) override;

	void mousePressEvent(QMouseEvent*)override;
	void mouseReleaseEvent(QMouseEvent*)override;

	bool event(QEvent* event) override;

	void render();

private:
	bool m_initialized = false;
};