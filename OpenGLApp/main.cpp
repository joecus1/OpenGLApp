#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

// movement
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.00005f;

// rotation
float currAngle = 0.0f;
float angleIncrement = 0.01f;

// scaling
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
float scalingIncrement = 0.0001f;

static const char* vShaderFile = "Shaders/shader.vert";

static const char* fShaderFile = "Shaders/shader.frag";

float ToRadians(float num)
{
	return num * (3.14159265f / 180.0f);
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShaderFile, fShaderFile);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.0f, (float)mainWindow.GetBufferWidth() / (float)mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		// Get and handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		currAngle += angleIncrement;
		if (currAngle >= 360)
		{
			currAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += scalingIncrement;
		}
		else
		{
			curSize -= scalingIncrement;
		}
		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}


		if (abs(triOffset) >= triMaxOffset) direction = !direction;

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model(1.0f); // identity
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}