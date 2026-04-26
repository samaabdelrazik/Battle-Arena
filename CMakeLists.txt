cmake_minimum_required(VERSION 3.5)

project(Battle_Arena VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Battle_Arena
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


        Character.h
        Character.cpp

        Warrior.h
        Warrior.cpp

        Mage.h
        Mage.cpp

        Archer.h
        Archer.cpp

        Block.h
        Block.cpp

        GoodBlock.h
        GoodBlock.cpp

        BadBlock.h
        BadBlock.cpp

        Platform.h
        Platform.cpp

        battle_arenahome.h
        battle_arenahome.cpp
        battle_arenahome.ui
        
        Enemy.h
        Enemy.cpp

    )
else()
    if(ANDROID)
        add_library(Battle_Arena SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Battle_Arena
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Battle_Arena PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Battle_Arena)
endif()

set_target_properties(Battle_Arena PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Battle_Arena
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Battle_Arena)
endif()
