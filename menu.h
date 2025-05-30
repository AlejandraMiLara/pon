// menu.c
#include "raylib.h"
#include "menu.h"

// Referencia a la función para cambiar de estado:
extern EstadoPantalla JuegoState;
extern EstadoPantalla *estadoActual;

EstadoPantalla MenuState = {
    .Update = UpdateMenu,
    .Draw = DrawMenu
};

void UpdateMenu(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        estadoActual = &JuegoState;
    }
}

void DrawMenu(void) {
    ClearBackground(BLACK);
    DrawText("PONG EVOLUTION", 300, 200, 40, WHITE);
    DrawText("Presiona ENTER para jugar", 280, 300, 20, GRAY);
}
