#pragma once

#include <QWindow>

class VulkanWindow : public QWindow
{
    public:
    VulkanWindow();

protected:
    void exposeEvent(QExposeEvent *) override;

    bool event(QEvent *event) override;

    void render();

private:
    bool m_initialized = false;
};