// docs.gl - site with openGL commands description
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Macro for assertion, if failed break debug
#define ASSERT(x) if (!(x)) __debugbreak();
// Macro for error checking in opengl 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "ERROR " << error << function << " "
			<< file << ":" << line <<  '\n';
		return false;
	}
	return true;
}

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
	GLCall(unsigned int id = glCreateShader(type)); // Creates a shader object
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr)); // Replaces the source code in a shader object
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char)); // The way to get array of length length
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << '\n';
		std::cout << message << '\n';
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	GLCall(unsigned int program = glCreateProgram()); // Creates a program object
	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	GLCall(glAttachShader(program, vs)); // Attaches a shader objet to a program object
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program)); // Links a program object
	GLCall(glValidateProgram(program)); // Validates a program object

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

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
	GLCall(glGenBuffers(1, &buffer)); // Generate buffer object names
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // Bind a named buffer object
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); // Creates and initializes a buffer object's data store

	
	GLCall(glEnableVertexAttribArray(0)); // Enable a generic vertex attribute array
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // Define an array of generic vertex attribute data

	unsigned int ibo; // index buffer
	GLCall(glGenBuffers(1, &ibo)); // Generate buffer object names
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // Bind a named buffer object
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); // Creates and initializes a buffer object's data store

	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
	GLCall(glUseProgram(shader));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


  		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}