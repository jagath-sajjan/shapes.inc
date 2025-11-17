#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    Vec2 position;
    Vec2 velocity;
    float rotation;
    float angularVelocity;
    float mass;
} RigidBody;

typedef struct {
    RigidBody body;
    float fuel;
    float maxFuel;
    float thrust;
    float fuelRate;
    bool throttle;
} Rocket;

void rocket_init(Rocket* rocket, float x, float y);
void rocket_update(Rocket* rocket, float dt, float gravity);

#endif
