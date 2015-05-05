// MD2 animation renderer
// This demo will load and render an animated MD2 model, an OBJ model and a skybox
// Most of the OpenGL code for dealing with buffer objects, etc has been moved to a 
// utility library, to make creation and display of mesh objects as simple as possible

// Windows specific: Uncomment the following line to open a console window for debug output
#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "skybox.h"
#include "hobgoblin.h"
#include "camera.h"
#include "ground.h"
#include "lavaground.h"
#include "movingPlatform.h"
#include "largeRock.h"
#include "bunnyPickup.h"
#include "onscreenHUD.h"
#include "OnscreenWinHUD.h"
#include "OnscreenDeadHUD.h"
#include <SDL_ttf.h>

using namespace std;



// Globals
// Real programs don't use globals :-D

//GLuint meshIndexCount = 0;
//GLuint md2VertCount = 0;
//GLuint meshObjects[2];

GLuint shaderProgram;

Skybox *mySkybox;

Hobgoblin *myHobgoblin;

Camera *myCamera;

OnscreenHUD *myHUD;
OnscreenWinHUD *myWinScreen;
OnscreenDeadHUD *myDeathScreen;

Ground *myGround;
LavaGround *lavapool;
//Array of moving platforms
//number of moving platforms stored under one variable
//so I can just change the number in one place
const int numOfmPlatforms = 6;
MovingPlatform *mPlatform[numOfmPlatforms];

//Array of Golden Bunny collectables
//Also including a const variable to store number of bunnies
//so that it can be changed in one place
const int numOfgBunnies = 8;
BunnyPickup *gBunnies[numOfgBunnies];

//Pointer for an array of large rocks that will be displayed around the level
//-these will form a wall around the platforms
//One rock for now to ensure I can load the rock correctly
//with texture and collision working
const int numOfrocks = 22;
LargeRock *lrgRock[numOfrocks];

GLfloat r = 0.0f;

stack<glm::mat4> mvStack; 

// TEXTURE STUFF
GLuint textures[2];

//GLuint labels[5];

rt3d::lightStruct light0 = {
	{0.3f, 0.3f, 0.3f, 1.0f}, // ambient
	{1.0f, 1.0f, 1.0f, 1.0f}, // diffuse
	{1.0f, 1.0f, 1.0f, 1.0f}, // specular
	{-10.0f, 10.0f, 10.0f, 1.0f}  // position
};

glm::vec4 lightPos(-10.0f, 10.0f, 10.0f, 1.0f); //light position

rt3d::materialStruct material0 = {
	{0.2f, 0.4f, 0.2f, 1.0f}, // ambient
	{0.5f, 1.0f, 0.5f, 1.0f}, // diffuse
	{0.0f, 0.1f, 0.0f, 1.0f}, // specular
	2.0f  // shininess
};
rt3d::materialStruct material1 = {
	{0.4f, 0.4f, 1.0f, 1.0f}, // ambient
	{0.8f, 0.8f, 1.0f, 1.0f}, // diffuse
	{0.8f, 0.8f, 0.8f, 1.0f}, // specular
	1.0f  // shininess
};

// Set up rendering context
SDL_Window * setupRC(SDL_GLContext &context) {
	SDL_Window * window;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
        rt3d::exitFatalError("Unable to initialize SDL"); 
	  
    // Request an OpenGL 3.0 context.
	
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)
 
    // Create 800x600 window
    window = SDL_CreateWindow("B00256311 3D Tech Demo - Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if (!window) // Check window was created OK
        rt3d::exitFatalError("Unable to create window");
 
    context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
    SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}




void init(void) {
	// For this simple example we'll be using the most basic of shader programs
	shaderProgram = rt3d::initShaders("phong-tex.vert","phong-tex.frag");
	rt3d::setLight(shaderProgram, light0);
	rt3d::setMaterial(shaderProgram, material0);
	
	mySkybox = new Skybox();
	mySkybox->initialise();

	myGround = new Ground();
	myGround->initialise();

	lavapool = new LavaGround();
	lavapool->initialise();

	//Starting and end co-ordinates of moving platforms
	mPlatform[0] = new MovingPlatform(glm::vec3(20,-0.1,-35), glm::vec3(20, -0.1, -15));
	mPlatform[1] = new MovingPlatform(glm::vec3(-40, -0.1, -16), glm::vec3(5, -0.1, -10));
	mPlatform[2] = new MovingPlatform(glm::vec3(-50, -0.1, -20), glm::vec3(-50, -0.1, -65));
	mPlatform[3] = new MovingPlatform(glm::vec3(8, -0.1, 15), glm::vec3(-60, -0.1, 50));
	mPlatform[4] = new MovingPlatform(glm::vec3(-35, -0.1, 55), glm::vec3(32, -0.1, 15));
	mPlatform[5] = new MovingPlatform(glm::vec3(-35, -0.1, 70), glm::vec3(15, -0.1, 78));
	
	//Moving Platforms initialised
	for (int i = 0; i < numOfmPlatforms; i++)
	{
		
		mPlatform[i]->initialise();
	}

	//Positions of large rocks
	//main ground
	lrgRock[0] = new LargeRock(glm::vec3(2.0f, 0.1f, -42.0f), 0.2, 0);
	lrgRock[1] = new LargeRock(glm::vec3(8.0f, 0.1f, -42.0f), 0.2, 0);
	lrgRock[2] = new LargeRock(glm::vec3(1.8f, 0.1f, -48.0f), 0.2, 90.0f);
	lrgRock[3] = new LargeRock(glm::vec3(1.8f, 0.1f, -54.0f), 0.2, 90.0f);
	lrgRock[4] = new LargeRock(glm::vec3(1.8f, 0.1f, -60.0f), 0.2, 90.0f);
	lrgRock[5] = new LargeRock(glm::vec3(1.8f, 0.1f, -66.0f), 0.2, 90.0f);
	lrgRock[6] = new LargeRock(glm::vec3(1.8f, 0.1f, -72.0f), 0.2, 90.0f);
	lrgRock[7] = new LargeRock(glm::vec3(1.8f, 0.1f, -78.0f), 0.2, 90.0f);

	lrgRock[8] = new LargeRock(glm::vec3(5.0f, 0.1f, -80.0f), 0.2, 0);
	lrgRock[9] = new LargeRock(glm::vec3(10.0f, 0.1f, -80.0f), 0.4, 0);
	lrgRock[10] = new LargeRock(glm::vec3(17.0f, 0.1f, -80.0f), 0.2, 0);
	lrgRock[11] = new LargeRock(glm::vec3(22.0f, 0.1f, -80.0f), 0.3, 0);
	lrgRock[12] = new LargeRock(glm::vec3(28.0f, 0.1f, -80.0f), 0.2, 0);
	lrgRock[13] = new LargeRock(glm::vec3(34.0f, 0.1f, -80.0f), 0.2, 0);

	lrgRock[14] = new LargeRock(glm::vec3(40.0f, 0.1f, -80.0f), 0.4, 90);
	lrgRock[15] = new LargeRock(glm::vec3(40.0f, 0.1f, -72.0f), 0.2, 90);
	lrgRock[16] = new LargeRock(glm::vec3(40.0f, 0.1f, -66.0f), 0.2, 90);
	lrgRock[17] = new LargeRock(glm::vec3(40.0f, 0.1f, -60.0f), 0.2, 90);
	lrgRock[18] = new LargeRock(glm::vec3(40.0f, 0.1f, -55.0f), 0.1, 90);
	lrgRock[19] = new LargeRock(glm::vec3(40.0f, 0.1f, -50.0f), 0.2, 90);
	lrgRock[20] = new LargeRock(glm::vec3(40.0f, 0.1f, -43.5f), 0.2, 90);
	lrgRock[21] = new LargeRock(glm::vec3(31.5f, 0.1f, -40.0f), 0.4, 0);

	//Large Rocks initialised
	for (int j = 0; j < numOfrocks; j++)
	{
		lrgRock[j]->initialise();
	}

	//Positions of Golden Bunnies
	gBunnies[0] = new BunnyPickup(glm::vec3(30.0f, -0.1f, -50.0f));
	gBunnies[1] = new BunnyPickup(glm::vec3(-45, -0.1, -80));
	gBunnies[2] = new BunnyPickup(glm::vec3(-38, -0.1, -72));
	gBunnies[3] = new BunnyPickup(glm::vec3(-36, -0.1, -82));
	gBunnies[4] = new BunnyPickup(glm::vec3(22, -0.1, 5));
	gBunnies[5] = new BunnyPickup(glm::vec3(18, -0.1, -3));
	gBunnies[6] = new BunnyPickup(glm::vec3(-50.0f, -0.1f, 65.0f));
	gBunnies[7] = new BunnyPickup(glm::vec3(30.0f, -0.1f, 70.0f));

	//Golden bunny pickup initialised
	for (int k = 0; k < numOfgBunnies; k++)
	{
		gBunnies[k]->initialise();
	}

	myHobgoblin = new Hobgoblin();
	myHobgoblin->initialise();

	myCamera = new Camera();
	myCamera->initialise();

	myHUD = new OnscreenHUD();
	myHUD->initialise();

	myWinScreen = new OnscreenWinHUD();
	myWinScreen->initialise();

	myDeathScreen = new OnscreenDeadHUD();
	myDeathScreen->initialise();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

}

//Reset everything in the game
void reset()
{
	//Moving Platforms reset
	for (int i = 0; i < numOfmPlatforms; i++)
	{

		mPlatform[i]->reset();
	}

	//Golden bunny pickup reset
	for (int k = 0; k < numOfgBunnies; k++)
	{
		gBunnies[k]->reset();
	}

	myHobgoblin->reset();

	myCamera->reset();

	myHUD->reset();

	myWinScreen->reset();

	myDeathScreen->reset();
}

void update(void) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if ( keys[SDL_SCANCODE_1] ) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	if ( keys[SDL_SCANCODE_2] ) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	//if the Hobgoblin has won then allow the player to press R for reset
	if (myHobgoblin->hasWon() || myHobgoblin->hasDied())
	{ 
		if (keys[SDL_SCANCODE_R])
		{
			reset();
		}
	}
	

	for (int i = 0; i < numOfmPlatforms; i++)
	{
		mPlatform[i]->update(myHobgoblin);
	}

	for (int j = 0; j < numOfgBunnies; j++)
	{
		gBunnies[j]->update(myHobgoblin);
	}

	myHobgoblin->update(lrgRock, numOfrocks);

	myCamera->update(myHobgoblin->getPos(), myHobgoblin->getRotate(), myHobgoblin->hasWon());

	myHUD->update(myHobgoblin);
	myWinScreen->update(myHobgoblin);
	myDeathScreen->update(myHobgoblin);
	myGround->update(myHobgoblin);

	
}


void draw(SDL_Window * window) {

	// clear the screen
	glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection(1.0);
	projection = glm::perspective(60.0f, 800.0f / 600.0f, 1.0f, 150.0f);
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection));

	GLfloat scale(1.0f); // just to allow easy scaling of complete scene

	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview);

	//draw the camera
	myCamera->render(mvStack);

	// draw skybox here
	mySkybox->render(mvStack, projection);

	// back to remainder of rendering
	glDepthMask(GL_TRUE); // make sure depth test is on

	glUseProgram(shaderProgram);

	glm::vec4 tmp = mvStack.top()*lightPos;
	light0.position[0] = tmp.x;
	light0.position[1] = tmp.y;
	light0.position[2] = tmp.z;
	rt3d::setLightPos(shaderProgram, glm::value_ptr(tmp));


	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection));

	//draw the ground cube here
	myGround->render(mvStack);

	//draw large lavapool here
	lavapool->render(mvStack);

	//drawing moving platforms here
	for (int i = 0; i < numOfmPlatforms; i++)
	{
		mPlatform[i]->render(mvStack);
	}

	//drawing large rocks here
	for (int j = 0; j < numOfrocks; j++)
	{
		lrgRock[j]->render(mvStack);
	}

	//drawing golden bunnies here
	for (int k = 0; k < numOfgBunnies; k++)
	{
		gBunnies[k]->render(mvStack);
	}

	//draw the hobgoblin
	myHobgoblin->render(mvStack);

	//draw the onscreen labels here
	myHUD->render(mvStack, mySkybox);

	//draw win screen here
	myWinScreen->render(mvStack, mySkybox);

	//draw death screen here
	myDeathScreen->render(mvStack, mySkybox);

	glEnable(GL_DEPTH_TEST);//Re-enable depth test after HUD label 

	//// remember to use at least one pop operation per push...
	mvStack.pop(); // initial matrix
	glDepthMask(GL_TRUE);
	
	SDL_GL_SwapWindow(window); // swap buffers
}

// Program entry point - SDL manages the actual WinMain entry point for us
int main(int argc, char *argv[]) {
    SDL_Window * hWindow; // window handle
    SDL_GLContext glContext; // OpenGL context handle
    hWindow = setupRC(glContext); // Create window and render context 

	// Required on Windows *only* init GLEW to access OpenGL beyond 1.1
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << endl;
		exit (1);
	}
	cout << glGetString(GL_VERSION) << endl;

	init();

	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events
	while (running)	{	// the event loop
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update();
		draw(hWindow); // call the draw function
	}

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(hWindow);
    SDL_Quit();
    return 0;
}
