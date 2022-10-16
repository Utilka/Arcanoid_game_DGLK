#include "Framework.h"
#include "GameObject.h"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace MyGame;

#define RELATIVE_PLATFORM_WIDTH 1/8
#define PLATFORM_DIMENSIONS 128/485
#define BLOCK_DIMENSIONS 128/384
#define NUMBER_OF_BLOCKS 64
#define NUMBER_OF_BLOCKS_IN_A_ROW 16
#define TOP_PADDING_WINDOW_FRACTION 20

#define NUMBER_OF_ABILITIES_ON_SCREEN 4
#define RELATIVE_ABILITY_WIDTH 1/10
#define ABILITY_DIMENSIONS 128/485

#define ABILITY_SPAWN_INTERVAL 20000


//allocation of space for game objects and their sprites
Sprite *GameObject::sprite;
Sprite *background;
Block blockList[NUMBER_OF_BLOCKS];
Sprite *Block::blockSprite[4];
Platform player;
Sprite *Platform::platformSprites[3];
Ball ball;
Sprite *Ball::ballSprites[36];

Sprite *winSprite;
Sprite *loseSprite;
Sprite *restartSprite;


Ability AbilityList[NUMBER_OF_ABILITIES_ON_SCREEN];
Sprite *Ability::abilitySprite[3];

bool playerMoveRight, playerMoveLeft;

int mousePos_x, mousePos_y;
int lastAbilitySpawn;
int lastAbilitySpawnIndex;


/* Test Framework realization */
class MyFramework : public Framework {

public:


    virtual void PreInit(int &width, int &height, bool &fullscreen) {
        width = screenWidth;
        height = screenHeight;
        fullscreen = false;
    }

    virtual bool Init() {

        playerMoveRight = false;
        playerMoveLeft = false;
        mousePos_x = 0;
        mousePos_y = 0;
        gameStart = false;
        gameOver = false;
        winAnimation = false;


        std::srand(std::time(nullptr));
        background = createSprite("./data/66-background.jpg");
        setSpriteSize(background, screenWidth, screenHeight);

        winSprite = createSprite("data/victory.png");
        setSpriteSize(winSprite, screenWidth / 3, screenHeight / 5);
        loseSprite = createSprite("data/game_over.png");
        setSpriteSize(loseSprite, screenWidth / 3, screenHeight / 3);
        restartSprite = createSprite("data/restart.png");
        setSpriteSize(restartSprite, screenWidth / 3, screenHeight / 7);


        int playerWidth = screenWidth * RELATIVE_PLATFORM_WIDTH;
        int playerHeight = screenWidth * RELATIVE_PLATFORM_WIDTH * PLATFORM_DIMENSIONS;

        Platform::initSprites(playerWidth, playerHeight);
        player = Platform(screenWidth / 2 - playerWidth / 2,
                          screenHeight - screenHeight / 20 - playerHeight,
                          playerWidth,
                          playerHeight,
                          1);


        int ballWidth = screenWidth / 20;
        int ballHeight = ballWidth;

        Ball::initSprites(ballWidth, ballHeight);

        ball = Ball(screenWidth / 2 - ballWidth / 2,
                    screenHeight / 2 - ballHeight / 2,
                    ballWidth,
                    ballHeight);


        int blockWidth = screenWidth / NUMBER_OF_BLOCKS_IN_A_ROW;
        int rowHeight = blockWidth * BLOCK_DIMENSIONS;
        int topPadding = screenHeight / TOP_PADDING_WINDOW_FRACTION;

        Block::initSprites(blockWidth, rowHeight);

//        //example of solid wall spawn
//        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
//            // lowest 4 bits of i denote column index, rest bits denote row index
//            blockList[i] = Block(15, 1,
//                                 blockWidth * (i & 15), topPadding + rowHeight * ((i >> 4)),
//                                 blockWidth, rowHeight);
//
//        }

        //example of checkers spawn
        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
            // lowest 4 bits of i denote column index, rest bits denote row index
            blockList[i] = Block(15,
                                 (((i & 1) + 1) * !(((i & 15) == 3) || ((i & 15) == 12))) +
                                 ((((i & 15) == 3) || ((i & 15) == 12)) * INT_MAX), //INT_MAX
                                 blockWidth * (i & 15), topPadding + rowHeight * (2 * (i >> 4) + (i & 1)),
                                 blockWidth, rowHeight);

        }

        int AbilityWidth = screenWidth * RELATIVE_ABILITY_WIDTH;
        int AbilityHeight = AbilityWidth * ABILITY_DIMENSIONS;

        Ability::initSprites(AbilityWidth, AbilityHeight);


        for (int i = 0; i < NUMBER_OF_ABILITIES_ON_SCREEN; i++) {
            AbilityList[i] = Ability((i & 1),
                                     screenWidth / 2 - AbilityWidth / 2,
                                     screenHeight / 2 - AbilityHeight / 2,
                                     AbilityWidth,
                                     AbilityHeight);
        }

        lastAbilitySpawnIndex = 0;

        return true;
    }

    virtual void Close() {
    }

    virtual bool Tick() {
        drawSprite(background, 0, 0);

        player.checkModifiers();
        // modify player position, but limit it between 0 and screenWidth-player.size_x
        player.l_x = max(min(
                player.l_x + playerMoveRight * player.getSpeed_x() - playerMoveLeft * player.getSpeed_x(),
                double(screenWidth - player.s_x)), 0.0);
        ball.l_x += ball.getSpeed_x();
        ball.l_y += ball.getSpeed_y();

        if (((getTickCount() - lastAbilitySpawn) >= ABILITY_SPAWN_INTERVAL) && gameStart) {


            std::cout << lastAbilitySpawnIndex << std::endl;
            //TODO make abilities spawn in near diagonal directions
            AbilityList[lastAbilitySpawnIndex % NUMBER_OF_ABILITIES_ON_SCREEN].spawn(std::rand() % screenWidth,
                                                                                     std::rand() % screenWidth);
            lastAbilitySpawnIndex++;
            lastAbilitySpawn = getTickCount();
        }


        if (!(gameOver || winAnimation)) {
            ball.checkAllCollisions(&player, blockList, NUMBER_OF_BLOCKS);

            for (Ability &ability: AbilityList) {
                ability.l_x += ability.getSpeed_x();
                ability.l_y += ability.getSpeed_y();
            }

            for (Ability &ability: AbilityList) { ability.checkAllCollisions(&player); }

            for (Ability &ability: AbilityList) { ability.draw(); }
            for (Block &block: blockList) { block.draw(); }
            player.draw();
        } else {
            ball.scalingLaunch(screenWidth / 2, screenHeight / 2);
//            Init();
        }

        if (gameOver) {
            drawSprite(loseSprite, screenWidth / 3, 0);
            if ((getTickCount() - gameEndTimer) > 2000) {
                drawSprite(restartSprite, screenWidth / 3, screenHeight - screenHeight / 5);
            }
        }

        if (winAnimation) {
            drawSprite(winSprite, screenWidth / 3, 0);
            if ((getTickCount() - gameEndTimer) > 2000) {
                drawSprite(restartSprite, screenWidth / 3, screenHeight - screenHeight / 5);
            }
        }

        ball.draw();

        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
        mousePos_x = x;
        mousePos_y = y;

    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
        if ((!isReleased) && (!gameStart)) {
            lastAbilitySpawn = getTickCount();
            gameStart = true;
            ball.launch(mousePos_x, mousePos_y);
        }
        if ((!isReleased) && ((gameOver)||winAnimation)) {
            gameOver = false;
            winAnimation = false;
            Init();
        }
    }

    virtual void onKeyPressed(FRKey k) {
        playerMoveRight = (k == FRKey::RIGHT);
        playerMoveLeft = (k == FRKey::LEFT);
    }

    virtual void onKeyReleased(FRKey k) {
        playerMoveRight = (k != FRKey::RIGHT) && playerMoveRight;
        playerMoveLeft = (k != FRKey::LEFT) && playerMoveLeft;
    }

    virtual const char *GetTitle() override {
        return "Arcanoid";
    }
};


int main(int argc, char *argv[]) {


    for (int i = 0; i < argc; ++i) {

        if (string(argv[i]) == "-window") {
            char *widths = strtok(argv[i + 1], "x");
            char *heights = strtok(nullptr, "x");
            screenWidth = atoi(widths);
            screenHeight = atoi(heights);

        }
    }

    if ((screenWidth == 0 )||(screenHeight== 0)) {
        screenWidth = 800;
        screenHeight = 600;
    }
    return run(new MyFramework);
}
