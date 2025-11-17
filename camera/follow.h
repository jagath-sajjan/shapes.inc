#ifndef FOLLOW_H
#define FOLLOW_H

typedef struct {
    float x;
    float y;
    float zoom;
    float smoothness;
} Camera;

void camera_init(Camera* cam, float x, float y);
void camera_follow(Camera* cam, float targetX, float targetY, float dt);
void camera_update_zoom(Camera* cam, float altitude, float dt);

#endif
