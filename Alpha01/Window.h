#pragma once
#include <GL/glew.h>
#include <SDL.h>
#undef main
#include "Commons.h"
#include "maths_func.h"
#include "font_manager.h"
#include "sound.h"
#include "sound_manager.h"

namespace gearengine { namespace graphics {

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

		class Window
		{
		public:
			const float MAX_FPS = 60.0f;

			Window();
			~Window();

			void clear() const;
			void update();
			bool closed() const;
			void destroy();
			bool isKeyPressed(unsigned int keycode) const;
			bool isKeyTyped(unsigned int keycode) const;
			bool isMouseButtonPressed(unsigned int button) const;
			bool isMouseButtonClicked(unsigned int button) const;
			void getMousePosition(double& x, double& y) const;
			void turnOffRunning();
			
		private:
			bool init();
			bool _isRunning;
			SDL_Window* _pWindow;
			SDL_GLContext gContext;

			bool m_Keys[MAX_KEYS];
			bool m_KeyState[MAX_KEYS];
			bool m_KeyTyped[MAX_KEYS];
			bool m_MouseButtons[MAX_BUTTONS];
			bool m_MouseState[MAX_BUTTONS];
			bool m_MouseClicked[MAX_BUTTONS];

			
		};

	}
}

