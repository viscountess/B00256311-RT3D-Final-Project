#include "hobgoblin.h"
#include <SDL_ttf.h>
#include "Utils.h"
#include "collectable.h"
#include "movingPlatform.h"
using namespace std;



//constructor
Hobgoblin::Hobgoblin()
{
	int currentAnim = 0;
}

//deconstructor
Hobgoblin::~Hobgoblin()
{

}

void Hobgoblin::initialise()
{
	textures[1] = rt3d::loadBitmap("hobgoblin2.bmp");
	meshObjects[1] = tmpModel.ReadMD2Model("tris.MD2");
	md2VertCount = tmpModel.getVertDataCount();

	material1 = {
		{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
		1.0f  // shininess
	};

	shaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");
	//rt3d::setLight(shaderProgram, light0);

	reset();

	/* Initialize default output device */
	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		cout << "Can't initialize device";


	/* Load a sample from "file" and give it a max of 3 simultaneous
	playings using playback position as override decider */
	//sound courtesy of http://www.freesfx.co.uk/
	pickupSound = loadSound("bunny_pickup.mp3");
	//sound courtesy of http://www.audiomicro.com/
	deathSound = loadSound("death scream.wav");

}

//Reset the gameplay values
void Hobgoblin::reset()
{
	pos.x = 20.0f;
	pos.y = 1.2f;
	pos.z = -60.0f;
	rotate = 0;
	currBunny = nullptr;
	currPlatform = nullptr;
	collectCounter = 0;
	//This is the maximum number of collectables in the game
	maxNumOfCollectables = 8;
	isOnSolidGround = true;
	isDead = false;
}

void Hobgoblin::render(std::stack<glm::mat4>& _Stack)
{
	GLfloat scale(1.0f);

	// Animate the md2 model, and update the mesh with new vertex data
	tmpModel.Animate(currentAnim, 0.1, !hasDied());
	rt3d::updateMesh(meshObjects[1], RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	// draw the hobgoblin
	glCullFace(GL_FRONT); // md2 faces are defined clockwise, so cull front face
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	rt3d::materialStruct tmpMaterial = material1;
	rt3d::setMaterial(shaderProgram, tmpMaterial);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), pos);
	_Stack.top() = glm::rotate(_Stack.top(), rotate + 90.f, glm::vec3(0.0f, -1.0f, 0.0f));
	_Stack.top() = glm::rotate(_Stack.top(), 90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
	
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(scale*0.05, scale*0.05, scale*0.05));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::drawMesh(meshObjects[1], md2VertCount, GL_TRIANGLES);
	_Stack.pop();
	glCullFace(GL_BACK);
}

HSAMPLE Hobgoblin::loadSound(char * filename)
{
	HSAMPLE sam;
	if (sam = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_OVER_POS))
		cout << "sample " << filename << " loaded!" << endl;
	else
	{
		cout << "Can't load sample";
		exit(0);
	}
	return sam;

}


void Hobgoblin::update(LargeRock **_rocks, int numberOfRocks)
{
	//If the player has won just stop updating
	if (hasWon())
		return;
	//if the player has died then stop updating
	if (hasDied())
		return;

	//if the player is not on a platform then return true that player has died
	if (!isOnSolidGround && !currPlatform)
	{
		isDead = true;
		//if player has died then play appropiate animation
		currentAnim = 19;

		HCHANNEL ch = BASS_SampleGetChannel(deathSound, FALSE);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.2);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, -1);
		if (!BASS_ChannelPlay(ch, FALSE))
			cout << "Can't play sample" << endl;
		return;
	}
		

	glm::vec3 previousPos = pos;

	if (currentAnim != 11 || ((currentAnim == 11) && (tmpModel.getCurrentFrame()==134)))
	{
		currentAnim = 0;
	}
	

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]){
		//eye = moveForward(eye, r, 0.1f);
		//pos.z += 0.1f;d
		pos = Utils::moveForward(pos, rotate, -0.1f);
		currentAnim = 1;

	}
	if (keys[SDL_SCANCODE_S]) //eye = moveForward(eye, r, -0.1f);
	{
		//pos.z -= 0.1f;
		pos = Utils::moveForward(pos, rotate, 0.1f);
		currentAnim = 1;
	}

	//strafe left
	if (keys[SDL_SCANCODE_A])
	{
		currentAnim = 1;
		pos = Utils::moveRight(pos, rotate, 0.1f);
	}

	//strafe right
	if (keys[SDL_SCANCODE_D])
	{
		currentAnim = 1;
		pos = Utils::moveRight(pos, rotate, -0.1f);
	}

	//look left
	if (keys[SDL_SCANCODE_Q]) //eye = moveRight(eye, r, -0.1f);
	{
		rotate -= 0.4;
	}

	//look right
	if (keys[SDL_SCANCODE_E]) //eye = moveRight(eye, r, 0.1f);
	{
		rotate += 0.4;
	}



	if (keys[SDL_SCANCODE_Z]) {
		if (--currentAnim < 0) currentAnim = 19;
		cout << "Current animation: " << currentAnim << endl;
	}
	if (keys[SDL_SCANCODE_X]) {
		if (++currentAnim >= 20) currentAnim = 0;
		cout << "Current animation: " << currentAnim << endl;
	}

	if (keys[SDL_SCANCODE_SPACE])
	{
		currentAnim = 11;
		
		if (currBunny)
		{
			currBunny->pickUp();
			collectCounter++;

			HCHANNEL ch = BASS_SampleGetChannel(pickupSound, FALSE);
			BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
			BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.2);
			BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, -1);
			if (!BASS_ChannelPlay(ch, FALSE))
				cout << "Can't play sample" << endl;

			//if the player has won then play appropiate animation
			if (hasWon())
				currentAnim = 7;
		}
	}
	currBunny = nullptr;
	
	//If the hobgoblin is on a moving platform
	if (currPlatform)
	{
		//then move hobgoblin with platform
		pos += currPlatform->getOffset();
	}
	currPlatform = nullptr;

	for (int i = 0; i < numberOfRocks; i++)
	{
		LargeRock *currRock = _rocks[i];
		//A local vector representing the difference between the player and the rock
		glm::vec3 diff = pos - currRock->getRockPos();
		diff.y = 0;

		if (Utils::length(diff) < (currRock->getRockRadius() +0.5))
		{
			//normalising the diff length
			diff /= Utils::length(diff);

			//this pushes the player to the outer limits of the rock
			//so that player can walk around the rock
			diff *= (currRock->getRockRadius()+0.5);

			float height = pos.y;
			pos = currRock->getRockPos() + diff;
			pos.y = height;
		}
	}
}