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
    Vec2 acceleration;
    float rotation;
    float angularVelocity;
    float mass;
} RigidBody;

typedef struct {
    RigidBody body;
    float fuel;
    float maxFuel;
    float thrustPower;
    float fuelConsumption;
    bool engineOn;
} Rocket;

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mult(Vec2 v, float scalar);
Vec2 vec2_div(Vec2 v, float scalar);
float vec2_magnitude(Vec2 v);
Vec2 vec2_normalize(Vec2 v);
float vec2_distance(Vec2 a, Vec2 b);
Vec2 vec2_rotate(Vec2 v, float angle);

void rocket_init(Rocket* rocket, float x, float y);
void rocket_apply_thrust(Rocket* rocket, float dt);
void rocket_apply_gravity(Rocket* rocket, float gravity, float dt);
void rocket_rotate(Rocket* rocket, float rotationSpeed, float dt);
void rocket_update(Rocket* rocket, float dt);
bool rocket_has_fuel(Rocket* rocket);

#endif
