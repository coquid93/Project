project(Rendering)
cmake_minimum_required(VERSION 2.8)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_libraries(${GLUT_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
link_libraries(${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})


aux_source_directory(external/IMGUI SRC_LIST)
aux_source_directory(src SRC_LIST)


add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(
    ${PROJECT_NAME}
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES} )
include_directories( 
    ${GLUT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    external/
    include/
    external/IMGUI
    )

