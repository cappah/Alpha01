#include "Window.h"
#include <iostream>
#include <time.h>
#include <math.h>

#include "shader.h"
#include "buffer.h"
#include "fileutils.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "texture.h"
#include "timer.h"


namespace gearengine {
	namespace graphics {

		using namespace maths;

		Window::Window() :
			_isRunning(true)
		{
			if (!init())
				SDL_Quit();

			FontManager::add(new Font("SourceSansPro", "SourceSansPro-Light.ttf", 16));
			//audio::SoundManager::init();

			for (int i = 0; i < MAX_KEYS; i++)
			{
				m_Keys[i] = false;
				m_KeyState[i] = false;
				m_KeyTyped[i] = false;
			}

			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				m_MouseButtons[i] = false;
				m_MouseState[i] = false;
				m_MouseClicked[i] = false;
			}
		}


		Window::~Window()
		{
			FontManager::clean();
			//audio::SoundManager::clean();
			SDL_Quit();
		}


#pragma region SDL2/GLew Initialization
		bool Window::init()
		{
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			{
				std::cout << "Unable to initialize SDL" << std::endl;
			}

			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

			/* Turn on double buffering with a 24bit Z buffer.
			* You may need to change this to 16 or 32 for your system */
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

			/* For anti-aliasing */
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 4);

			/* Create our window centered at resolution */
			_pWindow = SDL_CreateWindow("Game Zero", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

			if (!_pWindow)
			{
				std::cout << "SDL Error: " << SDL_GetError() << std::endl;
				SDL_Quit();
			}

			/* Create our opengl context and attach it to our window */
			gContext = SDL_GL_CreateContext(_pWindow);

			GLenum status;
			glewExperimental = GL_TRUE;
			status = glewInit();

			if (GLEW_OK != status)
			{
				std::cout << "Error: " << glewGetErrorString(status) << std::endl;
				exit(1);
			}
			else
				std::cout << "GLEW Init: Success!" << std::endl;

			glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);

			glLineWidth(0.5f);
			glEnable(GL_LINE_SMOOTH); 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			glEnable(GL_BLEND);


			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl << std::endl;

			/* This makes our buffer swap synchronized with the monitor's vertical refresh */
			SDL_GL_SetSwapInterval(0);

			return true;
		}
#pragma endregion


#pragma region Update
		void Window::update()
		{
			for (int i = 0; i < MAX_KEYS; i++)
				m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];

			for (int i = 0; i < MAX_BUTTONS; i++)
				m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

			memcpy(m_KeyState, m_Keys, MAX_KEYS);
			memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);

			

			// OpenGL Error checker
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				std::cout << "OpenGL Error: " << error << std::endl;

			SDL_GL_SwapWindow(_pWindow);
			
		}
#pragma endregion

		bool Window::closed() const
		{
			
			return _isRunning == false;
		}


		void Window::destroy()
		{
			SDL_GL_DeleteContext(gContext);
			SDL_DestroyWindow(_pWindow);
			SDL_Quit();
		}

#pragma region KEY & MOUSE INPUTS
		bool Window::isKeyPressed(unsigned int keycode) const
		{
			// TODO: Log this!
			if (keycode >= MAX_KEYS)
				return false;

			return m_Keys[keycode];
		}

		bool Window::isKeyTyped(unsigned int keycode) const
		{
			// TODO: Log this!
			if (keycode >= MAX_KEYS)
				return false;

			return m_KeyTyped[keycode];
		}

		bool Window::isMouseButtonPressed(unsigned int button) const
		{
			// TODO: Log this!
			if (button >= MAX_BUTTONS)
				return false;

			return m_MouseButtons[button];
		}

		bool Window::isMouseButtonClicked(unsigned int button) const
		{
			// TODO: Log this!
			if (button >= MAX_BUTTONS)
				return false;

			return m_MouseClicked[button];
		}
#pragma endregion


		void Window::clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		void Window::turnOffRunning()
		{
			_isRunning = false;
		}
	}
}