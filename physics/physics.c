#include "physics.h"
#include <math.h>

Vec2 vec2_add(Vec2 a, Vec2 b) {
    return (Vec2){ a.x + b.x, a.y + b.y };
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return (Vec2){ a.x - b.x, a.y - b.y };
}

Vec2 vec2_mult(Vec2 v, float scalar) {
    return (Vec2){ v.x * scalar, v.y * scalar };
}

Vec2 vec2_div(Vec2 v, float scalar) {
    return (Vec2){ v.x / scalar, v.y / scalar };
}

float vec2_magnitude(Vec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vec2 vec2_normalize(Vec2 v) {
    float mag = vec2_magnitude(v);
    if (mag == 0) return (Vec2){ 0, 0 };
    return vec2_div(v, mag);
}

float vec2_distance(Vec2 a, Vec2 b) {
    return vec2_magnitude(vec2_sub(b, a));
}

Vec2 vec2_rotate(Vec2 v, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    return (Vec2){
        v.x * cosA - v.y * sinA,
        v.x * sinA + v.y * cosA
    };
}

void rocket_init(Rocket* rocket, float x, float y) {
    rocket->body.position = (Vec2){ x, y };
    rocket->body.velocity = (Vec2){ 0, 0 };
    rocket->body.acceleration = (Vec2){ 0, 0 };
    rocket->body.rotation = 0.0f;
    rocket->body.angularVelocity = 0.0f;
    rocket->body.mass = 50.0f;

    rocket->fuel = 100.0f;
    rocket->maxFuel = 100.0f;
    rocket->thrustPower = 2500.0f;
    rocket->fuelConsumption = 8.0f;
    rocket->engineOn = false;
}

void rocket_apply_thrust(Rocket* rocket, float dt) {
    if (!rocket->engineOn || rocket->fuel <= 0) {
        return;
    }

    float thrustForce = rocket->thrustPower;

    rocket->body.acceleration.x += sinf(rocket->body.rotation) * thrustForce / rocket->body.mass;
    rocket->body.acceleration.y += -cosf(rocket->body.rotation) * thrustForce / rocket->body.mass;

    rocket->fuel -= rocket->fuelConsumption * dt;
    if (rocket->fuel < 0) rocket->fuel = 0;
}

void rocket_apply_gravity(Rocket* rocket, float gravity, float dt) {
    rocket->body.acceleration.y += gravity;
}

void rocket_rotate(Rocket* rocket, float rotationSpeed, float dt) {
    rocket->body.angularVelocity = rotationSpeed;
}

void rocket_update(Rocket* rocket, float dt) {
    rocket->body.velocity.x += rocket->body.acceleration.x * dt;
    rocket->body.velocity.y += rocket->body.acceleration.y * dt;

    float drag = 0.995f;
    rocket->body.velocity.x *= drag;
    rocket->body.velocity.y *= drag;

    rocket->body.position.x += rocket->body.velocity.x * dt;
    rocket->body.position.y += rocket->body.velocity.y * dt;

    rocket->body.rotation += rocket->body.angularVelocity * dt;

    rocket->body.acceleration.x = 0;
    rocket->body.acceleration.y = 0;
    rocket->body.angularVelocity *= 0.92f;
}

bool rocket_has_fuel(Rocket* rocket) {
    return rocket->fuel > 0;
}
