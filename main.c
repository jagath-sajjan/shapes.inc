#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "physics/physics.h"
#include "worlds/earth.h"
#include "camera/follow.h"

typedef enum GameScreen {
    SCREEN_MENU = 0,
    SCREEN_LOADING,
    SCREEN_SIM
} GameScreen;

typedef struct Star {
    float x;
    float y;
    float speed;
    float brightness;
} Star;

int main(void) {
    const int screenW = 1280;
    const int screenH = 720;
    InitWindow(screenW, screenH, "Shapes.Inc");
    SetTargetFPS(60);
    GameScreen currentScreen = SCREEN_MENU;

    Rectangle playButton = {
        screenW/2.0f - 150.0f,
        screenH/2.0f + 20.0f,
        300.0f,
        60.0f
    };

    Rectangle backButton = {
        20.0f,
        20.0f,
        120.0f,
        50.0f
    };

    float loadingProgress = 0.0f;
    int loadingDots = 0;
    float dotTimer = 0.0f;

    Rocket rocket;
    Earth earth;
    GameCamera camera;
    bool rocketInitialized = false;

    Star stars[150];
    for (int i = 0; i < 150; i++) {
        stars[i].x = GetRandomValue(0, screenW);
        stars[i].y = GetRandomValue(0, screenH);
        stars[i].speed = (float)GetRandomValue(10, 40) / 100.0f;
        stars[i].brightness = (float)GetRandomValue(100, 255);
    }

    float time = 0.0f;
    float rocketGlow = 0.0f;
    float baseGlow = 0.0f;

    while (!WindowShouldClose()) {
        time += GetFrameTime();
        rocketGlow = fabs(sinf(time * 2.0f)) * 0.5f + 0.5f;
        baseGlow = fabs(sinf(time * 1.5f)) * 0.3f + 0.3f;

        for (int i = 0; i < 150; i++) {
            stars[i].y += stars[i].speed;
            if (stars[i].y > screenH) {
                stars[i].y = 0;
                stars[i].x = GetRandomValue(0, screenW);
            }
        }

        Vector2 mouse = GetMousePosition();
        bool hoveringPlay = CheckCollisionPointRec(mouse, playButton);
        bool clickedPlay  = hoveringPlay && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        bool hoveringBack = CheckCollisionPointRec(mouse, backButton);
        bool clickedBack = hoveringBack && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (currentScreen == SCREEN_MENU) {
            if (clickedPlay) {
                currentScreen = SCREEN_LOADING;
                loadingProgress = 0.0f;
                loadingDots = 0;
                dotTimer = 0.0f;
            }
        }
        else if (currentScreen == SCREEN_LOADING) {
            loadingProgress += 0.01f;
            dotTimer += GetFrameTime();

            if (dotTimer >= 0.3f) {
                dotTimer = 0.0f;
                loadingDots = (loadingDots + 1) % 4;
            }

            if (loadingProgress >= 1.0f) {
                currentScreen = SCREEN_SIM;
                if (!rocketInitialized) {
                    earth_init(&earth, screenW, screenH);
                    rocket_init(&rocket, screenW / 2.0f, earth.groundLevel - 5.0f);
                    camera_init(&camera, rocket.body.position.x, rocket.body.position.y - 200.0f);
                    rocketInitialized = true;
                }
            }
        }
        else if (currentScreen == SCREEN_SIM) {
            if (clickedBack) {
                currentScreen = SCREEN_MENU;
                rocketInitialized = false;
            }

            float dt = GetFrameTime();

            rocket.throttle = (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP));

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                rocket.body.angularVelocity = -2.0f;
            }
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                rocket.body.angularVelocity = 2.0f;
            }

            rocket_update(&rocket, dt, earth.gravity);

            float altitude = earth.groundLevel - rocket.body.position.y;
            camera_follow(&camera, rocket.body.position.x, rocket.body.position.y, dt);
            camera_update_zoom(&camera, altitude, dt);
            earth_update(&earth, altitude, dt);

            if (rocket.body.position.y >= earth.groundLevel - 5) {
                rocket.body.position.y = earth.groundLevel - 5;
                rocket.body.velocity.y = 0;
                rocket.body.velocity.x *= 0.7f;
            }
        }

        BeginDrawing();
            if (currentScreen == SCREEN_SIM) {
                earth_draw(&earth, screenW, screenH, camera.y, earth.groundLevel);

                float altitude = earth.groundLevel - rocket.body.position.y;
                if (altitude > 800) {
                    for (int i = 0; i < 150; i++) {
                        float starAlpha = 0;
                        if (altitude > 800 && altitude < 1500) {
                            starAlpha = (altitude - 800) / 700.0f;
                        } else if (altitude >= 1500) {
                            starAlpha = 1.0f;
                        }

                        float size = stars[i].speed * 3.0f;
                        DrawCircle(stars[i].x, stars[i].y, size,
                                  (Color){ 200, 220, 255, (unsigned char)(stars[i].brightness * starAlpha) });
                    }
                }
            } else {
                ClearBackground((Color){ 2, 4, 15, 255 });

                for (int i = 0; i < 150; i++) {
                    float size = stars[i].speed * 3.0f;
                    DrawCircle(stars[i].x, stars[i].y, size,
                              (Color){ 200, 220, 255, (unsigned char)stars[i].brightness });
                }
            }

            if (currentScreen == SCREEN_MENU) {
                const char *title = "SHAPES.INC ROCKET LAB";
                int titleFontSize = 60;
                int titleWidth = MeasureText(title, titleFontSize);

                DrawText(title,
                         screenW/2 - titleWidth/2 + 3,
                         screenH/2 - 150 + 3,
                         titleFontSize,
                         (Color){ 0, 255, 200, 80 });
                DrawText(title,
                         screenW/2 - titleWidth/2,
                         screenH/2 - 150,
                         titleFontSize,
                         (Color){ 0, 255, 200, 255 });

                const char *subtitle = "retro 2d rocket sandbox • open source vibes";
                int subFontSize = 22;
                int subWidth = MeasureText(subtitle, subFontSize);
                DrawText(subtitle,
                         screenW/2 - subWidth/2,
                         screenH/2 - 90,
                         subFontSize,
                         (Color){ 255, 100, 200, 255 });

                Color btnColor = hoveringPlay ?
                    (Color){ 0, 255, 200, 255 } :
                    (Color){ 0, 200, 160, 255 };
                Color btnGlow = hoveringPlay ?
                    (Color){ 0, 255, 200, 120 } :
                    (Color){ 0, 255, 200, 60 };

                DrawRectangleRounded(
                    (Rectangle){ playButton.x - 4, playButton.y - 4, playButton.width + 8, playButton.height + 8 },
                    0.3f, 16, btnGlow);
                DrawRectangleRounded(playButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
                DrawRectangleRoundedLines(playButton, 0.3f, 16, btnColor);

                const char *btnText = ">> START SIM <<";
                int btnFontSize = 26;
                int btnTextWidth = MeasureText(btnText, btnFontSize);
                DrawText(btnText,
                         playButton.x + playButton.width/2 - btnTextWidth/2,
                         playButton.y + playButton.height/2 - btnFontSize/2,
                         btnFontSize,
                         btnColor);
            }
            else if (currentScreen == SCREEN_LOADING) {
                const char *loadingText = ">>> INITIALIZING SIMULATION";
                int loadFontSize = 32;
                int loadWidth = MeasureText(loadingText, loadFontSize);

                Color loadColor = (Color){
                    0,
                    (unsigned char)(200 + 55 * sinf(time * 3)),
                    (unsigned char)(160 + 95 * sinf(time * 3)),
                    255
                };

                DrawText(loadingText,
                         screenW/2 - loadWidth/2,
                         screenH/2 - 80,
                         loadFontSize,
                         loadColor);

                char dotsStr[5] = "";
                for (int i = 0; i < loadingDots; i++) {
                    dotsStr[i] = '.';
                }
                dotsStr[loadingDots] = '\0';

                DrawText(dotsStr,
                         screenW/2 - loadWidth/2 + MeasureText(loadingText, loadFontSize) + 10,
                         screenH/2 - 80,
                         loadFontSize,
                         loadColor);

                Rectangle barBg = {
                    screenW/2.0f - 250.0f,
                    screenH/2.0f,
                    500.0f,
                    35.0f
                };

                DrawRectangleRounded(barBg, 0.5f, 16, (Color){ 10, 20, 40, 255 });

                Rectangle barFill = {
                    screenW/2.0f - 250.0f + 3,
                    screenH/2.0f + 3,
                    (500.0f - 6) * loadingProgress,
                    29.0f
                };

                Color barColor1 = (Color){ 0, 255, 200, 255 };
                Color barColor2 = (Color){ 255, 0, 150, 255 };

                for (int i = 0; i < barFill.width; i += 2) {
                    float t = (float)i / barFill.width;
                    Color mixColor = (Color){
                        (unsigned char)(barColor1.r * (1-t) + barColor2.r * t),
                        (unsigned char)(barColor1.g * (1-t) + barColor2.g * t),
                        (unsigned char)(barColor1.b * (1-t) + barColor2.b * t),
                        255
                    };
                    DrawRectangle(barFill.x + i, barFill.y, 2, barFill.height, mixColor);
                }

                DrawRectangleRoundedLines(barBg, 0.5f, 16, (Color){ 0, 255, 200, 200 });

                char percentText[8];
                sprintf(percentText, "%d%%", (int)(loadingProgress * 100));
                int percentSize = 24;
                int percentWidth = MeasureText(percentText, percentSize);
                DrawText(percentText,
                         screenW/2 - percentWidth/2,
                         screenH/2 + 50,
                         percentSize,
                         (Color){ 0, 255, 200, 255 });
            }
            else if (currentScreen == SCREEN_SIM) {
                float altitude = earth.groundLevel - rocket.body.position.y;

                float baseWidth = 100.0f;
                float baseCenterX = screenW / 2.0f;
                float launchPadWorldY = earth.groundLevel;
                float launchPadScreenY = launchPadWorldY - camera.y + screenH / 2;

                if (launchPadScreenY > -50 && launchPadScreenY < screenH + 50) {
                    Rectangle drawLaunchBase = {
                        baseCenterX - baseWidth/2,
                        launchPadScreenY - 15.0f,
                        baseWidth,
                        15.0f
                    };

                    DrawRectangleRec(
                        (Rectangle){ drawLaunchBase.x - 2, drawLaunchBase.y - 2, drawLaunchBase.width + 4, drawLaunchBase.height + 4 },
                        (Color){ 255, 150, 0, (unsigned char)(80 + 80 * baseGlow) });
                    DrawRectangleRec(drawLaunchBase, (Color){ 30, 35, 40, 255 });
                    DrawRectangleLinesEx(drawLaunchBase, 2, (Color){ 255, 140, 0, 255 });
                }

                float rocketSize = 40.0f;

                float rocketScreenX = rocket.body.position.x - camera.x + screenW / 2;
                float rocketScreenY = rocket.body.position.y - camera.y + screenH / 2;

                float cosR = cosf(rocket.body.rotation);
                float sinR = sinf(rocket.body.rotation);

                float tipX = 0;
                float tipY = -rocketSize;
                float leftX = -rocketSize/2;
                float leftY = 0;
                float rightX = rocketSize/2;
                float rightY = 0;

                Vec2 p1 = {
                    rocketScreenX + (tipX * cosR - tipY * sinR),
                    rocketScreenY + (tipX * sinR + tipY * cosR)
                };
                Vec2 p2 = {
                    rocketScreenX + (leftX * cosR - leftY * sinR),
                    rocketScreenY + (leftX * sinR + leftY * cosR)
                };
                Vec2 p3 = {
                    rocketScreenX + (rightX * cosR - rightY * sinR),
                    rocketScreenY + (rightX * sinR + rightY * cosR)
                };

                DrawTriangle((Vector2){ p1.x, p1.y }, (Vector2){ p3.x, p3.y }, (Vector2){ p2.x, p2.y }, (Color){ 255, 140, 0, 255 });

                for (int i = 1; i <= 3; i++) {
                    DrawLineEx((Vector2){ p1.x, p1.y }, (Vector2){ p2.x, p2.y }, 3.0f - i * 0.5f, (Color){ 255, 200, 0, (unsigned char)(200 - i * 40) });
                    DrawLineEx((Vector2){ p2.x, p2.y }, (Vector2){ p3.x, p3.y }, 3.0f - i * 0.5f, (Color){ 255, 200, 0, (unsigned char)(200 - i * 40) });
                    DrawLineEx((Vector2){ p3.x, p3.y }, (Vector2){ p1.x, p1.y }, 3.0f - i * 0.5f, (Color){ 255, 200, 0, (unsigned char)(200 - i * 40) });
                }

                DrawLineEx((Vector2){ p1.x, p1.y }, (Vector2){ p2.x, p2.y }, 3.0f, (Color){ 255, 200, 0, 255 });
                DrawLineEx((Vector2){ p2.x, p2.y }, (Vector2){ p3.x, p3.y }, 3.0f, (Color){ 255, 200, 0, 255 });
                DrawLineEx((Vector2){ p3.x, p3.y }, (Vector2){ p1.x, p1.y }, 3.0f, (Color){ 255, 200, 0, 255 });

                if (rocket.throttle && rocket.fuel > 0) {
                    float exhaustAngle = rocket.body.rotation;
                    float exhaustDist = 25.0f + (rand() % 15);
                    Vector2 exhaustPos = {
                        rocketScreenX + sinf(exhaustAngle) * exhaustDist,
                        rocketScreenY + cosf(exhaustAngle) * exhaustDist
                    };

                    DrawCircle(exhaustPos.x, exhaustPos.y, 10, (Color){ 255, 150, 0, 120 });
                    DrawCircle(exhaustPos.x, exhaustPos.y, 6, (Color){ 255, 220, 100, 180 });
                }

                Color backBtnColor = hoveringBack ?
                    (Color){ 0, 255, 200, 255 } :
                    (Color){ 0, 200, 160, 255 };
                Color backGlow = hoveringBack ?
                    (Color){ 0, 255, 200, 120 } :
                    (Color){ 0, 255, 200, 60 };

                DrawRectangleRounded(
                    (Rectangle){ backButton.x - 3, backButton.y - 3, backButton.width + 6, backButton.height + 6 },
                    0.3f, 16, backGlow);
                DrawRectangleRounded(backButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
                DrawRectangleRoundedLines(backButton, 0.3f, 16, backBtnColor);

                const char *backText = "← BACK";
                int backFontSize = 24;
                int backTextWidth = MeasureText(backText, backFontSize);
                DrawText(backText,
                         backButton.x + backButton.width/2 - backTextWidth/2,
                         backButton.y + backButton.height/2 - backFontSize/2,
                         backFontSize,
                         backBtnColor);

                const char *statusText = "ROCKET STATUS: READY";
                DrawText(statusText, screenW - MeasureText(statusText, 20) - 30, 30, 20,
                        (Color){ 0, 255, 200, 255 });

                char fuelText[32];
                sprintf(fuelText, "FUEL: %.0f%%", (rocket.fuel / rocket.maxFuel) * 100);
                DrawText(fuelText, screenW - MeasureText(fuelText, 20) - 30, 55, 20,
                        rocket.fuel > 20 ? (Color){ 0, 255, 200, 255 } : (Color){ 255, 100, 100, 255 });

                char altText[32];
                sprintf(altText, "ALT: %.0fm", earth.groundLevel - rocket.body.position.y);
                DrawText(altText, 30, screenH - 60, 20, (Color){ 0, 255, 200, 255 });

                char velText[32];
                float speed = sqrtf(rocket.body.velocity.x * rocket.body.velocity.x +
                                   rocket.body.velocity.y * rocket.body.velocity.y);
                sprintf(velText, "VEL: %.1fm/s", speed);
                DrawText(velText, 30, screenH - 35, 20, (Color){ 0, 255, 200, 255 });

                DrawText("CONTROLS: W/↑ = THRUST  A/D or ←/→ = ROTATE",
                        screenW/2 - MeasureText("CONTROLS: W/↑ = THRUST  A/D or ←/→ = ROTATE", 18)/2,
                        screenH - 30, 18, (Color){ 180, 200, 255, 150 });
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
