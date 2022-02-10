#pragma once
#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/// \brief
/// This class manages the SFML window
/// \details
/// You can initialize the window with certain arguments
/// and flush rendered stuff to the screen.
/// You can also check for SFML events and receive them in a list.
class WindowManager final
{
    sf::Window             m_window;
    sf::RenderWindow       m_splashScreen;
    std::vector<sf::Event> m_events;

  public:
    /// fullscreen: render the window in fullscreen with borders
    /// title: title of the window border (only works when fullscreen disabled)
    /// antiAliasing: amount of samples for AA (0 to 128 max, recommended is 8 or 16)
    /// oglMajor: OpenGL version(such as 4, if you want to use version 4.5)
    /// oglMajor: OpenGL subversion (such as 5, if you want to use version 4.5)
    /// width and height: size of the SFML window (only works when fullscreen disabled)S
    WindowManager(bool fullscreen, const std::string& title, unsigned int antiAliasing, unsigned int oglMajor,
                  unsigned int oglMinor, unsigned int width = -1, unsigned int height = -1);

    /// Update the windowmanager, poll and store window events
    void update();

    /// Polls SFML events and return them in a list(std::vector)
    std::vector<sf::Event>& getEvents();

    /// Flushes all rendered (either SFML or OpenGL) stuff to the screen
    void flush();

    /// Get the current cursor position in the iwndow
    /// \return The x and Y coordinates of the cursor position
    glm::ivec2 getCursorPosition();

    /// Close the splash screen (for when the game has finished loading)
    void closeSplashScreen();
};
