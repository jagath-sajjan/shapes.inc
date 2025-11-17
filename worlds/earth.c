#include "earth.h"
#include <stdlib.h>
#include <math.h>

void earth_init(Earth* earth, float screenW, float screenH) {
    earth->groundLevel = screenH - 100.0f;
    earth->gravity = 150.0f;

    earth->skyColor = (Color){ 15, 20, 35, 255 };
    earth->groundColor = (Color){ 20, 25, 30, 255 };

    earth->numClouds = 15;
    for (int i = 0; i < earth->numClouds; i++) {
        earth->clouds[i].x = rand() % (int)screenW;
        earth->clouds[i].y = 100 + rand() % 250;
        earth->clouds[i].speed = 5.0f + (rand() % 15);
        earth->clouds[i].size = 20.0f + (rand() % 25);
    }
}

void earth_update(Earth* earth, float altitude, float dt) {
    for (int i = 0; i < earth->numClouds; i++) {
        earth->clouds[i].x += earth->clouds[i].speed * dt;
        if (earth->clouds[i].x > 1400) {
            earth->clouds[i].x = -100;
            earth->clouds[i].y = 100 + rand() % 250;
        }
    }

    earth->skyColor = earth_get_atmosphere_color(altitude);
}

void earth_draw(Earth* earth, float screenW, float screenH, float cameraY, float groundLevel) {
    DrawRectangle(0, 0, screenW, screenH, earth->skyColor);

    float altitude = groundLevel - cameraY;

    if (altitude < 600) {
        for (int i = 0; i < earth->numClouds; i++) {
            Cloud* c = &earth->clouds[i];
            float worldCloudY = c->y;
            float screenCloudY = worldCloudY - cameraY + screenH / 2;

            if (worldCloudY < cameraY + 300 && screenCloudY > -50 && screenCloudY < screenH + 50) {
                int alpha = 80;
                if (altitude > 450) {
                    alpha = (int)(80 * (1.0f - (altitude - 450) / 150.0f));
                }

                DrawCircle(c->x, screenCloudY, c->size, (Color){ 60, 70, 90, alpha });
                DrawCircle(c->x + c->size * 0.5f, screenCloudY, c->size * 0.7f, (Color){ 60, 70, 90, alpha });
                DrawCircle(c->x - c->size * 0.5f, screenCloudY, c->size * 0.7f, (Color){ 60, 70, 90, alpha });
            }
        }
    }

    float worldGroundY = groundLevel;
    float screenGroundY = worldGroundY - cameraY + screenH / 2;

    if (screenGroundY > -100 && screenGroundY < screenH + 100) {
        DrawRectangle(0, screenGroundY, screenW, screenH - screenGroundY + 200, earth->groundColor);

        DrawLineEx((Vector2){ 0, screenGroundY }, (Vector2){ screenW, screenGroundY }, 3.0f, (Color){ 0, 200, 160, 255 });

        if (altitude < 200) {
            for (int i = -50; i < screenW + 50; i += 80) {
                int treeAlpha = 200;
                if (altitude > 100) {
                    treeAlpha = (int)(200 * (1.0f - (altitude - 100) / 100.0f));
                }
                DrawRectangle(i, screenGroundY + 5, 8, 20, (Color){ 40, 45, 50, treeAlpha });
                DrawCircle(i + 4, screenGroundY + 3, 12, (Color){ 30, 40, 45, treeAlpha });
            }
        }
    }
}

Color earth_get_atmosphere_color(float altitude) {
    if (altitude < 0) altitude = 0;

    Color lowAtmo = (Color){ 15, 20, 35, 255 };
    Color midAtmo = (Color){ 10, 15, 30, 255 };
    Color highAtmo = (Color){ 5, 8, 20, 255 };
    Color space = (Color){ 2, 4, 15, 255 };

    if (altitude < 400) {
        float t = altitude / 400.0f;
        return (Color){
            (unsigned char)(lowAtmo.r * (1-t) + midAtmo.r * t),
            (unsigned char)(lowAtmo.g * (1-t) + midAtmo.g * t),
            (unsigned char)(lowAtmo.b * (1-t) + midAtmo.b * t),
            255
        };
    } else if (altitude < 1000) {
        float t = (altitude - 400) / 600.0f;
        return (Color){
            (unsigned char)(midAtmo.r * (1-t) + highAtmo.r * t),
            (unsigned char)(midAtmo.g * (1-t) + highAtmo.g * t),
            (unsigned char)(midAtmo.b * (1-t) + highAtmo.b * t),
            255
        };
    } else if (altitude < 2000) {
        float t = (altitude - 1000) / 1000.0f;
        return (Color){
            (unsigned char)(highAtmo.r * (1-t) + space.r * t),
            (unsigned char)(highAtmo.g * (1-t) + space.g * t),
            (unsigned char)(highAtmo.b * (1-t) + space.b * t),
            255
        };
    }

    return space;
}
