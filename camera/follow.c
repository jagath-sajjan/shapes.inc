#include "follow.h"

void camera_init(GameCamera* cam, float x, float y) {
    cam->x = x;
    cam->y = y;
    cam->zoom = 1.0f;
    cam->smoothness = 5.0f;
}

void camera_follow(GameCamera* cam, float targetX, float targetY, float dt) {
    cam->x += (targetX - cam->x) * cam->smoothness * dt;
    cam->y += (targetY - cam->y) * cam->smoothness * dt;
}

void camera_update_zoom(GameCamera* cam, float altitude, float dt) {
    float targetZoom = 1.0f;

    if (altitude > 300) {
        targetZoom = 1.0f - (altitude - 300) / 3000.0f;
        if (targetZoom < 0.4f) targetZoom = 0.4f;
    }

    cam->zoom += (targetZoom - cam->zoom) * 2.0f * dt;
}
