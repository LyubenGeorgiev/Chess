include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)


# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.26.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

# SDL2_image
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.6.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
FetchContent_MakeAvailable(SDL2_image)

add_executable(chess
    main.cpp
    Board.cpp
    BoardState.cpp
    Game.cpp
    Piece.cpp
    Window.cpp
)

target_include_directories(
    chess
    PRIVATE
    include/
)

target_link_libraries(
    chess
    PRIVATE
    SDL2::SDL2-static
    SDL2_image::SDL2_image-static
)
