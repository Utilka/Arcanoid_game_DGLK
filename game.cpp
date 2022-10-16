#include "Framework.h"
#include "GameObject.h"
#include <algorithm>
#include <iostream>
//#include <cstddef>
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


//allocation of space for game objects and their sprites
Sprite *GameObject::sprite;
Sprite *background;
Block blockList[NUMBER_OF_BLOCKS];
Sprite *Block::blockSprite[4];
Platform player;
Sprite *Platform::platformSprites[3];
Ball ball;
Sprite *Ball::ballSprites[36];


Ability AbilityList[NUMBER_OF_ABILITIES_ON_SCREEN];
Sprite *Ability::abilitySprite[3];

int playerPos_x, playerPos_y;
bool playerMoveRight, playerMoveLeft;
int playingWidth;

int mousePos_x, mousePos_y;
int screenWidth, screenHeight;


/* Test Framework realization */
class MyFramework : public Framework {

public:


    virtual void PreInit(int &width, int &height, bool &fullscreen) {
        width = 800;
        height = 600;
        fullscreen = false;
    }

    virtual bool Init() {
        getScreenSize(screenWidth, screenHeight);
        playingWidth = screenWidth;
        background = createSprite("./data/66-background.jpg");
        setSpriteSize(background, screenWidth, screenHeight);


        int playerWidth = screenWidth * RELATIVE_PLATFORM_WIDTH;
        int playerHeight = screenWidth * RELATIVE_PLATFORM_WIDTH * PLATFORM_DIMENSIONS;

        Platform::initSprites(playerWidth,playerHeight) ;
        player = Platform(screenWidth / 2 - playerWidth / 2,
                          screenHeight - screenHeight / 20 - playerHeight,
                          playerWidth,
                          playerHeight);


        int ballWidth = screenWidth / 20;
        int ballHeight = ballWidth;

        Ball::initSprites(ballWidth,ballHeight) ;

        ball = Ball(screenWidth / 2 - ballWidth / 2,
                    screenHeight / 2 - ballHeight / 2,
                    ballWidth,
                    ballHeight);



        int blockWidth = screenWidth / NUMBER_OF_BLOCKS_IN_A_ROW;
        int rowHeight = blockWidth * BLOCK_DIMENSIONS;
        int topPadding = screenHeight / TOP_PADDING_WINDOW_FRACTION;

        Block::initSprites(blockWidth,rowHeight) ;

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
            blockList[i] = Block(15, (i & 1) + 1, //INT_MAX
                                 blockWidth * (i & 15), topPadding + rowHeight * (2 * (i >> 4) + (i & 1)),
                                 blockWidth, rowHeight);

        }

        int AbilityWidth = screenWidth * RELATIVE_ABILITY_WIDTH;
        int AbilityHeight = AbilityWidth * ABILITY_DIMENSIONS;

        Ability::initSprites(AbilityWidth,AbilityHeight);

        AbilityList[0] = Ability(0, screenWidth / 2 - AbilityWidth / 2, screenHeight / 2 - AbilityHeight / 2, AbilityWidth,
                                 AbilityHeight);
        AbilityList[1] = Ability(1, screenWidth / 2 - AbilityWidth / 2, screenHeight / 2 - AbilityHeight / 2, AbilityWidth,
                                 AbilityHeight);
        AbilityList[2] = Ability(0, screenWidth / 2 - AbilityWidth / 2, screenHeight / 2 - AbilityHeight / 2, AbilityWidth,
                                 AbilityHeight);
        AbilityList[3] = Ability(1, screenWidth / 2 - AbilityWidth / 2, screenHeight / 2 - AbilityHeight / 2, AbilityWidth,
                                 AbilityHeight);

        AbilityList[0].spawn(360 + 20, 290 + 20);
        AbilityList[1].spawn(360 - 20, 290 + 20);
        AbilityList[2].spawn(360 + 20, 290 - 20);
        AbilityList[3].spawn(360 - 20, 290 - 20);

        return true;
    }

    virtual void Close() {
    }

    virtual bool Tick() {
        drawSprite(background, 0, 0);
        // modify player position, but limit it between 0 and playingWidth-player.size_x
        player.l_x = max(min(
                player.l_x + playerMoveRight * 1 - playerMoveLeft * 1,
                double(playingWidth - player.s_x)), 0.0);
        ball.l_x += ball.getSpeed_x();
        ball.l_y += ball.getSpeed_y();

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
        }else{
            ball.launch(screenWidth/2, screenHeight/2);
        }
        ball.draw();

        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
        mousePos_x = x;
        mousePos_y = y;

    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
        if (!isReleased) {
            ball.launch(mousePos_x, mousePos_y);
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
    return run(new MyFramework);
}
