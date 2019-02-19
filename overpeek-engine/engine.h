#pragma once

#define _CRT_SECURE_NO_WARNINGS

//#include <fstream>
//
//#include <iostream>
//#include <omp.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

//#include "graphics/camera.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "graphics/textureManager.h"
#include "graphics/buffers/buffer.h"
#include "graphics/buffers/vertexArray.h"
#include "graphics/buffers/indexBuffer.h"


//Keys
#define OE_KEY_SPACE              32
#define OE_KEY_APOSTROPHE         39  /* ' */
#define OE_KEY_COMMA              44  /* , */
#define OE_KEY_MINUS              45  /* - */
#define OE_KEY_PERIOD             46  /* . */
#define OE_KEY_SLASH              47  /* / */
#define OE_KEY_0                  48
#define OE_KEY_1                  49
#define OE_KEY_2                  50
#define OE_KEY_3                  51
#define OE_KEY_4                  52
#define OE_KEY_5                  53
#define OE_KEY_6                  54
#define OE_KEY_7                  55
#define OE_KEY_8                  56
#define OE_KEY_9                  57
#define OE_KEY_SEMICOLON          59  /* ; */
#define OE_KEY_EQUAL              61  /* = */
#define OE_KEY_A                  65
#define OE_KEY_B                  66
#define OE_KEY_C                  67
#define OE_KEY_D                  68
#define OE_KEY_E                  69
#define OE_KEY_F                  70
#define OE_KEY_G                  71
#define OE_KEY_H                  72
#define OE_KEY_I                  73
#define OE_KEY_J                  74
#define OE_KEY_K                  75
#define OE_KEY_L                  76
#define OE_KEY_M                  77
#define OE_KEY_N                  78
#define OE_KEY_O                  79
#define OE_KEY_P                  80
#define OE_KEY_Q                  81
#define OE_KEY_R                  82
#define OE_KEY_S                  83
#define OE_KEY_T                  84
#define OE_KEY_U                  85
#define OE_KEY_V                  86
#define OE_KEY_W                  87
#define OE_KEY_X                  88
#define OE_KEY_Y                  89
#define OE_KEY_Z                  90
#define OE_KEY_LEFT_BRACKET       91  /* [ */
#define OE_KEY_BACKSLASH          92  /* \ */
#define OE_KEY_RIGHT_BRACKET      93  /* ] */
#define OE_KEY_GRAVE_ACCENT       96  /* ` */
#define OE_KEY_WORLD_1            161 /* non-US #1 */
#define OE_KEY_WORLD_2            162 /* non-US #2 */

#define OE_KEY_ESCAPE             256
#define OE_KEY_ENTER              257
#define OE_KEY_TAB                258
#define OE_KEY_BACKSPACE          259
#define OE_KEY_INSERT             260
#define OE_KEY_DELETE             261
#define OE_KEY_RIGHT              262
#define OE_KEY_LEFT               263
#define OE_KEY_DOWN               264
#define OE_KEY_UP                 265
#define OE_KEY_PAGE_UP            266
#define OE_KEY_PAGE_DOWN          267
#define OE_KEY_HOME               268
#define OE_KEY_END                269
#define OE_KEY_CAPS_LOCK          280
#define OE_KEY_SCROLL_LOCK        281
#define OE_KEY_NUM_LOCK           282
#define OE_KEY_PRINT_SCREEN       283
#define OE_KEY_PAUSE              284
#define OE_KEY_F1                 290
#define OE_KEY_F2                 291
#define OE_KEY_F3                 292
#define OE_KEY_F4                 293
#define OE_KEY_F5                 294
#define OE_KEY_F6                 295
#define OE_KEY_F7                 296
#define OE_KEY_F8                 297
#define OE_KEY_F9                 298
#define OE_KEY_F10                299
#define OE_KEY_F11                300
#define OE_KEY_F12                301
#define OE_KEY_F13                302
#define OE_KEY_F14                303
#define OE_KEY_F15                304
#define OE_KEY_F16                305
#define OE_KEY_F17                306
#define OE_KEY_F18                307
#define OE_KEY_F19                308
#define OE_KEY_F20                309
#define OE_KEY_F21                310
#define OE_KEY_F22                311
#define OE_KEY_F23                312
#define OE_KEY_F24                313
#define OE_KEY_F25                314
#define OE_KEY_KP_0               320
#define OE_KEY_KP_1               321
#define OE_KEY_KP_2               322
#define OE_KEY_KP_3               323
#define OE_KEY_KP_4               324
#define OE_KEY_KP_5               325
#define OE_KEY_KP_6               326
#define OE_KEY_KP_7               327
#define OE_KEY_KP_8               328
#define OE_KEY_KP_9               329
#define OE_KEY_KP_DECIMAL         330
#define OE_KEY_KP_DIVIDE          331
#define OE_KEY_KP_MULTIPLY        332
#define OE_KEY_KP_SUBTRACT        333
#define OE_KEY_KP_ADD             334
#define OE_KEY_KP_ENTER           335
#define OE_KEY_KP_EQUAL           336
#define OE_KEY_LEFT_SHIFT         340
#define OE_KEY_LEFT_CONTROL       341
#define OE_KEY_LEFT_ALT           342
#define OE_KEY_LEFT_SUPER         343
#define OE_KEY_RIGHT_SHIFT        344
#define OE_KEY_RIGHT_CONTROL      345
#define OE_KEY_RIGHT_ALT          346
#define OE_KEY_RIGHT_SUPER        347
#define OE_KEY_MENU               348

/*! @brief If this bit is set one or more Shift keys were held down.
 */
#define OE_MOD_SHIFT           0x0001
 /*! @brief If this bit is set one or more Control keys were held down.
  */
#define OE_MOD_CONTROL         0x0002
  /*! @brief If this bit is set one or more Alt keys were held down.
   */
#define OE_MOD_ALT             0x0004
   /*! @brief If this bit is set one or more Super keys were held down.
	*/
#define OE_MOD_SUPER           0x0008

//Mouse buttons
#define OE_MOUSE_BUTTON_1         0
#define OE_MOUSE_BUTTON_2         1
#define OE_MOUSE_BUTTON_3         2
#define OE_MOUSE_BUTTON_4         3
#define OE_MOUSE_BUTTON_5         4
#define OE_MOUSE_BUTTON_6         5
#define OE_MOUSE_BUTTON_7         6
#define OE_MOUSE_BUTTON_8         7
#define OE_MOUSE_BUTTON_LAST      OE_MOUSE_BUTTON_8
#define OE_MOUSE_BUTTON_LEFT      OE_MOUSE_BUTTON_1
#define OE_MOUSE_BUTTON_RIGHT     OE_MOUSE_BUTTON_2
#define OE_MOUSE_BUTTON_MIDDLE    OE_MOUSE_BUTTON_3

//Joystick
#define OE_JOYSTICK_1             0
#define OE_JOYSTICK_2             1
#define OE_JOYSTICK_3             2
#define OE_JOYSTICK_4             3
#define OE_JOYSTICK_5             4
#define OE_JOYSTICK_6             5
#define OE_JOYSTICK_7             6
#define OE_JOYSTICK_8             7
#define OE_JOYSTICK_9             8
#define OE_JOYSTICK_10            9
#define OE_JOYSTICK_11            10
#define OE_JOYSTICK_12            11
#define OE_JOYSTICK_13            12
#define OE_JOYSTICK_14            13
#define OE_JOYSTICK_15            14
#define OE_JOYSTICK_16            15
#define OE_JOYSTICK_LAST          OE_JOYSTICK_16