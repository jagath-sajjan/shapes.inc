#ifndef BUILD_H
#define BUILD_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    bool showSettingsMenu;
    Rectangle launchButton;
    Rectangle settingsButton;
    Rectangle homeButton;
    Rectangle backToBuildButton;
    Rectangle backButton;
    Rectangle settingsMenuCard;
} BuildScreen;

void build_init(BuildScreen* build, float screenW, float screenH);
void build_update(BuildScreen* build, Vector2 mouse, bool* shouldLaunch, bool* shouldGoHome, bool* shouldGoBack);
void build_draw(BuildScreen* build, float screenW, float screenH);
void build_draw_settings(BuildScreen* build, Vector2 mouse, bool* shouldGoHome, bool* shouldGoBackToBuild);

#endif
