#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>

#include <Misc/Globals.hpp>

class GuiEntity;
class GuiEntityRenderer;


struct Text
{
    std::string                             id;
    float                                   x;
    float                                   y;
    float                                   w;
    float                                   h;
    std::vector<std::shared_ptr<GuiEntity>> chars;

    /// Create a GUI text element
    /// \param id ID for the GUI element
    /// \param chars String of characters
    /// \param x X position of the text
    /// \param y Y position of the text
    /// \param w Width of the text
    /// \param h Height of the text
    Text(std::string id, const std::string& chars, float x, float y, float w, float h)
        : id(std::move(id)), x(x), y(y), w(w), h(h)
    {
        setChars(chars);
    }

    /// Change the text that is displayed
    /// \param chrs Characters to display
    void setChars(const std::string& chrs);
};

struct GuiWindow
{
    const std::string ID;

    std::vector<std::shared_ptr<Text>>      texts;
    std::vector<std::shared_ptr<GuiEntity>> components;

    /// Create a Gui Window
    /// \param id String id of the Window
    GuiWindow(std::string  id) : ID(std::move(id)) {}

    /// Create a GUI element
    /// \param id string Id of the element
    /// \param x X position of the element
    /// \param y Y position of the element
    /// \param w Width of the element
    /// \param h Height of the element
    /// \param texFile
    /// \param texfiltering
    void createElement(STRING id, float x, float y, float w, float h, STRING texFile, bool texfiltering);

    /// Create a button
    /// \param id String id of the button
    /// \param x X position of the element
    /// \param y Y position of the element
    /// \param w Width of the element
    /// \param h Height of the element
    /// \param texFile
    /// \param texfiltering
    void createButton(STRING id, float x, float y, float w, float h, STRING texFile, bool texfiltering);

    /// Create a text element
    /// \param id String id of the button
    /// \param chars Text to be displayed
    /// \param x X position of the element
    /// \param y Y position of the element
    /// \param w Width of the element
    /// \param h Height of the element
    void createText(STRING id, const STRING chars, float x, float y, float w, float h);
};


float convSize(float size);

class GuiManager final
{
  private:
    std::shared_ptr<GuiWindow>              m_hud;
    std::shared_ptr<GuiWindow>              m_resourceWindow;
    std::vector<std::shared_ptr<GuiWindow>> m_windows;
    std::string                             m_selectedWindow = "";
    bool                                    m_hudEnabled     = false;

    /// Render a single window using the given renderer
    /// \param renderer  Renderer to be used
    /// \param window Window to render
    void m_renderWindow(GuiEntityRenderer& renderer, const GuiWindow& window);

    /// Convert cursor coordinates to NDC
    /// \param cursorPos cursor coordinates to convert
    /// \return
    glm::vec2 m_convertToNdc(glm::ivec2 cursorPos);

    /// Update the hover state for all elements in a window in the GUImanager
    /// \param window Window to check hover states for
    /// \param x Cursor x Position to check for
    /// \param y Cursor y Position to check for
    void m_updateHoverState(std::shared_ptr<GuiWindow> window, float x, float y);

  public:
    /// Create the GUI manager
    GuiManager();

    /// Update the GUI manager, gets the cursos position, and updates hover states
    /// \param deltaTime
    /// \param cursorPos Current position of the cursor
    void update(float deltaTime, glm::ivec2 cursorPos);

    /// Updates the heads-up resource/health/worker counts
    /// \param resources Current resource inventory of the user
    void updateHud(const ResourcesCollection& resources);

    /// Update the current playerGoal UI
    /// \param resources Current Player worker goal
    void updatePlayerWorkerAssignmentWindow(const ResourcesCollection& resources);

    /// Change the health of the friendly castle
    void setFriendlyCastleHealth(int health);

    /// Change the health of the enemy castle
    void setEnemyCastleHealth(int health);

    /// Change the IDLE worker count GUI element
    /// \param count New idle worker count
    void setIdleWorkerCount(int count);

    /// Change the currently selected window
    /// \param id ID of the window to select
    void selectWindow(const std::string& id);

    /// Get a shared pointer to the currently selected window
    /// \return The currently selected window
    std::shared_ptr<GuiWindow> getSelectedWindow();

    /// Checks if the cursor is in the currently selected window at the moment
    /// \param cursorPos Position of the cursor
    /// \return True if the cursor is in the selected window
    bool isCursorInWindow(glm::ivec2 cursorPos);

    /// Check if a button is currently hovered
    /// \param buttonId ID of the button to check for
    /// \return
    bool isButtonHovered(const std::string& buttonId);

    /// Add a window to the GUI
    /// \param window Pointer to a window to add
    void addWindow(std::shared_ptr<GuiWindow> window);

    /// Check if the HUD is enabled
    /// \return True if the HUD is enabled
    [[nodiscard]] bool isHudEnabled() const;

    /// Enable or disable the HUD
    void setHudEnabled(bool hudEnabled);

    /// Format a string as a spawn button ID
    /// \param id Id of an entity that would be spawned
    /// \return
    static std::string spawnButtonStr(const std::string& id);

    /// Render all windows using the given renderer
    /// \param renderer The renderer
    void renderAll(GuiEntityRenderer& renderer);
};

struct CreationWindow : public GuiWindow
{
    std::string constructionId;
    float       size;
    float       currentX = -0.9f;

    /// Make a creation window
    /// \param constructionId ID of the construction the window is for
    /// \param size
    CreationWindow(const string& constructionId, float size);

    /// Add a spawn button for a unit to this window
    void addSpawnButton(const std::string& unitId);
};
