#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "physics/physics.h"

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
    float baseY = screenH - 100.0f;
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
                    rocket_init(&rocket, screenW / 2.0f, screenH - 150.0f);
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

            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                rocket.engineOn = true;
            } else {
                rocket.engineOn = false;
            }

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                rocket_rotate(&rocket, -2.5f, dt);
            }
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                rocket_rotate(&rocket, 2.5f, dt);
            }

            rocket_apply_thrust(&rocket, dt);
            rocket_apply_gravity(&rocket, 200.0f, dt);
            rocket_update(&rocket, dt);

            if (rocket.body.position.y > baseY - 5) {
                rocket.body.position.y = baseY - 5;
                rocket.body.velocity.y = 0;
                if (rocket.body.velocity.x > -5 && rocket.body.velocity.x < 5) {
                    rocket.body.velocity.x *= 0.8f;
                }
            }
        }

        BeginDrawing();
            ClearBackground((Color){ 2, 4, 15, 255 });

            for (int i = 0; i < 150; i++) {
                float size = stars[i].speed * 3.0f;
                DrawCircle(stars[i].x, stars[i].y, size,
                          (Color){ 200, 220, 255, (unsigned char)stars[i].brightness });
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
                for (int y = 0; y < screenH; y += 40) {
                    DrawLine(0, y, screenW, y, (Color){ 0, 80, 120, 20 });
                }
                for (int x = 0; x < screenW; x += 40) {
                    DrawLine(x, 0, x, screenH, (Color){ 0, 80, 120, 20 });
                }

                DrawLineEx((Vector2){ 0, baseY }, (Vector2){ screenW, baseY }, 5.0f,
                          (Color){ 0, 255, 200, 255 });
                DrawLineEx((Vector2){ 0, baseY + 2 }, (Vector2){ screenW, baseY + 2 }, 3.0f,
                          (Color){ 0, 255, 200, 100 });

                float baseWidth = 100.0f;
                float baseCenterX = screenW / 2.0f;
                Rectangle launchBase = {
                    baseCenterX - baseWidth/2,
                    baseY - 15.0f,
                    baseWidth,
                    15.0f
                };

                DrawRectangleRec(
                    (Rectangle){ launchBase.x - 2, launchBase.y - 2, launchBase.width + 4, launchBase.height + 4 },
                    (Color){ 255, 150, 0, (unsigned char)(100 + 100 * baseGlow) });
                DrawRectangleRec(launchBase, (Color){ 40, 40, 50, 255 });
                DrawRectangleLinesEx(launchBase, 2, (Color){ 255, 150, 0, 255 });

                float rocketSize = 40.0f;
                Vec2 p1 = { rocket.body.position.x, rocket.body.position.y - rocketSize };
                Vec2 p2 = { rocket.body.position.x - rocketSize/2, rocket.body.position.y };
                Vec2 p3 = { rocket.body.position.x + rocketSize/2, rocket.body.position.y };

                Vec2 center = { rocket.body.position.x, rocket.body.position.y - rocketSize/3 };
                p1 = vec2_add(vec2_rotate(vec2_sub(p1, center), rocket.body.rotation), center);
                p2 = vec2_add(vec2_rotate(vec2_sub(p2, center), rocket.body.rotation), center);
                p3 = vec2_add(vec2_rotate(vec2_sub(p3, center), rocket.body.rotation), center);

                DrawTriangle((Vector2){ p1.x, p1.y }, (Vector2){ p3.x, p3.y }, (Vector2){ p2.x, p2.y }, (Color){ 255, 140, 0, 255 });

                for (int i = 1; i <= 3; i++) {
                    DrawLineEx((Vector2){ p1.x, p1.y }, (Vector2){ p2.x, p2.y }, 3.0f - i * 0.5f, (Color){ 255, 200, 0, (unsigned char)(200 - i * 40) });
                    DrawLineEx((Vector2){ p2.x, p2.y }, (Vector2){ p3.x, p3.y }, 3.0f - i * 0.5f, (Color){ 255, 200, 0, (unsigned char)(200 - i * 40) });
                    DrawLineEx((Vector2){ p3.x, p3.y }, (Vector2){ p1.x, p1.y }, 3.0f - i * 0.5f, (Color){ 255, 200, 0, (unsigned char)(200 - i * 40) });
                }

                DrawLineEx((Vector2){ p1.x, p1.y }, (Vector2){ p2.x, p2.y }, 3.0f, (Color){ 255, 200, 0, 255 });
                DrawLineEx((Vector2){ p2.x, p2.y }, (Vector2){ p3.x, p3.y }, 3.0f, (Color){ 255, 200, 0, 255 });
                DrawLineEx((Vector2){ p3.x, p3.y }, (Vector2){ p1.x, p1.y }, 3.0f, (Color){ 255, 200, 0, 255 });

                if (rocket.engineOn && rocket_has_fuel(&rocket)) {
                    Vec2 exhaustDir = (Vec2){ sinf(rocket.body.rotation), cosf(rocket.body.rotation) };
                    float exhaustLength = 20.0f + (rand() % 10);
                    Vector2 exhaustStart = { rocket.body.position.x, rocket.body.position.y };
                    Vector2 exhaustEnd = {
                        exhaustStart.x + exhaustDir.x * exhaustLength,
                        exhaustStart.y + exhaustDir.y * exhaustLength
                    };

                    DrawCircle(exhaustEnd.x, exhaustEnd.y, 8, (Color){ 255, 150, 0, 150 });
                    DrawCircle(exhaustEnd.x, exhaustEnd.y, 5, (Color){ 255, 200, 100, 200 });
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
                sprintf(altText, "ALT: %.0fm", baseY - rocket.body.position.y);
                DrawText(altText, 30, screenH - 60, 20, (Color){ 0, 255, 200, 255 });

                char velText[32];
                float speed = vec2_magnitude(rocket.body.velocity);
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
