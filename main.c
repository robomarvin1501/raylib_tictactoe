#include "raylib.h"
#include <stdio.h>

typedef enum GameScreen { LOGO, TITLE, GAMEPLAY, ENDING } GameScreen;

struct CellPosition {
    int x;
    int y;
};

struct CellPosition getCellPosition(int boardIndex) {
    struct CellPosition pos = {0, 0};
    pos.x = boardIndex / 3;
    pos.y = boardIndex % 3;
    return pos;
}

enum CellContents{
    EMPTY,
    X,
    O,
};

int GameBoard[9];

enum WhichPlayer {
    PLAYER_0,
    PLAYER_1,
};

enum WhichPlayer whichPlayer = PLAYER_0;

int main()
{
    const int screenWidth = 800;
    const int screenHeight = screenWidth;
    const int cellWidth = screenWidth / 3;
    const int cellHeight = screenHeight / 3;

    InitWindow(screenWidth, screenHeight, "PROJECT: BLOCKS GAME");


    GameScreen screen = LOGO;       // Current game screen state

    int framesCounter = 0;          // General pourpose frames counter
    int gameResult = -1;            // Game result: 0 - Loose, 1 - Win, -1 - Not defined
    bool gamePaused = false;        // Game paused state toggle

    SetTargetFPS(60);               // Set desired framerate (frames per second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(screen)
        {
            case LOGO:
            {
                framesCounter++;

                if (framesCounter > 1)// 80)
                {
                    screen = TITLE;    // Change to TITLE screen after 3 seconds
                    framesCounter = 0;
                }

            } break;
            case TITLE:
            {
                // Update TITLE screen data here!

                framesCounter++;

                if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;

            } break;
            case GAMEPLAY:
            {

                if (!gamePaused)
                    if (IsKeyPressed(KEY_ZERO)) {
                        GameBoard[0] = X;
                    }

                if (IsKeyPressed(KEY_ENTER)) screen = ENDING;

            } break;
            case ENDING:
            {

                framesCounter++;

                if (IsKeyPressed(KEY_ENTER)) screen = TITLE;

            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(screen)
            {
                case LOGO:
                {
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 3 SECONDS...", 290, 220, 20, GRAY);

                } break;
                case TITLE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

                } break;
                case GAMEPLAY:
                {
                    // DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    for (int i=0; i<9; i++) {
                        struct CellPosition position = getCellPosition(i);
                        position.x = position.x * cellWidth;
                        position.y = position.y * cellHeight;
                        DrawRectangleLines(
                            position.x,
                            position.y,
                            cellWidth,
                            cellHeight,
                            BLACK
                        );

                        DrawText(
                                TextFormat("%d", i),
                                position.x + 20,
                                position.y + 20,
                                14,
                                BLACK
                        );

                        if (GameBoard[i] == X) {
                            DrawText(
                                "X",
                                position.x + cellWidth / 2,
                                position.y + cellHeight / 2,
                                20,
                                BLACK
                            );
                        } else if (GameBoard[i] == O) {
                            DrawText(
                                "O",
                                position.x + cellWidth / 2,
                                position.y + cellHeight / 2,
                                20,
                                BLACK
                            );
                        } else {
                            DrawText(
                                "EMPTY",
                                position.x + cellWidth / 2,
                                position.y + cellHeight / 2,
                                20,
                                BLACK
                            );

                        }
                    }
                    const char* whichPlayerString = "Player %s";
                    if (whichPlayer == PLAYER_0) {
                        whichPlayerString = TextFormat(whichPlayerString, "0");
                    } else if (whichPlayer == PLAYER_1) {
                        whichPlayerString = TextFormat(whichPlayerString, "1");
                    }
                    DrawText(
                        "Player 0",
                        screenWidth / 2 - 80,
                        20,
                        40,
                        BLACK
                    );

                } break;
                case ENDING:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // NOTE: Unload any loaded resources (texture, fonts, audio)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
