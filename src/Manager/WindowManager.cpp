#include "WindowManager.hpp"

#include <GL/glew.h>
#include <Misc/Logger.hpp>

WindowManager::WindowManager(bool fullscreen, const std::string& title, unsigned int antiAliasing,
                             unsigned int oglMajor, unsigned int oglMinor, unsigned int width, unsigned int height)
    : m_window{sf::VideoMode{width, height}, title,
               static_cast<sf::Uint32>(fullscreen ? sf::Style::Fullscreen : sf::Style::Close),
               sf::ContextSettings{24, 8, antiAliasing, oglMajor, oglMinor, sf::ContextSettings::Attribute::Core}},
      m_splashScreen{sf::VideoMode{850, 480}, title, sf::Style::None}
{

    // Splash screen
    m_window.setVisible(false);
    sf::Texture splashScreen;
    splashScreen.loadFromFile("Assets/Images/SplashScreen.png");
    sf::RectangleShape frame;
    frame.setPosition({0, 0});
    frame.setSize({float(850), float(480)});
    frame.setTexture(&splashScreen);
    m_splashScreen.draw(frame);
    m_splashScreen.display();

    // Activate OpenGL context
    m_window.setActive(true);

    // GLEW setup
    glewExperimental = GL_TRUE;
    GLenum initGlew  = glewInit();
    if (initGlew != GLEW_OK)
    {
        Logger::throwError(reinterpret_cast<char const*>(glewGetErrorString(initGlew)));
    }
}

void WindowManager::update()
{
    m_events.clear();

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_events.push_back(event);
    }
}

std::vector<sf::Event>& WindowManager::getEvents()
{
    return m_events;
}

void WindowManager::flush()
{
    m_window.display();
}

glm::ivec2 WindowManager::getCursorPosition()
{
    auto pos = sf::Mouse::getPosition(m_window);
    return {pos.x, pos.y};
}

void WindowManager::closeSplashScreen()
{
    m_splashScreen.close();
    m_window.setVisible(true);
}
