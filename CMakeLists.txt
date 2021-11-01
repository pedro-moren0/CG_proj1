project(proj1)
add_executable(${PROJECT_NAME}
    main.cpp
    openglwindow.cpp
    ship.cpp
    crab.cpp
    bullet.cpp)
enable_abcg(${PROJECT_NAME})
