#include "Renderer.h"
#include "game/Application.h"
#include "game/graphics/gl.hpp"
#include "game/utils/Logger.h"
#include <chrono>

Renderer::Renderer(HDC hDC, const dimension_t<int>& size)
    : m_hDC(hDC)
    , m_size(size)
    , m_working(false)
    , m_thread()
{
}

Renderer::~Renderer()
{
    stop();
}

void Renderer::start()
{
    if (m_working)
        return;
    m_working = true;
    m_thread.reset(new std::thread(std::bind(&Renderer::work, this)));
}

void Renderer::stop()
{
    if (!m_working)
        return;
    m_working = false;
    m_thread->join();
    m_thread.reset();
}

void Renderer::work()
{
    // Создаем render context (RC)
    const auto hRC = wglCreateContext(m_hDC);
    const bool is_ok = wglMakeCurrent(m_hDC, hRC);
    Logger::instance().info("Создание контекста OpenGL {}", (is_ok ? "ok" : "err"));
    // Инициализируем графику
    glbinding::initialize(nullptr);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_size.w, m_size.h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Инициализируем приложение
    Application::instance().initialize(m_size, m_hDC, hRC);

    //???? Application::instance().m_GUI->MapViewer->m_center.x = Application::instance().m_GUI->MapViewer->m_player->m_object->cell()->x;
    //???? Application::instance().m_GUI->MapViewer->m_center.y = Application::instance().m_GUI->MapViewer->m_player->m_object->cell()->y;

    // Запускаем потоки приложения

    //???? Application::instance().start();

    const std::chrono::milliseconds normal_duration(1000 / 60);
    // Выполняем графический цикл, пока renderer не будет остановлен
    while (m_working) {
        auto start = std::chrono::system_clock::now();
        //if (Application::instance().m_GUI) { if (Application::instance().m_GUI->MapViewer) { Application::instance().m_GUI->MapViewer->update(); } }
        Application::instance().render();
        SwapBuffers(m_hDC);
        auto end = std::chrono::system_clock::now();

        // если на отрисовку ушло больше normal_duration, никакой задержки не должно быть
        if (start + normal_duration < end)
            continue;
        auto duration(std::chrono::duration_cast<std::chrono::microseconds>(end - start));
        std::this_thread::sleep_for(normal_duration - duration);
    }
    // renderer остановлен, останавливаем потоки приложения
    Application::instance().stop();

    // Разрушаем графический контекст OpenGL
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hRC);
}