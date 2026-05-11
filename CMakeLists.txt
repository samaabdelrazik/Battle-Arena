cmake_minimum_required(VERSION 3.16)

project(Battle_Arena VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_resources(RESOURCES_SOURCES game-image.qrc resources.qrc)

qt_add_executable(Battle_Arena
    main.cpp

    ${RESOURCES_SOURCES}

    Character.h
    Character.cpp

    Warrior.h
    Warrior.cpp

    Mage.h
    Mage.cpp

    Archer.h
    Archer.cpp

    Enemy.h
    Enemy.cpp

    Block.h
    Block.cpp

    GoodBlock.h
    GoodBlock.cpp

    BadBlock.h
    BadBlock.cpp

    Platform.h
    Platform.cpp

    Arrow.h
    Arrow.cpp


    CombatEffect.h
    CombatEffect.cpp


    battle_arenahome.h
    battle_arenahome.cpp
    battle_arenahome.ui

    CharacterSelect.h
    CharacterSelect.cpp
    CharacterSelect.ui
)

target_link_libraries(Battle_Arena
    PRIVATE Qt6::Widgets
)

include(GNUInstallDirs)

install(TARGETS Battle_Arena
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
