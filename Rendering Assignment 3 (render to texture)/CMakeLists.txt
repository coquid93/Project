project(Rendering)
cmake_minimum_required(VERSION 2.8)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
link_libraries(${OPENGL_LIBRARIES})
add_executable(${PROJECT_NAME}
  #header.
  include/camera.h
  include/Geometry.h
  include/glslShader.h
  include/picking.h
  include/imgui_internal.h
  include/imgui_impl_glut.h
  include/imconfig.h
  include/imgui.h
  include/stb_rect_pack.h
  include/stb_textedit.h
  include/stb_truetype.h
  include/render2texture.h
  #src file.
  src/camera.cpp
  src/Geometry.cpp
  src/glslShader.cpp
  src/picking.cpp
  src/main.cpp
  src/imgui.cpp
  src/imgui_draw.cpp
  src/imgui_impl_glut.cpp
  src/render2texture.cpp
  )

IF(WIN32)
	set(GLEW_PAHT)
	target_link_libraries(
	    ${PROJECT_NAME}
	    lib/freeglut
	    ${OPENGL_LIBRARIES}
	    lib/Debug/glew32
	    )
	include_directories(
	    include/GL/
	    ${OPENGL_INCLUDE_DIRS}
	    include/GL/
	    external/
	    include/
	    external/IMGUI
	    external/Eigen
	    )
ENDIF(WIN32)

IF(UNIX)
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIRS})
	link_libraries(${GLUT_LIBRARIES})

	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	link_libraries(${GLEW_LIBRARIES})

	target_link_libraries(
	    ${PROJECT_NAME}
	    ${GLUT_LIBRARIES}
	    ${OPENGL_LIBRARIES}
	    ${GLEW_LIBRARIES}
      )
	include_directories(
	    ${GLUT_INCLUDE_DIRS}
	    ${OPENGL_INCLUDE_DIRS}
	    ${GLEW_INCLUDE_DIRS}
	    external/
	    include/
	    external/IMGUI
	    )

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
ENDIF(UNIX)
