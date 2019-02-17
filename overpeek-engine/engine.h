#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <fstream>

#include <iostream>
#include <omp.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "audio/audioManager.h"

#include "math/noise.h"
#include "math/math.h"
#include "math/FastNoise.h"
#include "math/FastNoiseSIMD/FastNoiseSIMD.h"

#include "utility/clock.h"
#include "utility/random.h"
#include "utility/binaryIO.h"
#include "utility/logger.h"
#include "utility/debug.h"
#include "utility/gameloop.h"
#include "utility/filereader.h"

#include "graphics/camera.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "graphics/textureManager.h"
#include "graphics/buffers/buffer.h"
#include "graphics/buffers/vertexArray.h"
#include "graphics/buffers/indexBuffer.h"