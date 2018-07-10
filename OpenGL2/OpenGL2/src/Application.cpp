// docs.gl - site with openGL commands description
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};
static ShaderProgramSource ParseShader(const std::string filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos){
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}
			}
			else {
				ss[(int)type] << line << '\n';
			}
		}
		return { ss[0].str(), ss[1].str() };
}
static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type); // Creates a shader object
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); // Replaces the source code in a shader object
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // The way to get array of length length
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << '\n';
		std::cout << message << '\n';
		glDeleteShader(id);
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram(); // Creates a program object
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs); // Attaches a shader objet to a program object
	glAttachShader(program, fs);
	glLinkProgram(program); // Links a program object
	glValidateProgram(program); // Validates a program object

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR" << std::endl;

	std::cout << glGetString(GL_VERSION) << '\n';

	//Positions of vertices
	float positions[] = {
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,	0.5f, // 2
		-0.5f,  0.5f  // 3
	};

	unsigned int indices[]{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); // Generate buffer object names
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // Bind a named buffer object
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW); // Creates and initializes a buffer object's data store

	
	glEnableVertexAttribArray(0); // Enable a generic vertex attribute array
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // Define an array of generic vertex attribute data

	unsigned int ibo; // index buffer
	glGenBuffers(1, &ibo); // Generate buffer object names
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Bind a named buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // Creates and initializes a buffer object's data store

	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}