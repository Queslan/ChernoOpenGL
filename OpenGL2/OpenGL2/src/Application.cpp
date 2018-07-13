// docs.gl - site with openGL commands description

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"


int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 560, "777", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // setup window to refresh same rate as monitor

	if (glewInit() != GLEW_OK)
		std::cout << "glewInit ERROR" << '\n';

	std::cout << glGetString(GL_VERSION) << '\n'; // show used version of opengl

	/* Extra scope for variables to be deleted from stack */
	{ 
		//Positions of vertices
		float positions[] {
			 100.0f, 100.0f, 0.0f, 0.0f, // 0
			 200.0f, 100.0f, 1.0f, 0.0f, // 1
			 200.0f, 200.0f, 1.0f, 1.0f, // 2
			 100.0f, 200.0f, 0.0f, 1.0f  // 3
		};

		unsigned int indices[] {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);
		
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 560.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model;
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/img.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);
		
		/* Unbind all */
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		float r = 0.0f;
		float changeValue = 0.05f;
	
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			ImGui_ImplOpenGL3_NewFrame();

			shader.Bind();	
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			
			renderer.Draw(va, ib, shader);


			if (r > 1.0f)
				changeValue = -0.05f;
			else if (r < 0.0f)
				changeValue = 0.05f;

			r += changeValue;

			{
				static float f = 0.0f;
				static int counter = 0;
				ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}