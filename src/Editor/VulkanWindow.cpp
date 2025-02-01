#include <QResizeEvent>
#include <QTimer>

#include "Editor/VulkanWindow.h"

#include "VulkanContext/VulkanContext.h"

VulkanWindow::VulkanWindow()
{
	setSurfaceType(QSurface::VulkanSurface);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &VulkanWindow::requestUpdate);
	timer->start(1/60.0f);
}

void VulkanWindow::exposeEvent(QExposeEvent*)
{
	if (isExposed()) {
		if (!m_initialized) {
			m_initialized = true;

			InitInfo initInfo;
			initInfo.m_platformWindow = reinterpret_cast<void*>(winId());

#ifndef NDEBUG
			initInfo.m_debug = true;
#endif // NDEBUG

			VulkanContext::instance()->setInitInfo(initInfo);
			VulkanContext::instance()->init();

			render();
		}
	}
}

void VulkanWindow::resizeEvent(QResizeEvent* event)
{
	if (!m_initialized) {
		return;
	}

	if (event->size() == event->oldSize())
	{
		return;
	}

	VulkanContext::instance()->resizeSurface();
}

void VulkanWindow::closeEvent(QCloseEvent*)
{
	//VulkanContext::instance()->wait();
}

bool VulkanWindow::event(QEvent* event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
		VulkanContext::instance()->drawFrame();
		return true;
	}
	return QWindow::event(event);
}

void VulkanWindow::render()
{
	requestUpdate();
}
