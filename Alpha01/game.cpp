#include "game.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace gearengine
{
	using namespace graphics;

	Game::Game()
	{

	}

	Game::~Game()
	{
		
	}

	void Game::init()
	{
		window = createWindow("Gear Engine", WIDTH, HEIGHT);
		FontManager::get()->setScale(WIDTH / 32.0f, HEIGHT / 18.0f);
		

		shader = new ModelShader();
		model = new Model(*shader, "resources/objects/nanosuit/nanosuit.obj");
		model->setModelPlacement(glm::vec3(0.0f, 0.0f, 3.0f), 0.2f, 45.0f);

		terrainShader = ModelShader();
		terrain = new Terrain(terrainShader, "heightmap512x512.png", "textures/props/cratepng.png");
		
		

		projectionMatrix = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		viewMatrix = glm::lookAt(
			glm::vec3(0.f, 5.f, -15.f),  // Camera position
			glm::vec3(0, 0, 0),			// Camera target
			glm::vec3(0, 1, 0));		// Camera orientation (up)

		model->init(projectionMatrix, viewMatrix);
		terrain->init(projectionMatrix, viewMatrix, glm::vec3(0, 0, 0));
		


		// Initialize music
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			std::cout << "Error: " << Mix_GetError() << std::endl;

		music = Mix_LoadMUS("music/testmusic.ogg");
	}

	void Game::tick()
	{
		
	}


	void Game::updateKeyMovement()
	{
		state = SDL_GetKeyboardState(NULL);

	}


	void Game::update()
	{
		window->clear();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				Mix_FreeMusic(music);
				music = nullptr;
				Mix_Quit();
				window->turnOffRunning();
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_p:
					if (!Mix_PlayingMusic())
						Mix_PlayMusic(music, -1);
					else if (Mix_PausedMusic())
						Mix_ResumeMusic();
					else
						Mix_PauseMusic();
					break;
				case SDLK_l:
					Mix_HaltMusic();
					break;
				}
			}
				

			if (event.type == SDL_MOUSEMOTION)
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			}
		}
		
		updateKeyMovement();
		
		
	}


	void Game::render()
	{	
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		terrain->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		model->draw();
	}	
}


