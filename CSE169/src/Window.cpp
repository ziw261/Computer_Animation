////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 169 Starter";

// Objects to render
Cube * Window::cube;
Skeleton * Window::mainSkeleton;
Skin* Window::mainSkin;
AnimationClip* Window::mainAnimation;
ParticleSystem* Window::mainParticleSystem;
Fluid* Window::mainFluid;

float Window::startTime;
float Window::currentTime;
float Window::pauseStart;
float Window::pauseEnd;
bool Window::shouldPause;
bool Window::isAirOn;

// Switch
int Window::nowLoading = -1;
int Window::loadAnimation = 0;
int Window::loadCloth = 1;
int Window::loadFluid = 2;


// Joint selection Properties
int Window::currentJointIndex = 0;
vector<joint*> Window::jointGroup;
int Window::xyzChoice = 0;


// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;


////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("/Users/aki/GitHub/CSE169_Animation/CSE169/shaders/shader.vert", "/Users/aki/GitHub/CSE169_Animation/CSE169/shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects(int argc,char **argv)
{
	// Create a cube
	cube = new Cube();
    
    // Decide to what to load
    nowLoading = loadFluid;
    
    if(nowLoading == loadFluid){
        mainFluid = new Fluid();
    }
    else if(nowLoading == loadCloth){
        isAirOn = true;
        mainParticleSystem = new ParticleSystem(30.0f);
       // mainParticleSystem->airVelocity = glm::vec3(0);
    }
    else if(nowLoading == loadAnimation){
        mainSkeleton = new Skeleton();
        
        if(argc == 1) {
            mainSkeleton->Load("wasp.skel");
        } else{
            mainSkeleton->Load(argv[1]);
            //std::cout << argc << std::endl;
        }
        
        // Get all the joints in the mainSkeleton
        jointGroup = mainSkeleton->GetJointGroup();
        
        mainSkin = new Skin(jointGroup);
        

        //mainSkin->Start("wasp_smooth.skin");
        if(argc == 1){
            mainSkin->Start("wasp.skin");
        } else {
            mainSkin->Start(argv[2]);
        }
        
        mainAnimation = new AnimationClip(jointGroup);
        
        if(argc == 1){
            startTime = clock();
            mainAnimation->Load("wasp_walk.anim");
            //mainAnimation->EvaluateAll(0);
        }
    }

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
    
    //currentTime = (float)(clock()-startTime)/CLOCKS_PER_SEC;  //1000000
    if(!shouldPause){
        currentTime = (float)(clock()-startTime)/1000000000;  //1000000
    }

	// Perform any updates as necessary. 
	Cam->Update();
    //cerr << currentTime << endl;
    if(nowLoading == loadCloth){
        mainParticleSystem->Update(0.002f);

    }
    else if(nowLoading == loadAnimation){
        mainSkeleton->Update();
        mainSkin->Update();
        mainAnimation->Update(currentTime);
    }
    
    //cerr<<mainAnimation->channels[3]->extrapOut<<endl;
    //cerr << currentTime << endl;
	//cube->update();
    
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    
    
    
	// Render the object.
	//cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    
    if(nowLoading == loadAnimation){
        //mainSkeleton->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
        mainSkin->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
        
    }
    else if(nowLoading == loadCloth){
        mainParticleSystem->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    else if(nowLoading == loadFluid){
        mainFluid ->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}


// helper to change dof
void Window::changeDof(float value){
    switch (xyzChoice) {
        case 0:
            jointGroup[currentJointIndex]->ChangeDofX(value);
            break;
        case 1:
            jointGroup[currentJointIndex]->ChangeDofY(value);
            break;
        case 2:
            jointGroup[currentJointIndex]->ChangeDofZ(value);
            break;
        default:
            jointGroup[currentJointIndex]->ChangeDofX(value);
            break;
    }
}


// helper to selectJoint
void Window::selectJoint(int xyz){
    xyzChoice = xyz;
    
}



////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS) {
		switch (key){
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                startTime = clock();
                break;
            
            case GLFW_KEY_P:
                // Pause the animation logic
                if(nowLoading == loadAnimation){
                    if(!shouldPause){
                        pauseStart = clock();
                        shouldPause = true;
                        
                    } else {
                        pauseEnd = clock();
                        startTime += pauseEnd - pauseStart;
                        shouldPause = false;
                    }
                }
                break;
                
            case GLFW_KEY_X:
                if(nowLoading == loadAnimation){
                    selectJoint(0);
                    std::cout<<"Switching to change dof x value" << std::endl;
                }
                break;
                
            case GLFW_KEY_Y:
                if(nowLoading == loadAnimation){
                    selectJoint(1);
                    std::cout<<"Switching to change dof y value" << std::endl;
                }
                break;
                
            case GLFW_KEY_Z:
                if(nowLoading == loadAnimation){
                    selectJoint(2);
                    std::cout<<"Switching to change dof z value" << std::endl;
                }
                break;
            
            case GLFW_KEY_RIGHT:
                if(nowLoading == loadAnimation){
                    if(currentJointIndex+1 >= jointGroup.size()){
                        currentJointIndex = (int)(jointGroup.size()-1);
                    } else {
                        currentJointIndex++;
                    }
                
                    std::cout << "Switching to " << jointGroup[currentJointIndex]->GetName() << std::endl;
                }
                break;
                
            case GLFW_KEY_LEFT:
                if(nowLoading == loadAnimation){
                    if(currentJointIndex-1 < 0){
                        currentJointIndex = 0;
                    } else {
                        currentJointIndex--;
                    }
                
                    std::cout << "Now at " << jointGroup[currentJointIndex]->GetName() << std::endl;
                }
                
                break;
            
            case GLFW_KEY_UP:
                if(nowLoading == loadAnimation){
                    changeDof(0.2);
                    std::cout << "Now increasing "<<jointGroup[currentJointIndex]->GetName() << "'s ";
                    if(xyzChoice == 0){
                        std::cout << "dof x value." << std::endl;
                    } else if(xyzChoice == 1){
                        std::cout << "dof y value." << std::endl;
                    } else {
                        std::cout << "dof z value." << std::endl;
                    }
                }
                break;
            
            case GLFW_KEY_DOWN:
                if(nowLoading == loadAnimation){
                    changeDof(-0.2);
                    std::cout << "Now decreasing "<<jointGroup[currentJointIndex]->GetName() << "'s ";
                    if(xyzChoice == 0){
                        std::cout << "dof x value." << std::endl;
                    } else if(xyzChoice == 1){
                        std::cout << "dof y value." << std::endl;
                    } else {
                        std::cout << "dof z value." << std::endl;
                    }
                }
                break;
            
            case GLFW_KEY_W:
                if(nowLoading == loadCloth){
                    mainParticleSystem->MoveFixedPoint(glm::vec3(0.0f,0.1f,0.0f));
                }
                break;
                
            case GLFW_KEY_A:
                if(nowLoading == loadCloth){
                    mainParticleSystem->MoveFixedPoint(glm::vec3(-0.1f,0.0f,0.0f));
                }
                break;
            
            case GLFW_KEY_S:
                if(nowLoading == loadCloth){
                    mainParticleSystem->MoveFixedPoint(glm::vec3(0.0f,-0.1f,0.0f));
                }
                break;
            
            case GLFW_KEY_D:
                if(nowLoading == loadCloth){
                    mainParticleSystem->MoveFixedPoint(glm::vec3(0.1f,0.0f,0.0f));
                }
                break;
            
            case GLFW_KEY_Q:
                if(nowLoading == loadCloth){
                    mainParticleSystem->MoveFixedPoint(glm::vec3(0.0f,0.0f,0.1f));
                }
                break;
                
            case GLFW_KEY_E:
                if(nowLoading == loadCloth){
                    mainParticleSystem->MoveFixedPoint(glm::vec3(0.0f,0.0f,-0.1f));
                }
                break;
            
            case GLFW_KEY_T:
                if(nowLoading == loadCloth){
                    if(isAirOn){
                        isAirOn = false;
                        cout << "Wind is Off" << endl;
                        mainParticleSystem->airVelocity = glm::vec3(0);
                    } else {
                        isAirOn = true;
                        cout  << "Wind is On" << endl;
                        mainParticleSystem->airVelocity = glm::vec3(0.0f, 0.0f, -10.0f);

                    }
                }
                break;
            
            case GLFW_KEY_L:
                if(nowLoading == loadCloth){
                    mainParticleSystem->ReleaseFixedPoint();
                }
                break;
                
                
            case GLFW_KEY_V:
                if(nowLoading == loadCloth){
                    mainParticleSystem->airVelocity += glm::vec3(0.0f,0.0f,5.0f);
                }
                break;
            
            case GLFW_KEY_C:
                if(nowLoading == loadCloth){
                    mainParticleSystem->airVelocity -= glm::vec3(0.0f,0.0f,5.0f);
                }
                break;
            

            default:
                break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
