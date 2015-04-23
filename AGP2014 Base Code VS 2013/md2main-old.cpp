/* md2model
 * Code taken from:
 * md2.c -- md2 model loader

 * Modified: March 2013 by Daniel Livingstone
 * to remove use of GLTools, replacing with calls to rt3d library

 * Modified: November 2010, by Daniel Livingstone
 * To put MD2 code into a simple class structure for easier reuse
 * AND to replace OpenGL 1.x/2.x calls with GLTools functions
 * (From OpenGL SuperBible 5th Edition)
 * This project is now OpenGL Core compliant (pretty much!)
 * but there is still lots of room for improvement... 
 *
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * gcc -Wall -ansi -lGL -lGLU -lglut md2.c -o md2
 */

#define WIN32
#include "rt3d.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include "md2model.h"

// * Global Variables *
//GLMatrixStack		projectionMatrix;
//GLFrustum			viewFrustum;
//GLShaderManager		shaderManager;
//GLMatrixStack		modelViewMatrix;
//GLGeometryTransform	transformPipeline;

md2model			model1, model2;
GLuint				textures[2];	
GLuint				currentAnim = MD2_STAND;

//void setupRC()
//{
//	glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
//	glShadeModel (GL_SMOOTH);
//	glEnable (GL_DEPTH_TEST);
//	glEnable (GL_TEXTURE_2D);
//	shaderManager.InitializeStockShaders();
//}

// Load textures and MD2 models
void init()
{
//	textures[0] = loadTexture("hayden.tga",&textures[0]);
	if (!model1.ReadMD2Model ("hayden-tris.md2"))
		exit (EXIT_FAILURE);

//	textures[1] = loadTexture("hueteotl.tga",&textures[1]);
	if (!model2.ReadMD2Model ("hueteotl.md2"))
		exit (EXIT_FAILURE);
}

void cleanup ()
{
  model1.FreeModel();
  model2.FreeModel();
}

//void reshape(GLint w, GLint h)
//{
//	glViewport(0, 0, w, h);
//	viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
//	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
//}

void display ()
{
	static float interp = 0.0;
	static double current_time = 0;
	static double last_time = 0;
	float dt;
	GLfloat vColour[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat vLightPos[] = {0.0f, 2.0f, -1.0f};

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	last_time = current_time;
	current_time = (double)glutGet (GLUT_ELAPSED_TIME) / 100.0;

	/* Animate model from frames 0 to num_frames-1 */
	interp += (current_time - last_time);
	dt = (current_time - last_time);
	model1.Animate (MD2_STAND, dt);

	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

	// Would be better to use Frames to store position & orientation of
	// the characters!
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(-1.5f,0.0f,-6.0f); // move object back and left
	modelViewMatrix.Rotate(-90,1.0f,0.0f,0.0f);   // turn upright
	modelViewMatrix.Rotate(-90,0.0f,0.0f,1.0f);   // turn to face
	modelViewMatrix.Scale(0.05f,0.05f,0.05f);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
		modelViewMatrix.GetMatrix(), projectionMatrix.GetMatrix(),
		vLightPos, vColour, 0);
	glBindTexture(GL_TEXTURE_2D,textures[0]);

	/* Draw the model */
	model1.RenderFrameItp();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(1.5f,0.0f,-6.0f); // move object back and right
	modelViewMatrix.Rotate(-90,1.0f,0.0f,0.0f);   // turn upright
	modelViewMatrix.Rotate(-90,0.0f,0.0f,1.0f);   // turn to face
	modelViewMatrix.Scale(0.05f,0.05f,0.05f);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
		modelViewMatrix.GetMatrix(), projectionMatrix.GetMatrix(),
		vLightPos, vColour, 0);
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	model2.Animate (currentAnim, dt);
	model2.RenderFrameItp();
	modelViewMatrix.PopMatrix();

	if (interp >= 1.0f)
		interp = 0.0f;
	glutSwapBuffers ();
	glutPostRedisplay ();
}

//void keyboard (unsigned char key, int x, int y)
//{
//  /* Escape */
//  if (key == 27)
//    exit (0);
//  currentAnim++;
//  if (currentAnim >19)
//	  currentAnim = 0;
//}
//
