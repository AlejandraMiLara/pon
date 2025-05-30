#include "raylib.h"
#include "estado.h"
#include "menu.h"
#include "juego.h"

EstadoPantalla *estadoActual;

typedef enum GameScreen {
     MENU, 
     MULTIPLAYER, 
     SINGLEPLAYER,
     GAME_MULTI,
     GAME_SINGLE,
     ENDGAME,
     CREDITS 
} GameScreen;


int main(void) {
    estadoActual = &MenuState;

    const int screenWidth = 800;
    const int screenHeight = 450;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "PONG 646");
    InitAudioDevice();
    GameScreen currentScreen = MENU;

    // Variables LOAD
    float timer = 0.0f;

    // Variables MENU
    Texture2D normal_buttons = LoadTexture("resources/img/menu/normal_buttons.png");
    Texture2D hover_multiplayer = LoadTexture("resources/img/menu/hover_multiplayer.png");
    Texture2D hover_singleplayer = LoadTexture("resources/img/menu/hover_singleplayer.png"); 
    Texture2D hover_credits = LoadTexture("resources/img/menu/hover_credits.png");

    Sound menuSound = LoadSound("resources/sounds/menu/cancion.wav");
    Sound botonSound = LoadSound("resources/sounds/menu/boton.wav");
    bool menuSoundPlayed = false;
    bool botonSoundPlayed = false;

    Rectangle boton_m = { screenWidth/2, screenWidth/2 - 362, 300, 120 }; 
    Rectangle boton_s = { screenWidth/2, screenWidth/2 + 140 - 362, 300, 120 }; 
    Rectangle boton_c = { screenWidth/2 + 43, screenWidth/2 + 280 - 362, 220, 90 }; 

    bool boton_m_hover = false; 
    bool boton_m_click = false;

    bool boton_s_hover = false; 
    bool boton_s_click = false;

    bool boton_c_hover = false; 
    bool boton_c_click = false;  

    Rectangle botones[] = {boton_m, boton_s, boton_c};
    bool* hoverStates[] = {&boton_m_hover, &boton_s_hover, &boton_c_hover};
    bool* clickStates[] = {&boton_m_click, &boton_s_click, &boton_c_click};
    int types[] = {1, 0, -1}; // Usado para typeGame, -1 para creditos
    int screens[] = {MULTIPLAYER, SINGLEPLAYER, CREDITS};  


    // Variables Character (BOTH)
        // 0 -> singleplayer   1-> multiplayer
        int typeGame;
        bool playerChosen = false;
        bool playerChosenTwo = false; 
        
        /* 0 = default
           1 = yema
           2 = romeo
           3 = pistacho
           4 = kronos
           5 = donato
        */
        int playerChoice_one = 0; 
        int playerChoice_two = 0; 

        // 0 -> player1        1-> player2
        int actualPlayer; 

    Sound NextSound = LoadSound("resources/sounds/chars/next.wav");
    Sound WeeSound = LoadSound("resources/sounds/chars/wee.wav");

    float soundTimer = 0.0f;
    bool nextSoundPlayed = false;
    bool weeSoundPlayed = false;

        //YEMA
    Color yemaYellow = (Color){249, 181, 42, 255};
    Rectangle yemaRect = { screenWidth/2 - 305, screenWidth/2 - 292, 150, 150 };
    bool yemaHover = false;
    bool yemaClicked = false;
    Sound YemaSound = LoadSound("resources/sounds/chars/yema.wav");
    bool yemaSoundPlayed = false;

        //ROMEO
    Color romeoColor = (Color){255, 34, 33, 255};
    Rectangle romeoRect = { screenWidth/2 - 305, screenWidth/2 - 128, 150, 150 };
    bool romeoHover = false;
    bool romeoClicked = false;
    Sound RomeoSound = LoadSound("resources/sounds/chars/romeo.wav");
    bool romeoSoundPlayed = false;

        //PISTACHO
    Color pistachoColor = (Color){	109, 172, 100, 255};
    Rectangle pistachoRect = { screenWidth/2 + 205, screenWidth/2 - 292, 150, 150 };
    bool pistachoHover = false;
    bool pistachoClicked = false;
    Sound PistachoSound = LoadSound("resources/sounds/chars/pistacho.wav");
    bool pistachoSoundPlayed = false;

        //KRONOS
    Color kronosColor = (Color){84, 66, 66, 255};
    Rectangle kronosRect = { screenWidth/2 - 45, screenWidth/2 - 216, 150, 150 };
    bool kronosHover = false;
    bool kronosClicked = false;
    Sound KronosSound = LoadSound("resources/sounds/chars/kronos.wav");
    bool kronosSoundPlayed = false;

        //DONATO
    Color donatoColor = (Color){186, 114, 135, 255};
    Rectangle donatoRect = { screenWidth/2 + 205, screenWidth/2 - 125, 150, 150 };
    bool donatoHover = false;
    bool donatoClicked = false;
    Sound DonatoSound = LoadSound("resources/sounds/chars/donato.wav");
    bool donatoSoundPlayed = false;

    Rectangle rects[] = {yemaRect, romeoRect, pistachoRect, kronosRect, donatoRect};
    Color colors[] = {yemaYellow, romeoColor, pistachoColor, kronosColor, donatoColor};
    bool hovers[] = {yemaHover, romeoHover, pistachoHover, kronosHover, donatoHover};


    // Variables Character (Singleplayer)
    Texture2D singleplayerNormalTexture = LoadTexture("resources/img/char/player1_v2_normal.png");
    Texture2D singleplayerEnterTexture = LoadTexture("resources/img/char/player1_v2_enter.png");

    bool clicked[] = {yemaClicked, romeoClicked, pistachoClicked, kronosClicked, donatoClicked};
    Sound sounds[] = {YemaSound, RomeoSound, PistachoSound, KronosSound, DonatoSound};
    bool soundPlayed[] = {yemaSoundPlayed, romeoSoundPlayed, pistachoSoundPlayed, kronosSoundPlayed, donatoSoundPlayed};
    int choices[] = {1, 2, 3, 4, 5};

    // Variables Character (Multiplayer)
    Texture2D multiplayerNormalTexture = LoadTexture("resources/img/char/player2_v2_normal.png");
    Texture2D multiplayerEnterTexture = LoadTexture("resources/img/char/player2_v2_enter.png");


    //Variables Game Both
    int contadorPlayerOne = 0;
    int contadorPlayerTwo = 0;

    bool goalFlag = false;

    //0 -> por defecto   1-> player 1    2-> player 2
    int lastHitPlayer = 0;
 
    Sound gameSound = LoadSound("resources/sounds/game/game_song.wav");
    Sound latidoSound = LoadSound("resources/sounds/game/latido.wav");
    Sound passSound = LoadSound("resources/sounds/game/pass.wav");
    Sound touchSound = LoadSound("resources/sounds/game/touch.wav");
    Sound winSound = LoadSound("resources/sounds/game/win.wav");
    const char* contadorText;
    const char* contadorText2;

    bool gameSoundPlayed = false;

    //Variables Game Single
    Texture2D singleTxtre_1 = LoadTexture("resources/img/game/singleplayer_board_01.png");
    Texture2D singleTxtre_2 = LoadTexture("resources/img/game/singleplayer_board_02.png");
    Texture2D singleTxtre_3 = LoadTexture("resources/img/game/singleplayer_board_03.png");
    Texture2D singleTxtre_4 = LoadTexture("resources/img/game/singleplayer_board_04.png");
    Texture2D singleTxtre_5 = LoadTexture("resources/img/game/singleplayer_board_05.png");

    Texture2D textures[] = {singleTxtre_1, singleTxtre_2, singleTxtre_3, singleTxtre_4, singleTxtre_5};

    Vector2 ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    Vector2 ballSpeed = { 4.0f, 3.0f };
    int ballRadius = 6;
    bool pause = 0;

    Rectangle rectPlayerA = { 158, 383, 10, 30 };  
    Rectangle rectPlayerB = { 637, 156, 10, 30 }; 

    Rectangle rectGoalPlayerA = { 130, 130, 8, 292 }; 
    Rectangle rectGoalPlayerB = { 660, 130, 8, 292 }; 

    float playerSpeed = 5.0f;  

    // Variables Game Multi
    Texture2D multi_1_1Txtre = LoadTexture("resources/img/game/multiplayer_board_1_1.png");
    Texture2D multi_1_2Txtre = LoadTexture("resources/img/game/multiplayer_board_1_2.png");
    Texture2D multi_1_3Txtre = LoadTexture("resources/img/game/multiplayer_board_1_3.png");
    Texture2D multi_1_4Txtre = LoadTexture("resources/img/game/multiplayer_board_1_4.png");
    Texture2D multi_1_5Txtre = LoadTexture("resources/img/game/multiplayer_board_1_5.png");
    Texture2D multi_2_1Txtre = LoadTexture("resources/img/game/multiplayer_board_2_1.png");
    Texture2D multi_2_2Txtre = LoadTexture("resources/img/game/multiplayer_board_2_2.png");
    Texture2D multi_2_3Txtre = LoadTexture("resources/img/game/multiplayer_board_2_3.png");
    Texture2D multi_2_4Txtre = LoadTexture("resources/img/game/multiplayer_board_2_4.png");
    Texture2D multi_2_5Txtre = LoadTexture("resources/img/game/multiplayer_board_2_5.png");
    Texture2D multi_3_1Txtre = LoadTexture("resources/img/game/multiplayer_board_3_1.png");
    Texture2D multi_3_2Txtre = LoadTexture("resources/img/game/multiplayer_board_3_2.png");
    Texture2D multi_3_3Txtre = LoadTexture("resources/img/game/multiplayer_board_3_3.png");
    Texture2D multi_3_4Txtre = LoadTexture("resources/img/game/multiplayer_board_3_4.png");
    Texture2D multi_3_5Txtre = LoadTexture("resources/img/game/multiplayer_board_3_5.png");
    Texture2D multi_4_1Txtre = LoadTexture("resources/img/game/multiplayer_board_4_1.png");
    Texture2D multi_4_2Txtre = LoadTexture("resources/img/game/multiplayer_board_4_2.png");
    Texture2D multi_4_3Txtre = LoadTexture("resources/img/game/multiplayer_board_4_3.png");
    Texture2D multi_4_4Txtre = LoadTexture("resources/img/game/multiplayer_board_4_4.png");
    Texture2D multi_4_5Txtre = LoadTexture("resources/img/game/multiplayer_board_4_5.png");
    Texture2D multi_5_1Txtre = LoadTexture("resources/img/game/multiplayer_board_4_1.png");
    Texture2D multi_5_2Txtre = LoadTexture("resources/img/game/multiplayer_board_4_2.png");
    Texture2D multi_5_3Txtre = LoadTexture("resources/img/game/multiplayer_board_4_3.png");
    Texture2D multi_5_4Txtre = LoadTexture("resources/img/game/multiplayer_board_4_4.png");
    Texture2D multi_5_5Txtre = LoadTexture("resources/img/game/multiplayer_board_4_5.png");

    Texture2D multiTextures[5][5] = {
        {multi_1_1Txtre, multi_1_2Txtre, multi_1_3Txtre, multi_1_4Txtre, multi_1_5Txtre},
        {multi_2_1Txtre, multi_2_2Txtre, multi_2_3Txtre, multi_2_4Txtre, multi_2_5Txtre},
        {multi_3_1Txtre, multi_3_2Txtre, multi_3_3Txtre, multi_3_4Txtre, multi_3_5Txtre},
        {multi_4_1Txtre, multi_4_2Txtre, multi_4_3Txtre, multi_4_4Txtre, multi_4_5Txtre},
        {multi_5_1Txtre, multi_5_2Txtre, multi_5_3Txtre, multi_5_4Txtre, multi_5_5Txtre}
    };

    // Variables Credits
    Texture2D creditsTexture = LoadTexture("resources/img/credits/main.png");

    // Variables ENDING
    Texture2D endingTexture = LoadTexture("resources/img/ending/ending.png");


    int framesCounter = 0;
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {

        estadoActual->Update();

        Vector2 mouse = GetMousePosition();

        timer += GetFrameTime();

        if (currentScreen == MENU && !menuSoundPlayed)
        {
            PlaySound(menuSound);
            menuSoundPlayed = true;
        }

        switch (currentScreen) 
        {
            case MENU: 
                botonSoundPlayed = false;
                boton_m_hover = false; 
                boton_m_click = false;
                boton_s_hover = false; 
                boton_s_click = false;
                boton_c_hover = false; 
                boton_c_click = false;  
                playerChosen = false;
                playerChosenTwo = false; 
                playerChoice_one = 0; 
                playerChoice_two = 0; 
                nextSoundPlayed = false;
                weeSoundPlayed = false;
                yemaClicked = false;
                yemaSoundPlayed = false;
                romeoClicked = false;
                romeoSoundPlayed = false;
                pistachoClicked = false;
                pistachoSoundPlayed = false;
                kronosClicked = false;
                kronosSoundPlayed = false;
                donatoClicked = false;
                donatoSoundPlayed = false;
                contadorPlayerOne = 0;
                contadorPlayerTwo = 0;
                lastHitPlayer = 0;
                gameSoundPlayed = false;
                
                for (int i = 0; i < 5; i++)
                {
                    clicked[i] = false;
                }
                

                for (int i = 0; i < 3; i++)
                {
                    if (CheckCollisionPointRec(mouse, botones[i]))
                    {
                        *hoverStates[i] = true;

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            *clickStates[i] = true;
                            PlaySound(botonSound);
                            botonSoundPlayed = true;
                            if (types[i] != -1) {
                                typeGame = types[i];
                                actualPlayer = 0;
                            }
                            currentScreen = screens[i];
                        }
                    }
                    else
                    {
                        *hoverStates[i] = false;
                    }
                }

            break;

            case MULTIPLAYER:

                if(actualPlayer == 0)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        if (CheckCollisionPointRec(mouse, rects[i]) && !playerChosen)
                        {
                            hovers[i] = true;

                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                clicked[i] = true;
                                PlaySound(botonSound);
                                PlaySound(sounds[i]);
                                soundPlayed[i] = true;
                                playerChosen = true;
                                playerChoice_one = choices[i];
                                actualPlayer = 1;
                            }
                        }
                        else
                        {
                            hovers[i] = clicked[i] ? true : false;
                        }
                    }

                    //both

                    if(playerChosen && !nextSoundPlayed)
                    {
                        PlaySound(NextSound);
                        nextSoundPlayed = true;
                        soundTimer = 0.0f; 
                    }
                    else if(nextSoundPlayed && !weeSoundPlayed)
                    {
                        soundTimer += GetFrameTime();
                        if(soundTimer >= 2.0f)
                        {
                            PlaySound(WeeSound);
                            weeSoundPlayed = true;
                        }
                    }

                }

                else //player2
                {
                    playerChosen = false;
                    nextSoundPlayed = false;
                    weeSoundPlayed = false;
                    yemaClicked = false;
                    romeoClicked = false;
                    pistachoClicked = false;
                    kronosClicked = false;
                    donatoClicked = false;

                    for (int i = 0; i < 5; i++)
                    {
                        if (CheckCollisionPointRec(mouse, rects[i]) && !playerChosenTwo)
                        {
                            hovers[i] = true;

                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                clicked[i] = true;
                                PlaySound(botonSound);
                                PlaySound(sounds[i]);
                                soundPlayed[i] = true;
                                playerChosenTwo = true;
                                playerChoice_two = choices[i];
                            }
                        }
                        else
                        {
                            hovers[i] = clicked[i] ? true : false;
                        }
                    }

                }

                    //both
                    nextSoundPlayed = false;
                    weeSoundPlayed = false;

                    if(playerChosenTwo && !nextSoundPlayed)
                    {
                        PlaySound(NextSound);
                        nextSoundPlayed = true;
                        soundTimer = 0.0f; 
                    }
                    else if(nextSoundPlayed && !weeSoundPlayed)
                    {
                        soundTimer += GetFrameTime();
                        if(soundTimer >= 2.0f)
                        {
                            PlaySound(WeeSound);
                            weeSoundPlayed = true;
                        }
                    }


                    if (playerChoice_one != 0 && playerChoice_two != 0)
                    {
                        if(IsKeyPressed(KEY_ENTER))
                        {
                            currentScreen = GAME_MULTI;
                        }
                    }
                
            break;

            case SINGLEPLAYER:

                for (int i = 0; i < 5; i++)
                {
                    if (CheckCollisionPointRec(mouse, rects[i]) && !playerChosen)
                    {
                        hovers[i] = true;

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            clicked[i] = true;
                            PlaySound(botonSound);
                            PlaySound(sounds[i]);
                            soundPlayed[i] = true;
                            playerChosen = true;
                            playerChoice_one = choices[i];
                        }
                    }
                    else
                    {
                        hovers[i] = clicked[i] ? true : false;
                    }
                }


                //both

                if(playerChosen && !nextSoundPlayed)
                {
                    PlaySound(NextSound);
                    nextSoundPlayed = true;
                    soundTimer = 0.0f; 
                }
                else if(nextSoundPlayed && !weeSoundPlayed)
                {
                    soundTimer += GetFrameTime();
                    if(soundTimer >= 2.0f)
                    {
                        PlaySound(WeeSound);
                        weeSoundPlayed = true;
                    }
                }

                if (playerChoice_one != 0)
                {
                    if(IsKeyPressed(KEY_ENTER))currentScreen = GAME_SINGLE;
                }

            break;

            case GAME_MULTI:

                StopSound(menuSound);
                StopSound(WeeSound);
                StopSound(NextSound);

                if (!gameSoundPlayed) 
                { 
                    PlaySound(gameSound);
                    gameSoundPlayed = true;
                }

                if(contadorPlayerOne == 4 || contadorPlayerTwo == 4)
                {
                    if(!IsSoundPlaying(latidoSound)) PlaySound(latidoSound);
                }

                if (!pause)
                {
                    ballPosition.x += ballSpeed.x;
                    ballPosition.y += ballSpeed.y;

                    if ((ballPosition.x >= (672 - ballRadius)) || (ballPosition.x <= (125 + ballRadius)))
                        ballSpeed.x *= -1.0f;

                    if ((ballPosition.y >= (425 - ballRadius)) || (ballPosition.y <= (128 + ballRadius)))
                        ballSpeed.y *= -1.0f;

                    if (IsKeyDown(KEY_W) && rectPlayerA.y > 128) rectPlayerA.y -= playerSpeed;
                    if (IsKeyDown(KEY_S) && rectPlayerA.y + rectPlayerA.height < 425) rectPlayerA.y += playerSpeed;

                    if (IsKeyDown(KEY_UP) && rectPlayerB.y > 128) rectPlayerB.y -= playerSpeed;
                    if (IsKeyDown(KEY_DOWN) && rectPlayerB.y + rectPlayerB.height < 425) rectPlayerB.y += playerSpeed;

                    if (CheckCollisionCircleRec(ballPosition, ballRadius, rectPlayerA))
                    {
                        ballSpeed.x *= -1.0f;
                        lastHitPlayer = 1;
                        PlaySound(touchSound);
                    }
                        
                    if (CheckCollisionCircleRec(ballPosition, ballRadius, rectPlayerB))
                    {
                        ballSpeed.x *= -1.0f;
                        lastHitPlayer = 2;
                        PlaySound(touchSound);
                    }

                    if(!goalFlag)
                    {
                        if (CheckCollisionCircleRec(ballPosition, ballRadius, rectGoalPlayerB) && lastHitPlayer == 1)
                        {
                            contadorPlayerOne += 1;
                            goalFlag = true;
                            lastHitPlayer = 0;
                            ballSpeed.x += 0.5f;
                            ballSpeed.y += 0.5f; 
                            PlaySound(passSound);
                        }

                        if (CheckCollisionCircleRec(ballPosition, ballRadius, rectGoalPlayerA) && lastHitPlayer == 2)
                        {
                            contadorPlayerTwo += 1;
                            goalFlag = true;
                            lastHitPlayer = 0;
                            ballSpeed.x += 0.5f;
                            ballSpeed.y += 0.5f; 
                            PlaySound(passSound);
                        }
                    }

                    if ((ballPosition.x < 130 && goalFlag == true) || (ballPosition.x > 660 && goalFlag == true)) 
                    {
                        goalFlag = false; 
                    }

                    if(contadorPlayerOne == 5 || contadorPlayerTwo == 5)
                    {
                        currentScreen = ENDGAME;
                    }

                }
                else framesCounter++;

            break;

            case GAME_SINGLE:

                StopSound(menuSound);
                StopSound(WeeSound);
                StopSound(NextSound);

                if (!gameSoundPlayed) 
                { 
                    PlaySound(gameSound);
                    gameSoundPlayed = true;
                }

                if(contadorPlayerOne == 4 || contadorPlayerTwo == 4)
                {
                    if(!IsSoundPlaying(latidoSound)) PlaySound(latidoSound);
                }

                if (!pause)
                {
                    ballPosition.x += ballSpeed.x;
                    ballPosition.y += ballSpeed.y;

                    if ((ballPosition.x >= (672 - ballRadius)) || (ballPosition.x <= (125 + ballRadius)))
                        ballSpeed.x *= -1.0f;

                    if ((ballPosition.y >= (425 - ballRadius)) || (ballPosition.y <= (128 + ballRadius)))
                        ballSpeed.y *= -1.0f;

                    if (IsKeyDown(KEY_W) && rectPlayerA.y > 128) rectPlayerA.y -= playerSpeed;
                    if (IsKeyDown(KEY_S) && rectPlayerA.y + rectPlayerA.height < 425) rectPlayerA.y += playerSpeed;

                    if (CheckCollisionCircleRec(ballPosition, ballRadius, rectPlayerA))
                    {
                        ballSpeed.x *= -1.0f;
                        lastHitPlayer = 1;
                        PlaySound(touchSound);
                    }

                    if(!goalFlag)
                    {
                        if (CheckCollisionCircleRec(ballPosition, ballRadius, rectGoalPlayerB) && lastHitPlayer == 1)
                        {
                            contadorPlayerOne += 1;
                            goalFlag = true;
                            lastHitPlayer = 0;
                            ballSpeed.x += 0.5f;
                            ballSpeed.y += 0.5f; 
                            PlaySound(passSound);
                        }
                    }

                    if (ballPosition.x > 660 && goalFlag == true) goalFlag = false; 
                    if(contadorPlayerOne == 5) currentScreen = ENDGAME;
                }
                else framesCounter++;
            break;

            case ENDGAME:

                PlaySound(winSound);

                if (IsKeyPressed(KEY_X))
                {
                    StopSound(gameSound);
                    currentScreen = MENU;
                }
            break;

            case CREDITS:
                if (IsKeyPressed(KEY_X)) currentScreen = MENU;
            break;

            default: break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen) 
        {
            case MENU:
                ClearBackground(RAYWHITE);

                DrawTexture(normal_buttons, screenWidth / 2 - normal_buttons.width / 2, screenHeight / 2 - normal_buttons.height / 2, WHITE);


                if (boton_m_hover)
                {
                    DrawRectangleRec(boton_m, RED);
                    DrawTexture(hover_multiplayer, screenWidth / 2 - hover_multiplayer.width / 2, screenHeight / 2 - hover_multiplayer.height / 2, WHITE);
                }

                if (boton_s_hover)
                {
                    DrawTexture(hover_singleplayer, screenWidth / 2 - hover_singleplayer.width / 2, screenHeight / 2 - hover_singleplayer.height / 2, WHITE);
                }

                if (boton_c_hover)
                {
                    DrawTexture(hover_credits, screenWidth / 2 - hover_credits.width / 2, screenHeight / 2 - hover_credits.height / 2, WHITE);
                }
                break;

            case MULTIPLAYER:

                ClearBackground(RAYWHITE);
                DrawTexture(multiplayerNormalTexture, screenWidth / 2 - multiplayerNormalTexture.width / 2, screenHeight / 2 - multiplayerNormalTexture.height / 2, WHITE);

                if(playerChoice_one != 0 && playerChoice_two != 0)
                {
                    DrawTexture(multiplayerEnterTexture, screenWidth / 2 - multiplayerEnterTexture.width / 2, screenHeight / 2 - multiplayerEnterTexture.height / 2, WHITE);
                }

                for (int i = 0; i < 5; i++)
                {
                    if (hovers[i]) DrawRectangleLinesEx(rects[i], 6, colors[i]);
                } 

                break;

            case SINGLEPLAYER:
                ClearBackground(RAYWHITE);
                
                DrawTexture(singleplayerNormalTexture, screenWidth / 2 - singleplayerNormalTexture.width / 2, screenHeight / 2 - singleplayerNormalTexture.height / 2, WHITE);
                    
                if(playerChosen)
                {
                    DrawTexture(singleplayerEnterTexture, screenWidth / 2 - singleplayerEnterTexture.width / 2, screenHeight / 2 - singleplayerEnterTexture.height / 2, WHITE);
                }
                
                for (int i = 0; i < 5; i++)
                {
                    if (hovers[i])DrawRectangleLinesEx(rects[i], 6, colors[i]);
                }             

            break;

            case GAME_MULTI:

                if (playerChoice_one > 0 && playerChoice_one <= 5 && playerChoice_two > 0 && playerChoice_two <= 5)
                {
                    Texture2D selectedTexture = multiTextures[playerChoice_one - 1][playerChoice_two - 1];
                    DrawTexture(selectedTexture, screenWidth / 2 - selectedTexture.width / 2, screenHeight / 2 - selectedTexture.height / 2, WHITE);
                }

                contadorText = TextFormat("%d", contadorPlayerOne);
                contadorText2 = TextFormat("%d", contadorPlayerTwo);

                DrawText(contadorText, 232, 47, 30, WHITE);
                DrawText(contadorText, 231, 46, 30, (Color){48, 83, 79, 255});
                DrawText(contadorText, 230, 45, 30, YELLOW);

                DrawText(contadorText2, 587, 47, 30, WHITE);
                DrawText(contadorText2, 586, 46, 30, (Color){48, 83, 79, 255});
                DrawText(contadorText2, 585, 45, 30, YELLOW);

                DrawCircleV(ballPosition, (float)ballRadius, MAROON);

                DrawRectangleRec(rectPlayerA, WHITE);
                DrawRectangleRec(rectPlayerB, WHITE);

                DrawRectangleRec(rectGoalPlayerA, (Color){249, 181, 42, 255});  
                DrawRectangleRec(rectGoalPlayerB, (Color){249, 181, 42, 255});  

            break;

            case GAME_SINGLE:
            
                if (playerChoice_one >= 1 && playerChoice_one <= 5) {
                    int index = playerChoice_one - 1; 
                    DrawTexture(textures[index], screenWidth / 2 - textures[index].width / 2, screenHeight / 2 - textures[index].height / 2, WHITE);
                }

                contadorText = TextFormat("%d", contadorPlayerOne);
                DrawText(contadorText, 232, 47, 30, WHITE);
                DrawText(contadorText, 231, 46, 30, (Color){48, 83, 79, 255});
                DrawText(contadorText, 230, 45, 30, YELLOW);
                DrawText("/", 587, 47, 30, WHITE);
                DrawText("/", 586, 46, 30, (Color){48, 83, 79, 255});
                DrawText("/", 585, 45, 30, YELLOW);

                DrawCircleV(ballPosition, (float)ballRadius, MAROON);

                DrawRectangleRec(rectPlayerA, WHITE);

                DrawRectangleRec(rectGoalPlayerA, (Color){249, 181, 42, 255});  
                DrawRectangleRec(rectGoalPlayerB, (Color){249, 181, 42, 255});  

            break;

            case ENDGAME:

                DrawTexture(endingTexture, screenWidth / 2 - endingTexture.width / 2, screenHeight / 2 - endingTexture.height / 2, WHITE);

                if(contadorPlayerOne == 5)
                {
                    DrawText("Player 1", 536, 122, 30, WHITE);
                    DrawText("Player 1", 535, 121, 30, (Color){48, 83, 79, 255});
                    DrawText("Player 1", 534, 120, 30, YELLOW);
                }

                if(contadorPlayerTwo == 5)
                {
                    DrawText("Player 2", 536, 122, 30, WHITE);
                    DrawText("Player 2", 535, 121, 30, (Color){48, 83, 79, 255});
                    DrawText("Player 2", 534, 120, 30, YELLOW);
                }

                break;

            case CREDITS:

                DrawTexture(creditsTexture, screenWidth / 2 - creditsTexture.width / 2, screenHeight / 2 - creditsTexture.height / 2, WHITE);

            default: break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
