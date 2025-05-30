// juego.c
#include "raylib.h"
#include "juego.h"

// Referencia al estado actual para redirigir, si lo deseas:
extern EstadoPantalla MenuState;
extern EstadoPantalla *estadoActual;

static int x = 400, y = 300; // Demo: bola que se mueve

EstadoPantalla JuegoState = {
    .Update = UpdateJuego,
    .Draw = DrawJuego
};

void UpdateJuego(void) {
    if (IsKeyDown(KEY_RIGHT)) x += 5;
    if (IsKeyDown(KEY_LEFT)) x -= 5;

    if (IsKeyPressed(KEY_BACKSPACE)) {
        estadoActual = &MenuState;
    }
}

void DrawJuego(void) {
    ClearBackground(DARKGRAY);
    DrawText("Juego en curso", 20, 20, 20, WHITE);
    DrawCircle(x, y, 20, RED);
    DrawText("Presiona BACKSPACE para volver al menú", 20, 460, 20, GRAY);
}
