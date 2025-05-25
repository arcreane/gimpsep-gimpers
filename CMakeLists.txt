cmake_minimum_required(VERSION 3.10)
project(gimpsep-gimpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
endif()

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED 
    COMPONENTS core imgproc highgui imgcodecs
)
 
include_directories(
    src
    ${OpenCV_INCLUDE_DIRS}
)

set(CASCADES_DIR "${CMAKE_SOURCE_DIR}/cascades")
set(IMAGES_DIR   "${CMAKE_SOURCE_DIR}/images")

set(COMMON_SRC
    src/BrightnessProcessor.cpp
    src/EdgeDetector.cpp
)

add_executable(panorama src/stitching.cpp "src/MorphologyProcessor.cpp" "src/ResizeProcessor.cpp")
add_executable(face_recognition src/face_recognition.cpp ${COMMON_SRC} "src/MorphologyProcessor.cpp" "src/ResizeProcessor.cpp")
add_executable(image_editor src/main.cpp ${COMMON_SRC} "src/MorphologyProcessor.cpp" "src/ResizeProcessor.cpp")

target_link_libraries(panorama PRIVATE ${OpenCV_LIBS})
target_link_libraries(face_recognition PRIVATE ${OpenCV_LIBS})
target_link_libraries(image_editor PRIVATE ${OpenCV_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
    add_custom_command(TARGET panorama POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OpenCV_DIR}/../bin/opencv_world${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}.dll"
            ${EXECUTABLE_OUTPUT_PATH}
    )
endif()