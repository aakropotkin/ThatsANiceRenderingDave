#include <GL/glew.h>
#include <dirent.h>

#include "render_pass.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>

int window_width = 800, window_height = 600;
const std::string window_title = "Particles";

const char* particle_vertex_shader =
#include "shaders/particle.vert"
;

const char* particle_geometry_shader =
#include "shaders/particle.geom"
;

const char* particle_fragment_shader =
#include "shaders/particle.frag"
;

// FIXME: Add more shaders here.

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

GLFWwindow* init_glefw()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);
	CHECK_SUCCESS(ret != nullptr);
	glfwMakeContextCurrent(ret);
	glewExperimental = GL_TRUE;
	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	return ret;
}

int main(int argc, char* argv[])
{
	GLFWwindow *window = init_glefw();
    

    //Binders:
//    auto float_binder = [](int loc, const void* data) {
//        glUniform1fv(loc, 1, (const GLfloat*)data);
//    };
    
    /*
     * These lambda functions below are used to retrieve data
     */
//    auto selected_b_data = [&gui]() -> const void* {
//        static const glm::vec4 zero = glm::vec4(0, 0, 0, 0);
//        if (gui.getCurrentBone() >= 0) {
//            glm::vec4& n = gui.getCurrentRotation()[2];
//            return &n;
//        }
//        return &zero;
//    };
    
    //Uniforms:
//    ShaderUniform selected_b = { "selected_b", vector_binder, selected_b_data };
    
    std::vector<glm::vec4> points;
    points.push_back(glm::vec4(10, 10, 0, 0));
    
    RenderDataInput particle_pass_input;
    particle_pass_input.assign(0, "vertex_position", nullptr, points.size(), 4, GL_FLOAT);
    particle_pass_input.assign_index(points.data(), points.size(), 4);
    RenderPass particle_pass(-1,
                           particle_pass_input,
                           {
                               particle_vertex_shader,
                               particle_geometry_shader,
                               particle_fragment_shader
                           },
                           { /* uniforms */ },
                           { "fragment_color" }
                           );

	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

        //TODO: Draw here
        particle_pass.updateVBO(0, points.data(), points.size());
        
        particle_pass.setup();
        CHECK_GL_ERROR(glDrawElements(GL_POINTS, points.size(), GL_UNSIGNED_INT, 0));
        
		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
    
	exit(EXIT_SUCCESS);
}