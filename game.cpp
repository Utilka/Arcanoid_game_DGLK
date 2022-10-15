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
Sprite* background;
Block blockList[NUMBER_OF_BLOCKS];
Sprite *Block::blockSprite[4];
Platform player;
Sprite *Platform::sprite;
Ball ball;
Sprite *Ball::sprite;

Ability AbilityList[NUMBER_OF_ABILITIES_ON_SCREEN];
Sprite *Ability::abilitySprite[3];


int playerPos_x, playerPos_y;
bool playerMoveRight, playerMoveLeft;
int playingWidth;

int mousePos_x, mousePos_y;


/* Test Framework realization */
class MyFramework : public Framework {

public:


    virtual void PreInit(int &width, int &height, bool &fullscreen) {
        width = 800;
        height = 600;
        fullscreen = false;
    }

    virtual bool Init() {
        int w_w, w_h;//window_width window_height
        getScreenSize(w_w, w_h);
        playingWidth = w_w;
        background = createSprite("./data/66-backgroud.jpg");
        setSpriteSize(background,w_w,w_h);


        Platform::sprite = createSprite("./data/50-Breakout-Tiles.png");
        int playerWidth = w_w * RELATIVE_PLATFORM_WIDTH;
        int playerHeight = w_w * RELATIVE_PLATFORM_WIDTH * PLATFORM_DIMENSIONS;
        setSpriteSize(Platform::sprite,playerWidth,playerHeight);
        player = Platform(w_w / 2 - playerWidth / 2, w_h - w_h/20 - playerHeight,playerWidth,playerHeight);


        Ball::sprite = createSprite("./data/65-amogus.png");
        int ballWidth = w_w / 30;
        int ballHeight = ballWidth;
        setSpriteSize(Ball::sprite,ballWidth,ballHeight);
        ball = Ball(w_w / 2 - ballWidth / 2,w_h/2-ballHeight/2,ballWidth,ballHeight);



        // init game blocks
        int blockWidth = w_w / NUMBER_OF_BLOCKS_IN_A_ROW;
        int rowHeight = blockWidth * BLOCK_DIMENSIONS;
        int topPadding = w_h / TOP_PADDING_WINDOW_FRACTION;

        Block::blockSprite[0]= createSprite("data/0-Block.png");
        setSpriteSize(Block::blockSprite[0],blockWidth,rowHeight);
        Block::blockSprite[1]= createSprite("data/1-Block.png");
        setSpriteSize(Block::blockSprite[1],blockWidth,rowHeight);
        Block::blockSprite[2]= createSprite("data/2-Block.png");
        setSpriteSize(Block::blockSprite[2],blockWidth,rowHeight);
        Block::blockSprite[3]= createSprite("data/3-Block.png");
        setSpriteSize(Block::blockSprite[3],blockWidth,rowHeight);

//        //example of solid wall spawn
//        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
//            // lowest 4 bits of i denote column index, rest bits denote row index
//            blockList[i] = Block(15, 1,
//                                 blockWidth * (i & 15), topPadding + rowHeight * ((i >> 4)),
//                                 blockWidth, rowHeight);
//
//        }

//INT_MAX
        //example of checkers spawn
        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
            // lowest 4 bits of i denote column index, rest bits denote row index
            blockList[i] = Block(15, (i & 1) +1,
                                 blockWidth * (i & 15), topPadding + rowHeight * (2 * (i >> 4) + (i & 1)),
                                 blockWidth, rowHeight);

        }

        int AbilityWidth = w_w * RELATIVE_ABILITY_WIDTH;
        int AbilityHeight = AbilityWidth * ABILITY_DIMENSIONS;
        Ability::abilitySprite[0]= createSprite("data/0-Block.png");
        setSpriteSize(Ability::abilitySprite[0],AbilityWidth,AbilityHeight);
        Ability::abilitySprite[1]= createSprite("data/41-Breakout-Tiles.png");
        setSpriteSize(Ability::abilitySprite[1],AbilityWidth,AbilityHeight);
        Ability::abilitySprite[2]= createSprite("data/42-Breakout-Tiles.png");
        setSpriteSize(Ability::abilitySprite[2],AbilityWidth,AbilityHeight);

        AbilityList[0]=Ability(0,w_w/2 - AbilityWidth/2,w_h/2 - AbilityHeight/2,AbilityWidth, AbilityHeight);
        AbilityList[1]=Ability(1,w_w/2 - AbilityWidth/2,w_h/2 - AbilityHeight/2,AbilityWidth, AbilityHeight);
        AbilityList[2]=Ability(0,w_w/2 - AbilityWidth/2,w_h/2 - AbilityHeight/2,AbilityWidth, AbilityHeight);
        AbilityList[3]=Ability(1,w_w/2 - AbilityWidth/2,w_h/2 - AbilityHeight/2,AbilityWidth, AbilityHeight);

        AbilityList[0].spawn(360+20,290+20);
        AbilityList[1].spawn(360-20,290+20);
        AbilityList[2].spawn(360+20,290-20);
        AbilityList[3].spawn(360-20,290-20);

        return true;


    }

    virtual void Close() {
    }

    virtual bool Tick() {
        drawSprite(background,0,0);
        // modify player position, but limit it between 0 and playingWidth-player.size_x
        player.l_x = max(min(
                player.l_x + playerMoveRight * 1 - playerMoveLeft * 1,
                double(playingWidth - player.s_x)), 0.0);
        ball.l_x += ball.getSpeed_x();
        ball.l_y += ball.getSpeed_y();

        ball.checkAllCollisions(&player,blockList,NUMBER_OF_BLOCKS);

        for ( Ability &ability: AbilityList) {
            ability.l_x += ability.getSpeed_x();
            ability.l_y += ability.getSpeed_y();
        }

//
//        drawSprite(blockList[0].sprite,0,0);
//        drawSprite(blockList[0].sprite,200,200);
//		drawTestBackground();
        for (Ability &ability: AbilityList) { ability.checkAllCollisions(&player); }

        for (Ability &ability: AbilityList) { ability.Draw(); }
        for (Block &block: blockList) { block.Draw(); }
        ball.Draw();
        player.Draw();
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
