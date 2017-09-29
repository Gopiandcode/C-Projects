// make sure to include glew  before glfw
// it's like a big header file
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


#include "shader.h"
#include "shaderloader.h"
#include "logger.h"

static int g_win_width = 640;
static int g_win_height = 480;

static int g_fb_width = 640;
static int g_fb_height = 480;

void glPrintData(FILE *fp, int ref, char *str) {
	fprintf(fp, "%s: %s\n",str, glGetString(ref));
}


void glfw_error_callback(int error, const char *description) {
	logger_err("GLFW ERROR[%i] -> %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow *window, int width, int height) {
	g_win_width = width;
	g_win_height = height;
}

void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
	g_fb_width = width;
	g_fb_height = height;
}

void glfw_require_opengl32() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void glfw_enable_extras() {
	glfwWindowHint(GLFW_SAMPLES, 4);
}

void handleExitPress(GLFWwindow *window) {
	if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window,1);
	}	
}

GLFWwindow *createWindow(int fullscreen) {
	GLFWwindow *window = NULL;
	if(fullscreen) {
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode *vmode = glfwGetVideoMode(mon);
		window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);
	} else {
		window = glfwCreateWindow(640,490, "Hello Triangle", NULL, NULL);
	}
	if(!window || window == NULL) {
		logger_err("GLFWERR - Could not open a window\n");
		glfwTerminate();
		return NULL;
	}
	return window;
}

int main() {
	// initialize glfw
	
	glfw_require_opengl32();
	glfw_enable_extras();
	logger_log("Starting GLFW %s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		logger_err("ERROR: Could not start GLFW3\n");
		return 1;
	}


	// use glfw to create a window
	GLFWwindow *window = createWindow(1);
	if(window == NULL) return 1;

	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);



	// use this window - make opengl context use the window
	glfwMakeContextCurrent(window);

	// start glew
	glewExperimental = GL_TRUE;
	glewInit();

	glPrintData(stdout, GL_RENDERER, "Renderer");
	glPrintData(stdout, GL_VERSION, "OpenGL version supported");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	
	GLfloat points[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

	};

	GLfloat points2[] = {
		0.6f, 0.8f, 0.0f,
		0.4f, 0.6f, 0.0f,
		0.6f, 0.6f, 0.0f
	};

	GLuint vbo[] = {0,0};
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);



	GLuint vao[] = {0, 0};
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

	glBindVertexArray(vao[1]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);


	const char *code;
	code = loadshader("test.frag");
	shader_G fs = shader_new(GL_FRAGMENT_SHADER, code);

	code = loadshader("test.vert"); 
	shader_G vs = shader_new(GL_VERTEX_SHADER, code);


	code = loadshader("test2.frag");
	shader_G fs2 = shader_new(GL_FRAGMENT_SHADER, code);

	code = loadshader("test2.vert");
	shader_G vs2 = shader_new(GL_VERTEX_SHADER, code);

	GLuint shader_programme = glCreateProgram();
	
	glAttachShader(shader_programme, shader_get(fs));
	glAttachShader(shader_programme, shader_get(vs));
	glLinkProgram(shader_programme);


	GLuint shader_programme2 = glCreateProgram();
	glAttachShader(shader_programme2, shader_get(fs2));
	glAttachShader(shader_programme2, shader_get(vs2));
	glLinkProgram(shader_programme2);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.6f,0.6f, 0.8f, 1.0f);
		glViewport(0, 0, g_fb_width, g_fb_height);
		glUseProgram(shader_programme);

		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUseProgram(shader_programme2);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
		handleExitPress(window);
	}


	glfwTerminate();
	return 0;
}
