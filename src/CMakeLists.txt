set(GLEW_USE_STATIC_LIBS 1)

file(GLOB_RECURSE MyGame_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(MyGame ${MyGame_SRC})

if (MSVC)
    add_definitions(/MP12)
    set(SFML_STATIC_LIBRARIES TRUE)
    set_property(TARGET MyGame PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin") # Set Visual Studio working directory (when debugging)
endif ()

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(glm REQUIRED)

include_directories(${GLM_INCLUDE_DIR})

target_compile_definitions(MyGame PUBLIC GLEW_STATIC SFML_STATIC)
target_link_libraries(MyGame sfml-graphics sfml-audio GLEW::GLEW sfml-system sfml-window sfml-audio sfml-graphics ${OPENGL_gl_LIBRARY})
target_include_directories(MyGame PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

