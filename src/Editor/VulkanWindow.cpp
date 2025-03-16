#include <QResizeEvent>
#include <QTimer>

#include "Editor/VulkanWindow.h"

#include "Core/SceneManager.h"

#include "VulkanContext/VulkanContext.h"

#include "RenderingContextDriver/FrameManager.h"
#include "RenderingContextDriver/PipelineManager.h"

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

			RenderingContextDriver::setContext(new VulkanContext());
			RenderingContextDriver::instance()->setInitInfo(initInfo);
			RenderingContextDriver::instance()->initialize();

			PipelineManager::instance();
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
	
}

bool VulkanWindow::event(QEvent* event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
		FrameManager::instance()->beginFrame();

		CommandBufferID currentCommandBuffer = FrameManager::instance()->currentCommandBuffer();

		const Extent2D& surfaceExtent = RenderingContextDriver::instance()->getSurfaceExtent(SurfaceID());

		RenderingContextDriver::instance()->cmdBindPipeline(currentCommandBuffer, PipelineManager::instance()->getPipeline(PipelineType::Model));

		Rect2D scissor;
		scissor.offset = { 0,0 };
		scissor.extent = surfaceExtent;
		RenderingContextDriver::instance()->cmdSetScissor(currentCommandBuffer, scissor);

		Viewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)surfaceExtent.width;
		viewport.height = (float)surfaceExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		RenderingContextDriver::instance()->cmdSetViewport(currentCommandBuffer, viewport);

		SceneManager::instance()->record();

		FrameManager::instance()->endFrame();
		return true;
	}
	return QWindow::event(event);
}

void VulkanWindow::render()
{
	requestUpdate();
}
