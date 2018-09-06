animation2: trackballGUI.cpp camera.o Body_structure.o IKsolver.o Blending.o Read_BVH.o MotionCalc.o game.o imgui.o imgui_impl_glut.o imgui_draw.o imgui_demo.o 
	g++ trackballGUI.cpp camera.o Body_structure.o IKsolver.o Blending.o Read_BVH.o MotionCalc.o game.o imgui.o imgui_impl_glut.o imgui_draw.o imgui_demo.o -o animation2 -lGL -lGLU -lglut
camera.o : Luis/camera.cpp
	g++ -c Luis/camera.cpp -o camera.o -lGL -lGLU -lglut
Body_structure.o : Luis/Body_structure.cpp
	g++ -c Luis/Body_structure.cpp -o Body_structure.o -lGL -lGLU -lglut
Read_BVH.o : Luis/Read_BVH.cpp
	g++ -c Luis/Read_BVH.cpp -o Read_BVH.o -lGL -lGLU -lglut
IKsolver.o : Luis/IKsolver.cpp
	g++ -c Luis/IKsolver.cpp -o IKsolver.o -lGL -lGLU -lglut
MotionCalc.o : MotionCalc.cpp MotionCalc.h
	g++ -c MotionCalc.cpp -o MotionCalc.o -lGL -lGLU -lglut
game.o : game.cpp game.h
	g++ -c game.cpp -o game.o -lGL -lGLU -lglut
Blending.o : Blending.cpp MotionCalc.h
	g++ -c Blending.cpp -o Blending.o -lGL -lGLU -lglut
imgui_impl_glut.o : imgui_impl_glut.cpp
	g++ -c imgui_impl_glut.cpp -o imgui_impl_glut.o -lGL -lGLU -lglut
imgui.o : imgui.cpp
	g++ -c imgui.cpp -o imgui.o -lGL -lGLU -lglut
imgui_draw.o : imgui_draw.cpp
	g++ -c imgui_draw.cpp -o imgui_draw.o -lGL -lGLU -lglut
imgui_demo.o : imgui_demo.cpp
	g++ -c imgui_demo.cpp -o imgui_demo.o -lGL -lGLU -lglut



