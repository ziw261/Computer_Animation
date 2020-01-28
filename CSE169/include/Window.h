#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "Cube.h"
#include "shader.h"
#include "Camera.h"
#include "skeleton.hpp"
#include "Skin.hpp"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
    
    // Joint selection Properties
    static int currentJointIndex;
    static vector<joint*> jointGroup;
    static int xyzChoice;
    
    
	// Objects to render
	static Cube* cube;
    
    // Skeleton to render
    static Skeleton* mainSkeleton;
    
    // Skin to render
    static Skin* mainSkin;
    
	// Shader Program 
	static GLuint shaderProgram;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	static bool initializeObjects(int argc,char **argv);
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// helper to reset the camera
	static void resetCamera();

    // helper to change dof
    static void changeDof(float value);
    
    // helper to select joint
    static void selectJoint(int xyz);
    
	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);
};

////////////////////////////////////////////////////////////////////////////////

#endif
