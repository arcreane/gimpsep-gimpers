cmake_minimum_required(VERSION 3.10)
project(gimpsep-gimpers)

# Recherche des librairies OpenCV
find_package(OpenCV REQUIRED)

# Répertoires de ressources
set(CASCADES_DIR "${CMAKE_SOURCE_DIR}/cascades")
set(IMAGES_DIR   "${CMAKE_SOURCE_DIR}/images")

# Sources
set(STITCHING_SRC        src/stitching.cpp)
set(FACE_REC_SRC         src/face_recognition.cpp)
set(MAIN_SRC             src/main.cpp)

# Création des exécutables
add_executable(panorama         ${STITCHING_SRC})
add_executable(face_recognition ${FACE_REC_SRC})
add_executable(image_editor     ${MAIN_SRC})

# Liaison avec OpenCV
target_link_libraries(panorama         PRIVATE ${OpenCV_LIBS})
target_link_libraries(face_recognition PRIVATE ${OpenCV_LIBS})
target_link_libraries(image_editor     PRIVATE ${OpenCV_LIBS})

# Standard C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Répertoire de sortie
set_target_properties(panorama face_recognition image_editor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)