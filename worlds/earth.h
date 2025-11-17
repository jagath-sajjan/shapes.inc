#ifndef EARTH_H
#define EARTH_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float speed;
    float size;
} Cloud;

typedef struct {
    float groundLevel;
    float gravity;
    Cloud clouds[20];
    int numClouds;

    Color skyColor;
    Color groundColor;
} Earth;

void earth_init(Earth* earth, float screenW, float screenH);
void earth_update(Earth* earth, float altitude, float dt);
void earth_draw(Earth* earth, float screenW, float screenH, float cameraY, float groundLevel);
Color earth_get_atmosphere_color(float altitude);

#endif
