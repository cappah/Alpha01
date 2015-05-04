#pragma once

#pragma warning( disable : 4244 )

#include <windows.h>

const int WIDTH = GetSystemMetrics(SM_CXSCREEN) / 2;
const int HEIGHT = GetSystemMetrics(SM_CYSCREEN) / 2;
const float FOV = 70.0f;
const float NEARV = 0.1f;
const float FARV = 1000.0f;

const float ASPECT_RATIO = WIDTH / HEIGHT;