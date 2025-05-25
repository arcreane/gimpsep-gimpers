cmake_minimum_required(VERSION 3.10)
project(gimpsep-gimpers)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Find OpenCV ===
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui imgcodecs)

# === Find Qt6 (for GUI) ===
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Enable Qt Meta-object compiler
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# === Include directories ===
include_directories(
        src
        src/include
        src/core
        src/gui
        ${OpenCV_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
)

# === Executable output path ===
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# === Shared core logic ===
set(COMMON_SRC
        src/core/BrightnessProcessor.cpp
        src/core/EdgeDetector.cpp
        src/core/ResizeProcessor.cpp
        src/core/MorphologyProcessor.cpp
)

# === GUI logic ===
set(GUI_SRC
        src/gui/ImageEditorGUI.cpp
)

# === Executables ===
add_executable(panorama src/stitching.cpp)
add_executable(face_recognition src/face_recognition.cpp ${COMMON_SRC})
add_executable(image_editor src/main.cpp ${COMMON_SRC} ${GUI_SRC})

# === Link libraries ===
target_link_libraries(panorama         PRIVATE ${OpenCV_LIBS})
target_link_libraries(face_recognition PRIVATE ${OpenCV_LIBS})
target_link_libraries(image_editor     PRIVATE ${OpenCV_LIBS} Qt6::Widgets)
