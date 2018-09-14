#version 400 core

in vec3 fragPosition;

uniform vec4 color;
uniform vec2 offset;
uniform float zoom;

uniform float constant;
uniform float exp;

int mandelbrot(float x, float y, int maximum) {
    int iteration = 0;
  
    float zx = x; 
	float zy = y; 
	float xtemp;

    while (zx*zx + zy*zy <= 4.0 && iteration <= maximum) {
		//float xtmp = pow((zx*zx + zy*zy), (exp / 2)) * cos(exp * atan(zy, zx)) + constant;
	    //zy = pow((zx*zx + zy*zy), (exp / 2)) * sin(exp * atan(zy, zx)) + constant;
	    //zx=xtmp;
		//
		//float xtmp = pow((zx*zx + zy*zy), (exp / 2)) * cos(exp * atan(zy, zx)) + x;
	    //zy = pow((zx*zx + zy*zy), (exp / 2)) * sin(exp * atan(zy, zx)) + y;
	    //zx=xtmp;

		//xtemp = zx*zx - zy*zy;
        //zy = 2*zx*zy + constant; 
        //zx = xtemp + constant;

		xtemp = zx*zx - zy*zy;
        zy = 2*zx*zy + y; 
        zx = xtemp + x;

        if (zx == y && zy == y) {
            return -1;
        }

        iteration += 1;
    }

    if (iteration >= maximum) {
        return -1;
    } else {
        return iteration;
    }
	return 1;
}

vec3 getColor(int iterations) {
    int r, g, b;

    if (iterations == -1) {
        r = 0;
        g = 0;
        b = 0;
    } else if (iterations == 0) {
        r = 255;
        g = 0;
        b = 0;
    } else {
        // colour gradient:      Red -> Blue -> Green -> Red -> Black
        // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
        if (iterations < 16) {
            r = 16 * (16 - iterations);
            g = 0;
            b = 16 * iterations - 1;
        } else if (iterations < 32) {
            r = 0;
            g = 16 * (iterations - 16);
            b = 16 * (32 - iterations) - 1;
        } else if (iterations < 64) {
            r = 8 * (iterations - 32);
            g = 8 * (64 - iterations) - 1;
            b = 0;
        } else { // range is 64 - 127
            r = 255 - (iterations - 64) * 4;
            g = 0;
            b = 0;
        }
    }

    return vec3(b/255.0, g/255.0, r/255.0);
}

void main()
{
	int maximum = 127;
    int counter = 0;
	float startReal = fragPosition.x * zoom + offset.x;
	float startImag = fragPosition.y * zoom + offset.y;
	
	int value = mandelbrot(startReal, startImag, maximum);
	gl_FragColor = vec4(getColor(value), 1.0);
}