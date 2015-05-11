#pragma once

#pragma warning( disable : 4244 )

#include <windows.h>

const int WIDTH = GetSystemMetrics(SM_CXSCREEN) / 2;
const int HEIGHT = GetSystemMetrics(SM_CYSCREEN) / 2;
const float FOV = 70.0f;
const float NEARV = 0.1f;
const float FARV = 1000.0f;

const float ASPECT_RATIO = WIDTH / HEIGHT;

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()