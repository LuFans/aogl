#pragma once

using uint = unsigned int;
using ushort = unsigned short;
using uchar = unsigned char;

/* OpenGL Header */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* Standard Libary Header */
#include <string>
#include <memory>
#include <sstream>
#include <fstream>
#include <map>
#include <functional>
#include <vector>
#include <array>

#include "garbage_collector.hpp"
