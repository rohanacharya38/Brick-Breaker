#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
const int WIDTH = 600;
const int HEIGHT = 400;
const int BALL_SIZE = 10;
int roundno = 1;
void call(void);
unsigned int count[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
typedef struct Ball
{
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;
typedef struct Player
{
    int score;
    float xPosition;

} Player;
typedef struct Bricks
{
    float xPositon;
    float yPosition;
    int size;
} Bricks;
Bricks brick[20];
Ball ball;
Player player1;
Player player2;
const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 75;
const int PLAYER_MARGIN = 5;
const int BRICK_WIDTH = 60;
const int BRICK_HEIGHT = 10;
float SPEED = 130;
float PLAYER_MOVE_SPEED = 150.0f;
bool served;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

TTF_Font *gFont = NULL;
bool Initialize(void);
void Shutdown(void);
void Update(float);
Ball MakeBall(int size);
void UpdateBall(Ball *ball, float elapsed);
void RenderBall(const Ball *);
Player MakePlayers(void);
void UpdatePlayers(float elapsed);
void RenderPlayers(void);
void UpdateScore(int player, int points);
// void MakeBricks(void);
void UpdateBricks(void);
void RestartGame(void);
// void RenderBricks(void);
SDL_Color textColor = {255, 255, 255};
    
int main(int argc, char *argv[])
{

    srand((unsigned int)time(NULL));
    atexit(Shutdown);
    if (!Initialize())
    {
        exit(1);
    }
    bool quit = false;
    SDL_Event event;
    Uint32 lastTick = SDL_GetTicks();
    call();
    while (!quit)
    {
        const Uint8 *kState = SDL_GetKeyboardState(NULL);
        if (kState[SDL_SCANCODE_F])
        {
            UpdateScore(2,0);
        }
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        Uint32 curTick = SDL_GetTicks();
        Uint32 diff = curTick - lastTick;
        float elapsed = diff / 1000.0f;
        Update(elapsed);
        lastTick = curTick;
        if (roundno == 10)
        {
            quit = true;
        }
    }
    SDL_Quit();
    return 0;
}
bool Initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL:%s\n", SDL_GetError());
        return false;
    }
     char *pointsTable = "*****************************************Brick Breaker**************************************";
    int len = snprintf(NULL, 0, pointsTable);
    char buf[len + 1];
    snprintf(buf, len + 1, pointsTable);
    window = SDL_CreateWindow("Rohan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SetWindowTitle(window, buf);

    if (!window)
    {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        return false;
    }
    if (TTF_Init() == -1)
    {
        return false;
    }
    ball = MakeBall(BALL_SIZE);
    player1 = MakePlayers();
    player2 = MakePlayers();
    // MakeBricks();
    return true;
}
void Update(float elapsed)
{

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    UpdateBall(&ball, elapsed);
    RenderBall(&ball);

    UpdatePlayers(elapsed);
    RenderPlayers();

    UpdateBricks();
    SDL_RenderPresent(renderer);
}
void Shutdown(void)
{
    if (renderer)
    {

        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();
}
bool CoinFlip(void)
{
    return rand() % 2 == 1 ? true : false;
}
Ball MakeBall(int size)
{
    Ball ball = {
        .x = WIDTH / 2,
        .y = HEIGHT - PLAYER_WIDTH,
        .size = size,
        .xSpeed = SPEED * (CoinFlip() ? 1 : -1),
        .ySpeed = SPEED * (CoinFlip() ? 1 : -1),
    };
    return ball;
}
void RenderBall(const Ball *ball)
{
    int size = ball->size;
    int halfSize = size / 2;

    SDL_Rect rect =
        {
            .x = ball->x - halfSize,
            .y = ball->y - halfSize,
            .w = size,
            .h = size,
        };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void UpdateBall(Ball *ball, float elapsed)
{
    if (!served)
    {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT - PLAYER_WIDTH - PLAYER_MARGIN;
        player2.xPosition = WIDTH / 2 - PLAYER_HEIGHT / 4 - PLAYER_MARGIN;
        const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    // if (keyboardState[SDL_SCANCODE_RIGHT])
    // {
    //     ball->xSpeed=SPEED*SPEED;
    //     ball->ySpeed=SPEED*SPEED;
    // }
    // if (keyboardState[SDL_SCANCODE_LEFT])
    // {
    //     ball->xSpeed=-SPEED;
    //     ball->ySpeed=-SPEED;
    // }
    ball->xSpeed=SPEED;
    ball->ySpeed=SPEED;
        return;
    }
    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;
    if (ball->x < BALL_SIZE / 2)
    {
        ball->xSpeed = +fabs(ball->xSpeed);
    }
    if (ball->x > WIDTH - BALL_SIZE)
    {
        ball->xSpeed = -fabs(ball->xSpeed);
    }
    if (ball->y < BALL_SIZE / 2)
    {
        ball->ySpeed = fabs(ball->ySpeed);
    }
    if (ball->y > HEIGHT - BALL_SIZE)
    {
        // ball->ySpeed = -fabs(ball->ySpeed);
        RestartGame();
    }
}
Player MakePlayers(void)
{
    Player player = {
        .xPosition = WIDTH / 2 - PLAYER_HEIGHT / 4 - PLAYER_MARGIN,
    };
    return player;
}
void UpdatePlayers(float elapsed)
{

    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_SPACE])
    {
        served = true;
    }
    if (keyboardState[SDL_SCANCODE_LEFT])
    {
        player2.xPosition -= PLAYER_MOVE_SPEED * elapsed;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT])
    {
        player2.xPosition += PLAYER_MOVE_SPEED * elapsed;
    }
    if (player2.xPosition < PLAYER_WIDTH / 2)
    {
        player2.xPosition = PLAYER_WIDTH / 2;
    }
    if (player2.xPosition > WIDTH - PLAYER_HEIGHT/4-PLAYER_MARGIN)
    {
        player2.xPosition = WIDTH - PLAYER_HEIGHT/4-PLAYER_MARGIN;
    }
    //check if  ball rect overlaps with either player rect
    SDL_Rect ballRect = {
        .x = ball.x - ball.size / 2,
        .y = ball.y - ball.size / 2,
        .w = ball.size,
        .h = ball.size,
    };
    SDL_Rect player2Rect = {
        .x = (int)(player2.xPosition) - PLAYER_WIDTH / 2,
        .y = HEIGHT - PLAYER_WIDTH,
        .w = PLAYER_HEIGHT,
        .h = PLAYER_WIDTH,
    };
    if (SDL_HasIntersection(&ballRect, &player2Rect))
    {
        ball.ySpeed = -fabs(ball.ySpeed); //ball goes left
    }
}
void RenderPlayers(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect player2Rect = {
        .x = (int)(player2.xPosition) - PLAYER_WIDTH / 2,
        .y = HEIGHT - PLAYER_WIDTH,
        .w = PLAYER_HEIGHT,
        .h = PLAYER_WIDTH,
    };
    SDL_RenderFillRect(renderer, &player2Rect);
}
void UpdateScore(int player, int points)
{
     
    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, "CONGRATULATIONS! YOU LEVELED UP", textColor);
    SDL_Texture *mTexture =SDL_CreateTextureFromSurface(renderer,textSurface);
     gFont=TTF_OpenFont("arial.ttf",18);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_Rect dest={.x=WIDTH/2-(textSurface->w)/2,.y=HEIGHT/2,.w=textSurface->w,.h=textSurface->h};
    SDL_RenderFillRect(renderer,&dest);
    SDL_RenderCopy(renderer,mTexture,NULL,&dest);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_free(textSurface);
    SDL_DestroyTexture(mTexture);
    served = false;
    if (player == 2)
    {
        player2.score += points;
    }
    roundno++;
    for (int i = 0; i < 20; i++)
    {
        count[i] = roundno + 1;
    }
    PLAYER_MOVE_SPEED *= 1.3;
    SPEED *= 1.2;

    char *pointsTable = "*********Level %d***********";
    int len = snprintf(NULL, 0, pointsTable, roundno);
    char buf[len + 1];
    snprintf(buf, len + 1, pointsTable, roundno);
    SDL_SetWindowTitle(window, buf);
}

void UpdateBricks(void)
{


    static float sum = 0;
    bool render = false;
    int k, l, m = 255;
    //check if  ball rect overlaps with either player rect
    SDL_Rect ballRect = {
        .x = ball.x - ball.size / 2,
        .y = ball.y - ball.size / 2,
        .w = ball.size,
        .h = ball.size,
    };
    SDL_Rect bricks[20];
    for (int i = 0; i < 5; i++)
    {
        bricks[i].h = BRICK_HEIGHT;
        bricks[i].w = BRICK_WIDTH;
        bricks[i].x = 50 + 100 * i;
        bricks[i].y = 50;
    }
    for (int i = 5; i < 10; i++)
    {
        bricks[i].h = BRICK_HEIGHT;
        bricks[i].w = BRICK_WIDTH;
        bricks[i].x = 50 + 100 * (i - 5);
        bricks[i].y = 100;
    }
    for (int i = 10; i < 15; i++)
    {
        bricks[i].h = BRICK_HEIGHT;
        bricks[i].w = BRICK_WIDTH;
        bricks[i].x = 50 + 100 * (i - 10);
        bricks[i].y = 150;
    }

    for (int j = 15; j < 20; j++)
    {
        bricks[j].h = BRICK_HEIGHT;
        bricks[j].w = BRICK_WIDTH;
        bricks[j].x = 50 + 100 * (j - 15);
        bricks[j].y = 200;
    }

    for (int i = 0; i < 20; i++)
    {
        if (count[i] != 0 && count > 0)
        {
            render = true;
        }
        if (count[i] <= 0)
        {
            render = false;
        }
        while (render)
        {
            if (SDL_HasIntersection(&ballRect, &bricks[i]))
            {
                count[i]--;
                if (ball.ySpeed > 0)
                {
                    ball.ySpeed = -fabs(ball.ySpeed);
                }
                else
                {
                    ball.ySpeed = fabs(ball.ySpeed);
                }
            }
            break;
        }
        while (render)
        {
            k = i >= 5&&i<10 ?  -255: 0;
            l=i>=10&&i<15?-255:0;
            m=i>=10?-255:0;
            SDL_SetRenderDrawColor(renderer, 255+ k, 255 + l, 255 + m, 255);
            SDL_RenderFillRect(renderer, &bricks[i]);
            break;
        }
        if (count[i] < 0)
        {
            count[i] = 0;
        }
    }

    if (count[1] == 0 && count[2] == 0 && count[3] == 0 && count[4] == 0 && count[5] == 0 && count[6] == 0 && count[7] == 0 && count[8] == 0 && count[9] == 0 && count[0] == 0 &&
        count[10] == 0 && count[12] == 0 && count[13] == 0 && count[14] == 0 && count[15] == 0 && count[16] == 0 && count[17] == 0 && count[18] == 0 && count[19] == 0 && count[11] == 0)
    {
        UpdateScore(2, 100);
    }
}

void RestartGame(void)

{  
   
 SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, "OOOPS!YOU FAILED", textColor);
   SDL_Texture *mTexture =SDL_CreateTextureFromSurface(renderer,textSurface);
     gFont=TTF_OpenFont("arial.ttf",18);
    SDL_RenderClear(renderer);
    SDL_Rect dest={.x=WIDTH/2-(textSurface->w)/2,.y=HEIGHT/2,.w=textSurface->w,.h=textSurface->h};
    SDL_RenderFillRect(renderer,&dest);
    SDL_RenderCopy(renderer,mTexture,NULL,&dest);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_free(textSurface);
    SDL_DestroyTexture(mTexture);
    served = false;
    player2.score = 0;
    roundno = 1;
    for (int i = 0; i < 20; i++)
    {
        count[i] = 1;
    }
    PLAYER_MOVE_SPEED = 150.0f;
    SPEED = 140;

    char *pointsTable = "*****************************************Brick Breaker**************************************";
    int len = snprintf(NULL, 0, pointsTable);
    char buf[len + 1];
    snprintf(buf, len + 1, pointsTable);
    SDL_SetWindowTitle(window, buf);
}
void call(void)
{ 
       gFont=TTF_OpenFont("arial.ttf",18);
    SDL_Surface*textSurface = TTF_RenderText_Solid(gFont, "WELCOME!", textColor);
     SDL_Texture *mTexture =SDL_CreateTextureFromSurface(renderer,textSurface);
    SDL_Rect dest={.x=WIDTH/2-(textSurface->w)/2,.y=HEIGHT/2,.w=textSurface->w,.h=textSurface->h};
    SDL_RenderFillRect(renderer,&dest);
    SDL_RenderCopy(renderer,mTexture,NULL,&dest);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_free(textSurface);
    SDL_DestroyTexture(mTexture);
    SDL_Surface*Surface = TTF_RenderText_Solid(gFont, "PRESS 'f' to SKIP A LEVEL and SPACE TO BEGIN", textColor);
     SDL_Texture *Texture =SDL_CreateTextureFromSurface(renderer,Surface);
    SDL_Rect Ndest={.x=WIDTH/2-(Surface->w)/2,.y=HEIGHT/2,.w=Surface->w,.h=Surface->h};
    SDL_RenderFillRect(renderer,&Ndest);
    SDL_RenderCopy(renderer,Texture,NULL,&Ndest);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_free(Surface);
    SDL_DestroyTexture(Texture);
    
}