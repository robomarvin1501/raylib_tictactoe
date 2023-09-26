#include "raylib.h"
#include <signal.h>
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
const int GameTextSize = 80;
const int LetterOffset = 20;
const int EndTextOffset = 40;

typedef enum GameEnd {
    NOT_ENDED,
    X_WIN,
    O_WIN,
    DRAW,
    UNKNOWN,
} GameEnd;

enum GameEnd LineVictory(enum CellContents i, enum CellContents j, enum CellContents k) {
    if (i == EMPTY || j == EMPTY || k == EMPTY) {
        return NOT_ENDED;
    } else if (i == X && j == X && k == X) {
        return X_WIN;
    } else if (i == O && j == O && k == O) {
        return O_WIN;
    }
    return NOT_ENDED;
}

enum GameEnd CheckGameEnded() {
    enum GameEnd GameStates[] = {
        LineVictory(GameBoard[0], GameBoard[3], GameBoard[6]), // row0
        LineVictory(GameBoard[1], GameBoard[4], GameBoard[7]), // row1
        LineVictory(GameBoard[2], GameBoard[5], GameBoard[8]), // row2
        LineVictory(GameBoard[0], GameBoard[1], GameBoard[2]), // column0
        LineVictory(GameBoard[3], GameBoard[4], GameBoard[5]), // column1
        LineVictory(GameBoard[6], GameBoard[7], GameBoard[8]), // column2
        LineVictory(GameBoard[0], GameBoard[4], GameBoard[8]), // diagonal_ltr
        LineVictory(GameBoard[2], GameBoard[4], GameBoard[8]), // diagonal_rtl
    };

    for (int i=0; i<8; i++) {
        if (GameStates[i] == UNKNOWN) {
            raise(420);
            return UNKNOWN;
        } else if (GameStates[i] != NOT_ENDED) {
            return GameStates[i];
        }
    }
    return NOT_ENDED;
}


enum WhichPlayer {
    PLAYER_0,
    PLAYER_1,
};

enum WhichPlayer whichPlayer = PLAYER_0;
enum GameEnd currentGameState = NOT_ENDED;

int main()
{
    const int screenWidth = 800;
    const int screenHeight = screenWidth;
    const int cellWidth = screenWidth / 3;
    const int cellHeight = screenHeight / 3;

    InitWindow(screenWidth, screenHeight, "TIC TAC TOE");


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
                if (!gamePaused && (currentGameState != DRAW || currentGameState != X_WIN || currentGameState != O_WIN)) {
                    int playerInteraction = 0;
                    if (IsKeyPressed(KEY_ZERO) && GameBoard[0] == EMPTY) {
                        GameBoard[0] = whichPlayer + 1; // +1 because of the enums
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_ONE) && GameBoard[1] == EMPTY) {
                        GameBoard[1] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_TWO) && GameBoard[2] == EMPTY) {
                        GameBoard[2] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_THREE) && GameBoard[3] == EMPTY) {
                        GameBoard[3] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_FOUR) && GameBoard[4] == EMPTY) {
                        GameBoard[4] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_FIVE) && GameBoard[5] == EMPTY) {
                        GameBoard[5] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_SIX) && GameBoard[6] == EMPTY) {
                        GameBoard[6] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_SEVEN) && GameBoard[7] == EMPTY) {
                        GameBoard[7] = whichPlayer + 1;
                        playerInteraction = 1;
                    } else if (IsKeyPressed(KEY_EIGHT) && GameBoard[8] == EMPTY) {
                        GameBoard[8] = whichPlayer + 1;
                        playerInteraction = 1;
                    }

                    if (playerInteraction == 1) {
                        whichPlayer = !whichPlayer;
                        currentGameState = CheckGameEnded();
                    }

                if (IsKeyPressed(KEY_ENTER)) screen = ENDING;
                }

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
                        char cellOccupant[1];

                        if (GameBoard[i] == X) {
                            cellOccupant[0] = 'X';
                        } else if (GameBoard[i] == O) {
                            cellOccupant[0] = 'O';
                        } else {
                            cellOccupant[0] = '-';
                        }
                        DrawText(
                            cellOccupant,
                            position.x + cellWidth / 2 - LetterOffset,
                            position.y + cellHeight / 2 - LetterOffset,
                            GameTextSize,
                            BLACK
                        );
                    }
                    const char* whichPlayerString = "Player %s";
                    if (whichPlayer == PLAYER_0) {
                        whichPlayerString = TextFormat(whichPlayerString, "0");
                    } else if (whichPlayer == PLAYER_1) {
                        whichPlayerString = TextFormat(whichPlayerString, "1");
                    }
                    DrawText(
                        whichPlayerString,
                        screenWidth / 2 - 80,
                        20,
                        40,
                        BLACK
                    );


                    if (currentGameState == DRAW) {
                        DrawText(
                            "DRAW",
                            screenWidth / 2 - EndTextOffset,
                            screenHeight / 2 - EndTextOffset,
                            100,
                            RED
                        );
                    } else if (currentGameState == X_WIN) {
                        DrawText(
                            "X WINS",
                            screenWidth / 2 - EndTextOffset,
                            screenHeight / 2 - EndTextOffset,
                            100,
                            RED
                        );
                    } else if (currentGameState == O_WIN) {
                        DrawText(
                            "O WINS",
                            screenWidth / 2 - EndTextOffset,
                            screenHeight / 2 - EndTextOffset,
                            100,
                            RED
                        );
                    }
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
