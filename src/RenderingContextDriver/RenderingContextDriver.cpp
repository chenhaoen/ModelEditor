#include "RenderingContextDriver/RenderingContextDriver.h"

RenderingContextDriver* RenderingContextDriver::g_instance = nullptr;

RenderingContextDriver::~RenderingContextDriver()
{

}

RenderingContextDriver* RenderingContextDriver::instance()
{
	return g_instance;
}

void RenderingContextDriver::setContext(RenderingContextDriver* driver)
{
	g_instance = driver;
}

void RenderingContextDriver::destroyContext()
{
	delete g_instance;
	g_instance = nullptr;
}

RenderingContextDriver::DriverType RenderingContextDriver::getDriverType()
{
	return DriverType::Vulkan;
}

void RenderingContextDriver::setInitInfo(const InitInfo& initInfo)
{
	m_initInfo = initInfo;
}

const InitInfo& RenderingContextDriver::getInitInfo() const
{
	return m_initInfo;
}