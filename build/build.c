#include "build.h"
#include <math.h>

void build_init(BuildScreen* build, float screenW, float screenH) {
    build->showSettingsMenu = false;

    build->launchButton = (Rectangle){
        screenW - 170.0f,
        20.0f,
        150.0f,
        50.0f
    };

    build->settingsButton = (Rectangle){
        20.0f,
        20.0f,
        150.0f,
        50.0f
    };

    build->backButton = (Rectangle){
        20.0f,
        20.0f,
        120.0f,
        50.0f
    };

    build->settingsMenuCard = (Rectangle){
        30.0f,
        80.0f,
        220.0f,
        140.0f
    };

    build->homeButton = (Rectangle){
        40.0f,
        90.0f,
        200.0f,
        50.0f
    };

    build->backToBuildButton = (Rectangle){
        40.0f,
        150.0f,
        200.0f,
        50.0f
    };
}

void build_update(BuildScreen* build, Vector2 mouse, bool* shouldLaunch, bool* shouldGoHome, bool* shouldGoBack) {
    *shouldLaunch = false;
    *shouldGoHome = false;
    *shouldGoBack = false;

    bool hoveringLaunch = CheckCollisionPointRec(mouse, build->launchButton);
    bool clickedLaunch = hoveringLaunch && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    bool hoveringBack = CheckCollisionPointRec(mouse, build->backButton);
    bool clickedBack = hoveringBack && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (clickedLaunch) {
        *shouldLaunch = true;
    }

    if (clickedBack) {
        *shouldGoBack = true;
    }
}

void build_draw(BuildScreen* build, float screenW, float screenH) {
    DrawText("BUILD YOUR ROCKET", screenW / 2 - MeasureText("BUILD YOUR ROCKET", 40) / 2, 30, 40, (Color){ 0, 255, 200, 255 });

    DrawText("[Rocket parts will go here]", screenW / 2 - MeasureText("[Rocket parts will go here]", 24) / 2, screenH / 2, 24, (Color){ 100, 120, 140, 255 });

    bool hoveringBack = CheckCollisionPointRec(GetMousePosition(), build->backButton);
    Color backBtnColor = hoveringBack ? (Color){ 0, 255, 200, 255 } : (Color){ 0, 200, 160, 255 };

    DrawRectangleRounded(
        (Rectangle){ build->backButton.x - 3, build->backButton.y - 3, build->backButton.width + 6, build->backButton.height + 6 },
        0.3f, 16, (Color){ 0, 255, 200, 60 });
    DrawRectangleRounded(build->backButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
    DrawRectangleRoundedLines(build->backButton, 0.3f, 16, backBtnColor);

    const char* backText = "← BACK";
    int backTextWidth = MeasureText(backText, 24);
    DrawText(backText,
             build->backButton.x + build->backButton.width / 2 - backTextWidth / 2,
             build->backButton.y + build->backButton.height / 2 - 12,
             24,
             backBtnColor);

    bool hoveringLaunch = CheckCollisionPointRec(GetMousePosition(), build->launchButton);
    Color launchBtnColor = hoveringLaunch ? (Color){ 0, 255, 200, 255 } : (Color){ 0, 200, 160, 255 };

    DrawRectangleRounded(
        (Rectangle){ build->launchButton.x - 3, build->launchButton.y - 3, build->launchButton.width + 6, build->launchButton.height + 6 },
        0.3f, 16, (Color){ 0, 255, 200, 60 });
    DrawRectangleRounded(build->launchButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
    DrawRectangleRoundedLines(build->launchButton, 0.3f, 16, launchBtnColor);

    const char* launchText = "LAUNCH";
    int launchTextWidth = MeasureText(launchText, 24);
    DrawText(launchText,
             build->launchButton.x + build->launchButton.width / 2 - launchTextWidth / 2,
             build->launchButton.y + build->launchButton.height / 2 - 12,
             24,
             launchBtnColor);
}

void build_draw_settings(BuildScreen* build, Vector2 mouse, bool* shouldGoHome, bool* shouldGoBackToBuild) {
    *shouldGoHome = false;
    *shouldGoBackToBuild = false;

    bool hoveringSettings = CheckCollisionPointRec(mouse, build->settingsButton);
    bool clickedSettings = hoveringSettings && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (clickedSettings) {
        build->showSettingsMenu = !build->showSettingsMenu;
    }

    Color settingsBtnColor = hoveringSettings ? (Color){ 0, 255, 200, 255 } : (Color){ 0, 200, 160, 255 };

    DrawRectangleRounded(
        (Rectangle){ build->settingsButton.x - 3, build->settingsButton.y - 3, build->settingsButton.width + 6, build->settingsButton.height + 6 },
        0.3f, 16, (Color){ 0, 255, 200, 60 });
    DrawRectangleRounded(build->settingsButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
    DrawRectangleRoundedLines(build->settingsButton, 0.3f, 16, settingsBtnColor);

    const char* settingsText = "SETTINGS";
    int settingsTextWidth = MeasureText(settingsText, 22);
    DrawText(settingsText,
             build->settingsButton.x + build->settingsButton.width / 2 - settingsTextWidth / 2,
             build->settingsButton.y + build->settingsButton.height / 2 - 11,
             22,
             settingsBtnColor);

    if (build->showSettingsMenu) {
        bool clickedOutside = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                             !CheckCollisionPointRec(mouse, build->settingsMenuCard);

        if (clickedOutside) {
            build->showSettingsMenu = false;
            return;
        }

        DrawRectangleRounded(build->settingsMenuCard, 0.2f, 16, (Color){ 10, 15, 25, 255 });
        DrawRectangleRoundedLines(build->settingsMenuCard, 0.2f, 16, (Color){ 0, 255, 200, 255 });

        bool hoveringHome = CheckCollisionPointRec(mouse, build->homeButton);
        Color homeBtnColor = hoveringHome ? (Color){ 0, 255, 200, 255 } : (Color){ 0, 200, 160, 255 };

        DrawRectangleRounded(
            (Rectangle){ build->homeButton.x - 2, build->homeButton.y - 2, build->homeButton.width + 4, build->homeButton.height + 4 },
            0.3f, 16, (Color){ 0, 255, 200, 60 });
        DrawRectangleRounded(build->homeButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
        DrawRectangleRoundedLines(build->homeButton, 0.3f, 16, homeBtnColor);

        const char* homeText = "HOME";
        int homeTextWidth = MeasureText(homeText, 20);
        DrawText(homeText,
                 build->homeButton.x + build->homeButton.width / 2 - homeTextWidth / 2,
                 build->homeButton.y + build->homeButton.height / 2 - 10,
                 20,
                 homeBtnColor);

        bool hoveringBackToBuild = CheckCollisionPointRec(mouse, build->backToBuildButton);
        Color backToBuildColor = hoveringBackToBuild ? (Color){ 0, 255, 200, 255 } : (Color){ 0, 200, 160, 255 };

        DrawRectangleRounded(
            (Rectangle){ build->backToBuildButton.x - 2, build->backToBuildButton.y - 2, build->backToBuildButton.width + 4, build->backToBuildButton.height + 4 },
            0.3f, 16, (Color){ 0, 255, 200, 60 });
        DrawRectangleRounded(build->backToBuildButton, 0.3f, 16, (Color){ 5, 10, 30, 255 });
        DrawRectangleRoundedLines(build->backToBuildButton, 0.3f, 16, backToBuildColor);

        const char* backText = "BACK TO BUILD";
        int backTextWidth = MeasureText(backText, 20);
        DrawText(backText,
                 build->backToBuildButton.x + build->backToBuildButton.width / 2 - backTextWidth / 2,
                 build->backToBuildButton.y + build->backToBuildButton.height / 2 - 10,
                 20,
                 backToBuildColor);

        bool clickedHome = hoveringHome && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool clickedBackToBuild = hoveringBackToBuild && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (clickedHome) {
            *shouldGoHome = true;
            build->showSettingsMenu = false;
        }

        if (clickedBackToBuild) {
            *shouldGoBackToBuild = true;
            build->showSettingsMenu = false;
        }
    }
}
