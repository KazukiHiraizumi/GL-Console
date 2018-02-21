#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

static void error_callback(int error, const char* description){
    fputs(description, stderr);
}
int main(void){
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) exit(EXIT_FAILURE);

	// cout << "default shader lang: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// select opengl version
	// int major, minor, rev;
	// glfwGetVersion(&major, &minor, &rev);
	// cout << "glfw major.minor " << major << "." << minor << "." << rev << endl;

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	cout << "OpenGL shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	int width, height;
 	glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float) height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	float tpre=(float)glfwGetTime();
	float xelm=-2.2;
	float tmax=0;
	while (!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		float t=(float) glfwGetTime();
		float dt=t-tpre;
		tpre=t;
		glLoadIdentity();
//		glRotatef(0.f, 0.f, 0.f, 1.f);
		xelm+=1.f*dt;
		if(xelm>1.6) xelm=-2.2;
		glTranslatef(xelm, 0.f, 0.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		if(tmax<dt){
			tmax=dt;
			cout<<dt<<endl;
		}
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
