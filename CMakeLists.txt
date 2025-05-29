cmake_minimum_required(VERSION 3.10)
project(gimpsep-gimpers)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV (avec stitching pour le panorama)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui objdetect face stitching)

# Include path pour vos headers
include_directories(${CMAKE_SOURCE_DIR}/src)

# Sources partagées
set(COMMON_SRC
        src/BrightnessProcessor.cpp
        src/EdgeDetector.cpp
)

# Création des exécutables
add_executable(panorama         src/stitching.cpp)
add_executable(face_detection   src/face_detection.cpp ${COMMON_SRC})
add_executable(face_recognition src/face_recognition.cpp)
add_executable(image_editor     src/main.cpp           ${COMMON_SRC})

# Link avec OpenCV
foreach(t panorama face_detection face_recognition image_editor)
    target_link_libraries(${t} PRIVATE ${OpenCV_LIBS})
endforeach()
