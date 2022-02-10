#pragma once

#include <string>
#include <glm/glm.hpp>
#include <map>
#include "ResourcesCollection.hpp"
#include <memory>

using glm::vec3;
using std::string;

using STRING = const std::string&;

class Globals
{
  public:
    Globals() = delete;

    /// Half of the map size (or the size of a player's half of the map).
    static inline const float HALF_MAP_SIZE = 25.0f;

    /// Distance from the edge of the map where no buildings can be placed since they would be in the castle.
    static inline const float CASTLE_OFFSET = 2.0f;

    /// Amount of gridsquares that will be in the player's half of the map.
    static inline const int GRIDSQUARE_COUNT = 10;

    /// Should the game window be in fullscreen.
    static inline const bool WIN_FULLSCREEN = false;

    /// Title of the game window.
    static inline const string WIN_TITLE = "RTS Game 2";

    /// Width of the game window.
    static inline const int WIN_WIDTH = 1920;

    /// Height of the game window.
    static inline const int WIN_HEIGHT = 1080;

    /// Field of view of the camer.
    static inline const float CAM_FOV = 90.0f;

    /// Nearest z position for which objects are visible.
    static inline const float CAM_NEAR = 0.1f;

    /// Furthest z position for which objects are visible.
    static inline const float CAM_FAR = 100.0f;

    /// Initial camera position.
    static inline const vec3 CAM_POS = vec3(0.0f, 4.5f, 0.0f);

    /// Amount of milliseconds between each frame of sprite animations.
    static inline const float ANIMATION_DELTA = 50.0f;

    /// Minimum distance friendly units should keep between eachother while walking.
    static inline const float MIN_UNIT_DISTANCE = 0.4f;

    /// Distance at which a building can be entered.
    static inline const float BUILDING_ENTER_DISTANCE = 0.4f;

    /// Color of hovered buildings/gridsquares.
    static inline const vec3 HOVER_COLOR = vec3(0.5, 1.0, 0.5);

    /// Color of selected buildings/gridsquares.
    static inline const vec3 SELECTED_COLOR = vec3(0.5, 0.5, 1.0);

    /// Interval between AI player update in milliseconds.
    static inline const int UPDATE_TICK_INTERVAL = 250;

    /// Cooldown in milliseconds between each worker collection of resources.
    static inline const float WORKER_COLLECTION_COOLDOWN_MS = 1000.0;

    /// Amount of resources a worker gets per collection of a specific resource.
    static inline const ResourcesCollection RESOURCE_PER_COLLECTION{5, 5, 1, 1};

    /// Size of the GUI
    static inline const float GUI_SIZE = 0.075f;

    /// Enum for distinguishing sides or players. Left is the player whereas right is the opponent/bot.
    enum class side : int
    {
        LEFT  = 0,
        RIGHT = 1
    };

    /// Return LEFT if the input is RIGHT, or RIGHT if the input is LEFT.
    static side invertSide(side s)
    {
        switch (s)
        {
            case side::LEFT:
                return side::RIGHT;
            case side::RIGHT:
                return side::LEFT;
        }
    }

    /// Enum for distinguishing lanes, NONE can be used for no lanes.
    enum class lane : int
    {
        NONE = 0,
        TOP  = 1,
        MID  = 2,
        BOT  = 3
    };

    /// Map of the Y positions for spawnables on a lane.
    static inline const std::map<lane, float> LANE_Y{std::make_pair(lane::TOP, 3.75f), std::make_pair(lane::MID, 2.50f),
                                                     std::make_pair(lane::BOT, 1.5f), std::make_pair(lane::NONE, 0.f)};

    /// Map of the Z positions for spawnables on a lane.
    static inline const std::map<lane, float> LANE_Z{
        std::make_pair(lane::TOP, -6.25f), std::make_pair(lane::MID, -5.0f), std::make_pair(lane::BOT, -3.75f),
        std::make_pair(lane::NONE, 0.f)};

    /// Distance at which units should wait in front of pathblocking constructions.
    static const inline float CONSTRUCTION_BLOCKING_DISTANCE = 1.0f;
};
