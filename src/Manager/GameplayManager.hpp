#pragma once

#include <map>
#include <memory>

#include <Misc/Globals.hpp>

#include <SFML/Window.hpp>

class ConstructionManager;
class GuiManager;
class CameraManager;
class GridSquare;
class UnitManager;
class TerrainEntity;
class Worker;

/// \brief
/// This is the class for gameplay controlling
/// \details
/// This class serves as a gameplay class in which the player can perform all his actions.
/// It also ties up independent classes (such as GuiManager & UnitManager), so that there is the least amount of
/// coupling possible.
class GameplayManager final
{
  private:
    const float m_cameraSpeedFactor = 0.02f;
    bool        m_gameFinished      = false;

    UnitManager&         m_unitManager;
    ConstructionManager& m_constructionManager;
    GuiManager&          m_guiManager;
    CameraManager&       m_cameraManager;

    std::map<Globals::side, ResourcesCollection> m_resources        = {{Globals::side::LEFT, {500, 500, 500, 500}},
                                                                {Globals::side::RIGHT, {500, 500, 500, 500}}};
    std::map<Globals::side, ResourcesCollection> m_workerAssignment = {{Globals::side::LEFT, {0, 0, 0, 0}},
                                                                       {Globals::side::RIGHT, {0, 0, 0, 0}}};
    std::map<Globals::side, ResourcesCollection> m_workerGoal       = {{Globals::side::LEFT, {0, 0, 0, 0}},
                                                                 {Globals::side::RIGHT, {0, 0, 0, 0}}};

    // Selected grid
    std::shared_ptr<GridSquare> m_selectedGrid = nullptr;
    std::shared_ptr<GridSquare> m_hoveredGrid  = nullptr;

    /// Get the lane the user is currently hovering
    /// \param cursorPos Current Position of the cursor within the window
    /// \return The hovered lane
    Globals::lane getSelectedLane(glm::ivec2 cursorPos);

    /// Move the camera
    /// \param speedX The speed at which the camera moves on the x axis (can be negative)
    /// \param speedZ The speed at which the camera moves on the z axis (can be negative)
    /// \param deltaTime The deltatime, used for correctly calculating speed
    void moveCamera(float speedX, float speedZ, float deltaTime);

    /// Update the current castle healths
    void m_updateCastleHealth();

    /// Update 3D selection
    ///
    /// \details This decides which gridsquare/construction is currently hovered/clicked
    /// \param cursorPos The current cursor position within the window
    /// \param mouseButton The mouse button that is currently pressed
    void m_update3DSelection(const glm::ivec2& cursorPos, const sf::Mouse::Button& mouseButton);

    /// Update camera rendering
    /// \param deltaTime
    /// \param scrollWheelDelta
    void m_updateCamera(const float deltaTime, float scrollWheelDelta);

    /// Update 2D selection, handles GUI events
    /// \param mouseButton The pressed MouseButton
    void m_update2DSelection(const sf::Mouse::Button& mouseButton);

    /// Function used in GUI selection to increase/decrease worker goals
    ///
    /// \details Fails if the workercount would get below 0, or the total worker goal would exceed the number of units.
    /// \param type Unittype to update
    /// \param increase True if the amount needs to increase
    /// \return True if the worker goal was changed, false if it couldn't be changed
    bool m_changeWorkerGoal(ResourceType type, bool increase);

  public:
    /// Create the gameplaymanager
    /// \param unit Unitmanager to use for spawning units
    /// \param construction Constructionmanager used for creating constructions
    /// \param gui GuiManager used for interpreting Input events
    /// \param camera Cameramanager to be able to change camera position on keypress
    GameplayManager(UnitManager& unit, ConstructionManager& construction, GuiManager& gui, CameraManager& camera);

    /// Update player gameplay & actions
    void update(const float deltaTime, glm::ivec2 cursorPos, sf::Keyboard::Key key, sf::Mouse::Button mouseButton,
                bool isFocused, TerrainEntity& friendlyCastle, float scrollWheelDelta);

    /// Get the current resource wallet of a player
    /// \param player Player to get resourcecollection for
    /// \return The resourcecollection
    ResourcesCollection& getResources(const Globals::side& player);

    /// Get the amount of a resource of a worker's owner
    ///
    /// \details Retrieves the resource the worker is currently assigned to
    /// \param worker Worker to check for
    /// \return The amount of the assigned resource
    int&                 getResource(const Worker& worker);

    /// Get the currently assigned worker counts of a player
    /// \param player Player to check for
    /// \return A resource collection containing the amount of workers assigned to each resource
    ResourcesCollection& getWorkerAssignment(const Globals::side& player);

    /// Get the amount of workers owned by an owner assigned to a workers target resource
    /// \param worker The worker to find count for, owner is extracted from this worker
    /// \return The amount of workers assigned to the resource
    int&                 getWorkerAssignment(const Worker& worker);

    /// Get the amount of workers owned by an owner that should be assigned to each resource
    /// \param worker The worker to find count for, owner is extracted from this worker
    /// \return A ResourcesCollection containing the amounts of workers that should be assigned to each resource
    ResourcesCollection& getWorkerGoal(const Globals::side& player);

    /// Get the amount of workers owned by an owner that should be assigned to a workers target resource
    /// \param worker The worker to find count for, owner is extracted from this worker
    /// \return The amount of workers that should be assigned to the resource
    int&                 getWorkerGoal(const Worker& worker);
};
