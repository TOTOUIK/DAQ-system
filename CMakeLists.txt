cmake_minimum_required(VERSION 3.16)
project(DAQsystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ���� Qt MOC ����
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ��������·��
set(THIRD ${CMAKE_SOURCE_DIR}/third_party)

# OpenCV
set(OPENCV_WORLD_DBG opencv_world4120d)   # Debug
set(OPENCV_WORLD_REL opencv_world4120)    # Release

# �� Qt6
find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)

# include
include_directories(
    ${THIRD}/GCI_SDK/include
    ${THIRD}/opencv/build/include
    ${THIRD}/opencv/build/include/opencv2
    ${THIRD}/eigen
    ${THIRD}/cnpy
    ${THIRD}/zlib/include
    "C:/Program Files (x86)/National Instruments/NI-DAQ/DAQmx ANSI C Dev/include"
)

add_executable(DAQsystem
    datarecorder.cpp
    mainwindow.cpp
    syncmanager.cpp
    cameracontroller.cpp
    graphicsviewzoomer.cpp
    ${THIRD}/cnpy/cnpy.cpp
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    depthview.h depthview.cpp
    nidaqcontroller.h nidaqcontroller.cpp
)

target_sources(DAQsystem PRIVATE
    cameracontroller.h
    datarecorder.h
    syncmanager.h
    mainwindow.h
    graphicsviewzoomer.h
)

target_link_libraries(DAQsystem PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    "${THIRD}/GCI_SDK/lib/GC3D.lib"
    "${THIRD}/GCI_SDK/lib/GC3DAlgorithm.lib"
    "${THIRD}/GCI_SDK/lib/GC3D-加密.lib"
    "${THIRD}/zlib/lib/zlibwapi.lib"
    "C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.lib"
    "C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc/nisyscfg.lib"
    ws2_32
    $<$<CONFIG:Debug>:"${THIRD}/opencv/build/x64/vc16/lib/${OPENCV_WORLD_DBG}">
    $<$<CONFIG:Release>:"${THIRD}/opencv/build/x64/vc16/lib/${OPENCV_WORLD_REL}">
)

# ���� DLL
add_custom_command(TARGET DAQsystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${THIRD}/GCI_SDK/bin/GC3D.dll" "$<TARGET_FILE_DIR:DAQsystem>"
)

# 复制 GCI_SDK/bin 下所有运行时依赖，避免缺失导致 SDK 报错
add_custom_command(TARGET DAQsystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${THIRD}/GCI_SDK/bin"
        "$<TARGET_FILE_DIR:DAQsystem>"
)

add_custom_command(TARGET DAQsystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:"${THIRD}/opencv/build/x64/vc16/bin/${OPENCV_WORLD_DBG}.dll">
        $<$<CONFIG:Release>:"${THIRD}/opencv/build/x64/vc16/bin/${OPENCV_WORLD_REL}.dll">
        "$<TARGET_FILE_DIR:DAQsystem>"
)

add_custom_command(TARGET DAQsystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${THIRD}/zlib/bin/zlibwapi.dll"
        "$<TARGET_FILE_DIR:DAQsystem>"
)
