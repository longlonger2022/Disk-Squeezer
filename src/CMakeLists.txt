cmake_minimum_required(VERSION 3.16)

project(DiskSqueezer3 VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDiskSqueezer3
    main.cpp
    platform/resource.h
    platform/Resource.rc
    AutoTaskCreator.h
    FileSystemChecker.h
    WriteModeCreator.h
    CommandModeCreator.h
    AllocateModeCreator.h
    LogHandler.h
    ../assets/resources.qrc
)

qt_add_qml_module(appDiskSqueezer3
    URI DiskSqueezer3
    QML_FILES Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDiskSqueezer3 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER io.github.im0xlonger.disksqueezer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    VERSION ${PROJECT_VERSION}
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../android"
    QT_ANDROID_PACKAGE_NAME "io.github.im0xlonger.disksqueezer"
    QT_ANDROID_APP_NAME "硬盘榨干器"
    QT_ANDROID_APP_ICON "@drawable/ic_launcher"
    QT_ANDROID_VERSION_NAME "3.0.0"
    QT_ANDROID_VERSION_CODE 1
)

target_link_libraries(appDiskSqueezer3
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appDiskSqueezer3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
