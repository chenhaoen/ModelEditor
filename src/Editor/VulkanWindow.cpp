#include <QResizeEvent>
#include <QTimer>

#include "Editor/VulkanWindow.h"

#include "Core/SceneManager.h"
#include "Core/FrameManager.h"
#include "Core/Scene.h"
#include "Core/PipelineManager.h"
#include "Core/Camera.h"
#include "Core/SkyboxNode.h"
#include "Core/Material.h"

#include "Core/Commands/CommandGroup.h"

#include "VulkanContext/VulkanContext.h"

#include "IO/ReadNode.h"

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

			PipelineManager::instance()->init();
			FrameManager::instance();

			auto skybox = readSkyboxNode("E:/code/ModelEditer/build/bin/Debug/models/cube.gltf");
			skybox->getMaterial()->m_texureID = RenderingContextDriver::instance()->createKTXTexture("E:/code/VulkanSamples/Vulkan/assets/textures/cubemap_vulkan.ktx");
			SceneManager::instance()->getCurrentScene()->setSkyBox(skybox);

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

void VulkanWindow::wheelEvent(QWheelEvent* event)
{
	SceneManager::instance()->getCurrentScene()->getCurrentCamera()->zoom(event->angleDelta().y() / 100.0);
}

bool mouseClicked = false;
static QPoint lastPoint;

void VulkanWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (!mouseClicked)
	{
		return;
	}

	QPoint offset = event->pos() - lastPoint;
	lastPoint = event->pos();

	if (offset == lastPoint)
	{
		return;
	}

	SceneManager::instance()->getCurrentScene()->getCurrentCamera()->rotate(glm::vec3(offset.y(), offset.x(), 0.0));
	//SceneManager::instance()->getCurrentScene()->getCurrentCamera()->rotate(-offset.x(), offset.y());
}



void VulkanWindow::mousePressEvent(QMouseEvent* event)
{
	switch (event->button())
	{
	case 
	Qt::LeftButton:mouseClicked = true;
		lastPoint = event->pos();
		break;
	default:
		break;
	} 
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button())
	{
	case Qt::LeftButton:mouseClicked = false;
	default:
		break;
	}
}

bool VulkanWindow::event(QEvent* event)
{
	if (!m_listEvent.empty())
	{
		for (auto event : m_listEvent)
		{
			event();
		}
		m_listEvent.clear();
	}

	if (event->type() == QEvent::UpdateRequest)
	{


		FrameManager::instance()->beginFrame();

		auto commandGroup = FrameManager::instance()->currentCommandGroup();
		commandGroup->clear();

		CommandBufferID currentCommandBuffer = FrameManager::instance()->currentCommandBuffer();

		SceneManager::instance()->compile();

		//SceneManager::instance()->update(0.0);

		SceneManager::instance()->record();

		commandGroup->record();

		FrameManager::instance()->endFrame();
		return true;
	}

	return QWindow::event(event);
}

void VulkanWindow::render()
{


	requestUpdate();
}
