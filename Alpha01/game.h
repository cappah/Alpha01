#pragma once
#include "gear_engine.h"
//#include "sound.h"
//#include "sound_manager.h"
#include <SDL_mixer.h>
#include "ModelShader.h"
#include <GL/glew.h>
#include "Model.h"
#include "Terrain.h"
#include "ModelMesh.h"


namespace gearengine
{
	using namespace graphics;
	using namespace model;
	

	class Game : public GearEngine
	{
	private:
		Window* window;
		
		//Camera* camera;
		ModelShader* shader;
		Model* model;
		ModelShader terrainShader;
		SDL_Event event;
		const Uint8 *state;
		int mouseX;
		int mouseY;
		float scale = 50.0f;
		float half_scale = scale / 2.f;
		Mix_Music* music;
		
		void updateKeyMovement();
		float mouseConv(float dir, int mouse);

		Terrain* terrain;
		MMesh* mModel;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 modelMatrix;
		glm::mat4 scaleMatrix;
		glm::mat4 rotateMatrix;

		

		
	public:
		Game();
		~Game();
		

		void init() override;
		void tick() override;
		void update() override;
		void render() override;
	};

}