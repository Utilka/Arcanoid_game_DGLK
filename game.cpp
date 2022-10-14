#include "Framework.h"
#include "GameObject.h"
#include <algorithm>
//#include <cstddef>
using namespace std;
using namespace MyGame;


Platform player;
Ball ball;

int player_pos_x, player_pos_y;
bool playerMoveRight, playerMoveLeft;
int playing_width;

int mouse_pos_x, mouse_pos_y;


/* Test Framework realization */
class MyFramework : public Framework {

public:


    virtual void PreInit(int &width, int &height, bool &fullscreen) {
        width = 800;
        height = 600;
        fullscreen = false;
    }

    virtual bool Init() {
        int w_w, w_h;
        getScreenSize(w_w, w_h);
        playing_width = w_w;
        player = Platform("./data/50-Breakout-Tiles.png", w_w, w_h);

        ball = Ball("./data/png_transparent_among_us_icon_crewmate_imposter_astronaut_cartoon.png", w_w, w_h);

        // init game blocks

        return true;
    }

    virtual void Close() {

    }

    virtual bool Tick() {
        // modify player position, but limit it between 0 and playing_width-player.size_x
        player.l_x = max(min(
                player.l_x + playerMoveRight * 1 - playerMoveLeft * 1,
                double(playing_width - player.s_x)), 0.0);
        ball.l_x += ball.speed_x;
        ball.l_y += ball.speed_y;

        ball.checkAllCollisions(&player);

//		drawTestBackground();
        ball.Draw();
        player.Draw();
        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
        mouse_pos_x = x;
        mouse_pos_y = y;

    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
        if (!isReleased) {
            ball.Launch(mouse_pos_x, mouse_pos_y);
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
