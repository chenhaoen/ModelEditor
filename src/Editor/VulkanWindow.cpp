#include "Editor/VulkanWindow.h"

#include "VulkanContext/VulkanContext.h"

VulkanWindow::VulkanWindow()
{
    setSurfaceType(QSurface::VulkanSurface);
}

void VulkanWindow::exposeEvent(QExposeEvent *)
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

bool VulkanWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest)
    {
        render();
        return true;
    }
    return QWindow::event(event);
}

void VulkanWindow::render()
{
    requestUpdate();
}
