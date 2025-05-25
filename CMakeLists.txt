cmake_minimum_required(VERSION 3.10)
project(gimpsep-gimpers)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# === Find OpenCV ===
find_package(OpenCV REQUIRED)

# Debug info (optional)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# === Find Qt6 Widgets ===
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Enable Qt automatic features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# === Include directories ===
# Include OpenCV headers and your project headers
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(src)
include_directories(src/include)
include_directories(src/gui)


include_directories(src/gui)

# === Resource paths (optional for runtime use) ===
set(CASCADES_DIR "${CMAKE_SOURCE_DIR}/cascades")
set(IMAGES_DIR   "${CMAKE_SOURCE_DIR}/images")

# === Shared logic source files ===
set(COMMON_SRC
        src/core/BrightnessProcessor.cpp
        src/core/EdgeDetector.cpp
)

# === GUI source files ===
set(GUI_SRC
        src/gui/ImageEditorGUI.cpp
)

# === Executable source groups ===
set(STITCHING_SRC        src/stitching.cpp)
set(FACE_REC_SRC         src/face_recognition.cpp ${COMMON_SRC})
set(MAIN_SRC             src/main.cpp ${COMMON_SRC} ${GUI_SRC})

# === Add Executables ===
add_executable(panorama         ${STITCHING_SRC})
add_executable(face_recognition ${FACE_REC_SRC})
add_executable(image_editor     ${MAIN_SRC})

# === Link Libraries ===
target_link_libraries(panorama         PRIVATE ${OpenCV_LIBS})
target_link_libraries(face_recognition PRIVATE ${OpenCV_LIBS})
target_link_libraries(image_editor     PRIVATE ${OpenCV_LIBS} Qt6::Widgets)

# === Output directories for executables ===
set_target_properties(panorama face_recognition image_editor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
