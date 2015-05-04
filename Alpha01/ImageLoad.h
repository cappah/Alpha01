#pragma once

#include <string>
#include <FreeImage.h>

namespace gearengine {

	static BYTE* load_image(const char* filename, GLsizei* width, GLsizei* height)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = nullptr;
		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		BYTE* pixels = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);

		int size = *width * *height * (bits / 8);
		BYTE* result = new BYTE[size];
		memcpy(result, pixels, size);
		FreeImage_Unload(dib);
		return result;
	}


	static GLuint loadTexture2D(std::string a_sPath, std::string directory)
	{
		std::string filename = std::string(a_sPath);
		filename = directory + '/' + filename;
		//printf("filename: %s", filename);

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib(0);

		fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format

		if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());

		if (fif == FIF_UNKNOWN) // If still unkown, return failure
			return false;

		if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
			dib = FreeImage_Load(fif, a_sPath.c_str());
		if (!dib)
			return false;

		BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
		GLuint textureID;
		GLuint iWidth = FreeImage_GetWidth(dib); // Get the image width and height
		GLuint iHeight = FreeImage_GetHeight(dib);
		GLuint iBPP = FreeImage_GetBPP(dib);
		
		// If somehow one of these failed (they shouldn't), return failure
		if (bDataPointer == NULL || iWidth == 0 || iHeight == 0)
			return false;

		// Generate an OpenGL texture ID for this texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
		int iInternalFormat = iBPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		FreeImage_Unload(dib);

		return textureID; // Success
	}


}