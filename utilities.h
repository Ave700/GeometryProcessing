#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H
#include "globals.h"
#ifndef _WIN32
#include <unistd.h>
#endif // !_WIN32
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>
#include <limits>


bool ReadFile(const char* fileName, std::string& output);
#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#ifdef _WIN32
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#endif