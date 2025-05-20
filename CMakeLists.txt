cmake_minimum_required(VERSION 3.10)
project(gimpsep-gimpers)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include path for .h files in src/
include_directories(src)

# Resources
set(CASCADES_DIR "${CMAKE_SOURCE_DIR}/cascades")
set(IMAGES_DIR   "${CMAKE_SOURCE_DIR}/images")

# Shared logic source files
set(COMMON_SRC
        src/BrightnessProcessor.cpp
        src/EdgeDetector.cpp
)

# Sources for each binary
set(STITCHING_SRC        src/stitching.cpp)
set(FACE_REC_SRC         src/face_recognition.cpp ${COMMON_SRC})
set(MAIN_SRC             src/main.cpp ${COMMON_SRC})

# Executables
add_executable(panorama         ${STITCHING_SRC})
add_executable(face_recognition ${FACE_REC_SRC})
add_executable(image_editor     ${MAIN_SRC})

# Link with OpenCV
target_link_libraries(panorama         PRIVATE ${OpenCV_LIBS})
target_link_libraries(face_recognition PRIVATE ${OpenCV_LIBS})
target_link_libraries(image_editor     PRIVATE ${OpenCV_LIBS})

# Output path
set_target_properties(panorama face_recognition image_editor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
