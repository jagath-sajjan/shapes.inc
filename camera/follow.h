#ifndef FOLLOW_H
#define FOLLOW_H

typedef struct {
    float x;
    float y;
    float zoom;
    float smoothness;
} GameCamera;

void camera_init(GameCamera* cam, float x, float y);
void camera_follow(GameCamera* cam, float targetX, float targetY, float dt);
void camera_update_zoom(GameCamera* cam, float altitude, float dt);

#endif
