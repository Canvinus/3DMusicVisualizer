cmake_minimum_required(VERSION 3.17)
project(3DViz)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(../libraries libraries)

add_executable(3DViz
        src/cpp/main.cpp
        src/cpp/loaders/text/load_text.h
        src/cpp/loaders/text/load_text.cpp
        src/cpp/utils/opengl.h
        src/cpp/utils/opengl.cpp
        src/cpp/utils/exception.cpp
        src/cpp/utils/exception.h
        src/cpp/loaders/image/image.cpp
        src/cpp/loaders/image/image.h
        src/cpp/camera.cpp
        src/cpp/camera.h
        src/cpp/objects/skybox.cpp
        src/cpp/objects/skybox.h
        src/cpp/objects/object3d.cpp
        src/cpp/objects/object3d.h
        src/cpp/objects/textured_model.cpp
        src/cpp/objects/textured_model.h
        src/cpp/objects/AudioObject.h
        src/cpp/objects/AudioObject.cpp
        src/cpp/objects/PlayBack.h
        src/cpp/objects/PlayBack.cpp
        src/cpp/loaders/model/model_data.cpp
        src/cpp/loaders/model/model_data.h
        src/cpp/loaders/model/mesh_data.cpp
        src/cpp/loaders/model/mesh_data.h
        src/cpp/loaders/model/load_model.h
        src/cpp/loaders/model/load_model.cpp
        src/cpp/loaders/image/load_image.h
        src/cpp/loaders/image/load_image.cpp
        src/cpp/objects/text_object.cpp
        src/cpp/objects/text_object.h
        src/cpp/loaders/font/load_font.cpp
        src/cpp/loaders/font/load_font.h
        src/cpp/loaders/font/character.cpp
        src/cpp/loaders/font/character.h
        src/cpp/objects/AudioVis.h)

#GLFW
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

target_link_libraries(3DViz glfw)

#GLEW

include_directories(../libraries/glew-2.1.0/include)
target_link_libraries(3DViz glew_s)

#GLM

target_link_libraries(3DViz glm)

#stb

target_link_libraries(3DViz stb)

#Assimp

include_directories(../libraries/assimp-5.0.1/include)
target_link_libraries(3DViz assimp)

#Freetype
target_link_libraries(3DViz freetype)
include_directories(../libraries/freetype-2.10.4/include)

set(SFML_ROOT c:/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(3DViz ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()