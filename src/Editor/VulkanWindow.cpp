#include <QResizeEvent>
#include <QTimer>

#include "Editor/VulkanWindow.h"

#include "Nodes/NodeManager.h"

#include "VulkanContext/VulkanContext.h"

#include "RenderingContextDriver/FrameManager.h"

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

			NodeManager::root();

			RenderingContextDriver::setContext(new VulkanContext());
			RenderingContextDriver::instance()->setInitInfo(initInfo);
			RenderingContextDriver::instance()->initialize();

			FrameManager::instance();

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

	RenderingContextDriver::instance()->resizeSurface();
}

void VulkanWindow::closeEvent(QCloseEvent*)
{
	//RenderingContextDriver::instance()->wait();
}

bool VulkanWindow::event(QEvent* event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
		FrameManager::instance()->beginFrame();


		FrameManager::instance()->endFrame();
		return true;
	}
	return QWindow::event(event);
}

void VulkanWindow::render()
{
	requestUpdate();
}
