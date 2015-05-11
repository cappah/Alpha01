#pragma once
#include <FreeImage.h>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

#define FOR(q, n) for(int q = 0; q < n; q++)
#define SFOR(q, s, e) for(int q = s; q <= e; q++)
#define RFOR(q, n) for(int q = n; q >= 0; q--)
#define RSFOR(q, s, e) for(int q = s; q >= e; q--)

#define ESZ(elem) (int)elem.size()

namespace gearengine
{
	namespace graphics
	{
		enum ETextureFiltering
		{
			TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
			TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
			TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
			TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
			TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
			TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
			TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
		};


		class TTexLoader
		{
		public:
			TTexLoader();
			~TTexLoader();

			void CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format);
			void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps = false);

			bool ReloadTexture();

			bool LoadTexture2D(std::string a_sPath, bool bGenerateMipMaps = false);
			void BindTexture(int iTextureUnit = 0);

			void SetFiltering(int a_tfMagnification, int a_tfMinification);

			void SetSamplerParameter(GLenum parameter, GLenum value);

			int GetMinificationFilter();
			int GetMagnificationFilter();

			int GetWidth();
			int GetHeight();
			int GetBPP();

			GLuint GetTextureID();
			

			std::string GetPath();


		private:
			int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
			GLuint uiTexture; // Texture name
			GLuint uiSampler; // Sampler name
			bool bMipMapsGenerated;

			int tfMinification, tfMagnification;

			std::string sPath;

			void DeleteTexture();
		};

#define NUMTEXTURES 5
		extern TTexLoader tTextures[NUMTEXTURES];
		void LoadAllTextures();
	}
}


