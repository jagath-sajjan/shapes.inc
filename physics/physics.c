#include "physics.h"

void rocket_init(Rocket* rocket, float x, float y) {
    rocket->body.position.x = x;
    rocket->body.position.y = y;
    rocket->body.velocity.x = 0.0f;
    rocket->body.velocity.y = 0.0f;
    rocket->body.rotation = 0.0f;
    rocket->body.angularVelocity = 0.0f;
    rocket->body.mass = 1.0f;

    rocket->fuel = 100.0f;
    rocket->maxFuel = 100.0f;
    rocket->thrust = 400.0f;
    rocket->fuelRate = 15.0f;
    rocket->throttle = false;
}

void rocket_update(Rocket* rocket, float dt, float gravity) {
    float accelX = 0.0f;
    float accelY = gravity;

    if (rocket->throttle && rocket->fuel > 0) {
        float thrustX = sinf(rocket->body.rotation) * rocket->thrust;
        float thrustY = -cosf(rocket->body.rotation) * rocket->thrust;

        accelX += thrustX;
        accelY += thrustY;

        rocket->fuel -= rocket->fuelRate * dt;
        if (rocket->fuel < 0) rocket->fuel = 0;
    }

    rocket->body.velocity.x += accelX * dt;
    rocket->body.velocity.y += accelY * dt;

    rocket->body.position.x += rocket->body.velocity.x * dt;
    rocket->body.position.y += rocket->body.velocity.y * dt;

    rocket->body.rotation += rocket->body.angularVelocity * dt;

    rocket->body.angularVelocity *= 0.85f;
}
