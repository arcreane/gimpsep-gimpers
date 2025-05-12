cmake_minimum_required(VERSION 3.31)
project(gimpsep-gimpers)

# Trouver OpenCV
find_package(OpenCV REQUIRED)

# Répertoire des ressources
set(IMAGES_DIR "${CMAKE_SOURCE_DIR}/images")
set(VIDEOS_DIR "${CMAKE_SOURCE_DIR}/videos")
set(CASCADES_DIR "${CMAKE_SOURCE_DIR}/cascades")

# Sources du module "Islam"
set(STITCHING_SRC src/stitching.cpp)
set(FACE_RECOGNITION_SRC src/face_recognition.cpp)

# Exécutables
add_executable(panorama ${STITCHING_SRC})
add_executable(face_recognition ${FACE_RECOGNITION_SRC})

add_custom_target(run_panorama
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/panorama
        ${CMAKE_SOURCE_DIR}/images/van_gogh.jpg
        ${CMAKE_SOURCE_DIR}/images/another.jpg
        DEPENDS panorama
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)


# Lier OpenCV
target_link_libraries(panorama PRIVATE ${OpenCV_LIBS})
target_link_libraries(face_recognition PRIVATE ${OpenCV_LIBS})

# Standards C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Répertoire de sortie des exécutables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)