#include <GL/glew.h>
#include "render_pass.h"
#include "gui.h"

#include <fstream>
#include <iostream>

#include <random>
#include <thread>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>

#include "ParticleSystem.h"
#include "GravitySystem.h"
#include "SpaceSystem.h"
#include "OpenGLUtil.h"
#include "SmokeSystem.h"
#include "MassParticle.h"

using std::vector;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::uvec1;

int window_width = 1000, window_height = 1000;

int main(int argc, char* argv[])
{
    OpenGLUtil openGL = OpenGLUtil(window_width, window_height, "Particles");
    GLFWwindow* window = openGL.setup();
    GUI gui(window);

    vector<ParticleSystem*> systems;
    systems.push_back(new GravitySystem());
    systems.push_back(new SpaceSystem());
    for (ParticleSystem* system : systems){
        system->width = window_width;
        system->height = window_height;
        system->setup();
        system->prepareDraw();
    }

    SmokeSystem* rootSystem = new SmokeSystem();
    rootSystem->width = window_width * 2;
    rootSystem->height = window_height * 2;
    rootSystem->setup();
    rootSystem->prepareDraw();
    systems.push_back(rootSystem);
    gui.delegates.push_back(rootSystem);

    // **************
    //
    // ANIMATION LOOP
    //
    // **************


	while (openGL.drawBool()) {
        openGL.beforeDraw();

        for (ParticleSystem* system : systems) {
            //Step our systems
            system->step();

            //TODO: Draw here
            system->draw();
        }
    openGL.afterDraw();
	}
    openGL.destroy();
}
