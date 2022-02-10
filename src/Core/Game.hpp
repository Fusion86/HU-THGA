#pragma once

#include <memory>

#include <SFML/Window.hpp>

#include <Manager/WindowManager.hpp>
#include <Manager/CameraManager.hpp>
#include <Manager/GameplayManager.hpp>
#include <Manager/SoundManager.hpp>
#include <Manager/UnitManager.hpp>
#include <Manager/GuiManager.hpp>
#include <Manager/TerrainManager.hpp>
#include <Manager/ConstructionManager.hpp>

#include <Opponent/OpponentManager.hpp>

#include <Entity/SkyEntity.hpp>

#include <Renderer/SkyEntityRenderer.hpp>
#include <Renderer/TerrainEntityRenderer.hpp>
#include <Renderer/GameEntityRenderer.hpp>
#include <Renderer/GuiEntityRenderer.hpp>

#include <Unit/Unit.hpp>

/// \brief
/// This is the core class of the game (engine)
/// \details
/// This class holds all of the important instances such as renderers, managers and entities.
/// This is also the place where the main game-loop is started and stopped.
class Game final
{
  public:
    Game();

    /// Starts the game loop and blocks until the game has ended
    void start();

    /// Exits the game-loop
    void stop();

    static inline bool finished = false;

  private:
    /// Initializes stuff
    void m_initialize();

    /// Updates all game logic
    void m_update(const float deltaTime);

    /// Renders everything to the screen
    void m_render();

    /// Consume window events and set attributes containing information about user input
    void m_updateEvents();

    /// (Should only be called in the main menu)  Handles click and hover events for the menu
    void m_updateMainMenu(const float deltaTime);

    // Booleans
    bool m_isRunning  = true;
    bool m_wireframed = false;
    bool m_isFocused  = true; // Set true by default, because sometimes we dont receive the first focusWindow event.
    bool m_isInMenu   = true;

    // Core
    WindowManager m_windowManager;
    CameraManager m_cameraManager;

    // Renderers
    SkyEntityRenderer     m_skyEntityRenderer;
    TerrainEntityRenderer m_terrainEntityRenderer;
    GameEntityRenderer    m_gameEntityRenderer;
    GuiEntityRenderer     m_guiEntityRenderer;

    // Managers
    GameplayManager     m_gameplayManager;
    SoundManager        m_soundManager;
    UnitManager         m_unitManager;
    GuiManager          m_guiManager;
    TerrainManager      m_terrainManager;
    ConstructionManager m_constructionManager;
    OpponentManager     m_opponentManager;

    // Misc
    std::shared_ptr<SkyEntity> m_sky;

    // Event Handling
    sf::Keyboard::Key m_lastPressedKey         = sf::Keyboard::Key::KeyCount;
    sf::Mouse::Button m_lastPressedMouseButton = sf::Mouse::Button::ButtonCount;
    float             m_lastScrollWheelDelta   = 0.0f;
};
