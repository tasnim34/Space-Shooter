#include "iGraphics.h"
#include <math.h>
#define MAX_BULLETS 100000
#define bg_slices 15
#define bg_slice_height 40
#define screen_width  1000
#define screen_height 600
#define MAX_ALIENS 30
#define alienShip_TYPES 28
# define boss_number 1
# define boss1_height 256
# define boss1_bullet_number 200
# define boss1_bullet_width 30
# define boss1_bullet_height 63
#define MAX_BOSS_BULLETS 100
#define GAMESTATE_WIN 6

int alienX[MAX_ALIENS];
int alienY[MAX_ALIENS];
int alienDelay[MAX_ALIENS];
int alienType[MAX_ALIENS];
bool alienAlive[MAX_ALIENS];
Image alienImages[alienShip_TYPES];

int spaceshipX = 250;
int spaceshipY = 3;
int spaceshipWidth = 50;   // adjust size as needed
int spaceshipHeight = 10;
Image ship1;


Image bg2;
Image bullet1;
int bulletX[MAX_BULLETS];
int bulletY[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
float bulletAngle[MAX_BULLETS];
int bulletCount = 0;


bool explosionActive[MAX_ALIENS];
int explosionTimer[MAX_ALIENS];
int explosionX[MAX_ALIENS];
int explosionY[MAX_ALIENS];
bool alienDeadPermanently[MAX_ALIENS];
int alienHitCount[MAX_ALIENS];
Image explosion;



int playerHealth = 50;
int score = 0;



bool bossActive = false;
int bossX = 300, bossY = screen_height + 100;
int bossSpeedX = 4;
int bossSpeedY = 2;
float bossWaveAngle = 0;
float bossWaveSpeed = 0.05;
bool bossMovingRight = true;
bool bossMovingUp = false;
bool bossEntering = true;
int health_boss1 = 100;
int bossHitCount = 0;



Image bossImage;



int bossBulletX[MAX_BOSS_BULLETS];
int bossBulletY[MAX_BOSS_BULLETS];
bool bossBulletActive[MAX_BOSS_BULLETS];
int bossBulletCount = 0;
Image bossBulletImage;


int gameState = 0; // 0 = intro, 1 = menu, 2 = game

Image startIcon, aboutIcon, leaderboardIcon, exitIcon,menuBackground,introImage,gameOverImage,aboutImage,winImage,backIcon,continueIcon;
int iconWidth = 150, iconHeight = 150;
int startWidth = 200, startHeight = 60;
// Positions for icons on the menu screen
int startX = 400, startY = 310;
int aboutX = 400, aboutY = 210;
int leaderboardX = 400, leaderboardY = 110;
int exitX = 400, exitY = 10;
bool menuShown = false;
int continueBtnX = (screen_width / 2) - (iconWidth / 2);
int continueBtnY = 20;
int continueBtnWidth = 200;
int continueBtnHeight = 60;

bool gameOverTriggered = false;
int backIconX = 10, backIconY = screen_height - 60;
int backIconWidth = 50, backIconHeight = 50;

char bgimages[bg_slices][40] = {"renderBG\\tile014.bmp", "renderBG\\tile013.bmp", "renderBG\\tile012.bmp","renderBG\\tile011.bmp", "renderBG\\tile010.bmp", "renderBG\\tile009.bmp", "renderBG\\tile008.bmp",	"renderBG\\tile007.bmp",	"renderBG\\tile006.bmp",	"renderBG\\tile005.bmp",	"renderBG\\tile004.bmp",	"renderBG\\tile003.bmp",	"renderBG\\tile002.bmp",	"renderBG\\tile001.bmp",	"renderBG\\tile014.bmp"};
int bg_y[bg_slices];

char alienImageFiles[alienShip_TYPES][30] = {"alien1.png", "alien2.png", "alien3.png", "alien4.png", "alien5.png", "alien6.png", "alien7.png","alien8.png", "alien9.png", "alien10.png", "alien11.png", "alien12.png", "alien13.png", "alien14.png","alien15.png", "alien16.png", "alien17.png", "alien18.png", "alien19.png", "alien20.png", "alien21.png", "alien22.png", "alien23.png", "alien24.png", "alien25.png", "alien26.png","alien27.png", "alien28.png"};

void initialize_bg_y(void)
{
	int sum = 0;
	for (int i = 0; i < bg_slices; i++)
	{
		bg_y[i] = sum;
		sum += bg_slice_height;
	}
}


void loadResources()
{



    for (int i = 0; i < alienShip_TYPES; i++) {
    iLoadImage(&alienImages[i], alienImageFiles[i]);
    iScaleImage(&alienImages[i], 0.4);
}

iLoadImage(&bossImage, "boss.png");
iResizeImage(&bossImage, 150, 150);

iLoadImage(&bossBulletImage, "boss_bullet.png");
iScaleImage(&bossBulletImage, 0.2);


    iLoadImage(&ship1, "ship1.png");
	iLoadImage(&bullet1, "bullet1.png");

	double ship1AspectRatio = (double)ship1.width / ship1.height;
	iResizeImage(&ship1, 70, 70.0/ship1AspectRatio);

	iScaleImage(&bullet1,0.15);
	iLoadImage(&explosion, "explosion.png");
iScaleImage(&explosion, 0.4);

 iLoadImage(&startIcon, "1-start.png");
    iResizeImage(&startIcon, iconWidth, iconHeight);

    iLoadImage(&aboutIcon, "about.png");
    iResizeImage(&aboutIcon, iconWidth, iconHeight);

    iLoadImage(&leaderboardIcon, "leaderboard.png");
    iResizeImage(&leaderboardIcon, iconWidth, iconHeight);

    iLoadImage(&exitIcon, "3-exit.png");
    iResizeImage(&exitIcon, iconWidth, iconHeight);

    iLoadImage(&menuBackground, "menu_background.png");
    iResizeImage(&menuBackground, screen_width, screen_height);

    iLoadImage(&introImage, "introImage.png");
    iResizeImage(&introImage, screen_width, screen_height);

    iLoadImage(&gameOverImage, "game_over.png");  // Make sure you have this image file
    iResizeImage(&gameOverImage, screen_width, screen_height);



    iLoadImage(&aboutImage, "about_message.png");

iResizeImage(&aboutImage, screen_width, screen_height);

iLoadImage(&backIcon, "back.png");
iResizeImage(&backIcon, backIconWidth, backIconHeight);

iLoadImage(&winImage, "wining_message.png");
iResizeImage(&winImage, screen_width, screen_height);

    iLoadImage(&continueIcon, "continue.png");
    iResizeImage(&continueIcon,iconWidth, iconHeight);

}

void checkPlayerHealth() {
    if (gameState != 2) return;  // Only check health during actual gameplay

    if (playerHealth <= 0 && !gameOverTriggered) {
        playerHealth = 0;
        gameState = 5; // Game over screen
        gameOverTriggered = true;
    }
}





void initializeAliens() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        alienX[i] = rand() % (screen_width - 50);
        alienY[i] = screen_height + i * 100;
        alienAlive[i] = false;
        alienDelay[i] = i * 80;
    alienType[i] = rand() % alienShip_TYPES;  // enemy type (0,1,2)
    explosionActive[i] = false;
    explosionX[i] = 0;
explosionY[i] = 0;
explosionTimer[i] = 0;
alienHitCount[i] = 0;
alienDeadPermanently[i] = false;

    }

}

void moveAliens() {

    if (gameState != 2) return; // Only move aliens during gameplay
    for (int i = 0; i < MAX_ALIENS; i++) {
            if (alienDeadPermanently[i]) {
            // Alien is dead forever
            continue;
        }

        if (!alienAlive[i]) {
            alienDelay[i]--;
            if (alienDelay[i] <= 0) {
                alienAlive[i] = true;
            }
            continue;
        }

        alienY[i] -= 4;

        if (alienY[i] < -50) {
            alienY[i] = screen_height + rand() % 200;
            alienX[i] = rand() % (screen_width - 50);
            alienAlive[i] = false;
            alienDelay[i] = 100 + rand() % 200;
            alienType[i] = rand() % alienShip_TYPES;
        }

    }
    bool allAliensDead = true;
for (int i = 0; i < MAX_ALIENS; i++) {
    if (!alienDeadPermanently[i]) {
        allAliensDead = false;
        break;
    }
}

if (allAliensDead && !bossActive) {
    bossActive = true;
    bossX = 400;
    bossY = screen_height + 100;

}
for (int i = 0; i < MAX_ALIENS; i++) {
    if (alienAlive[i]) {

        //Touches player
        if (alienX[i]+50 > spaceshipX && alienX[i] < spaceshipX +spaceshipWidth &&
    alienY[i]+50 > spaceshipY && alienY[i] < spaceshipY+ spaceshipHeight) {
    playerHealth -= 10;
    alienAlive[i] = false;
    alienDeadPermanently[i] = true; // <--- Mark alien as permanently dead
explosionActive[i] = true;
    explosionX[i] = alienX[i];
    explosionY[i] = alienY[i];
    explosionTimer[i] = 10;



}


        //Escapes screen
        if (alienY[i] < 0) {
    playerHealth -= 5;
    alienAlive[i] = false;
    alienDeadPermanently[i] = true;
}


    }
}
checkPlayerHealth();

}

//void startGame() {
//    gameState = 2; // Game running
//    initializeAliens();
//    score = 0;
//    playerHealth = 100;
//    bossActive = false;
//    bossX = 300;
//    bossY = screen_height + 100;
//    bossEntering = true;
//    // Initialize other variables as needed
//}


  void startGame() {
    gameState = 2; // Switch to game running state

    initializeAliens();  // Initialize aliens first

    // Reset player
    spaceshipX = 250;
    spaceshipY = 3;
    playerHealth = 100;

    // Reset bullets
    for (int i = 0; i < bulletCount; i++) {
        bulletActive[i] = false;
    }
    bulletCount = 0;

    // Reset aliens (redundant since initializeAliens() does this, but kept for completeness)
    for (int i = 0; i < MAX_ALIENS; i++) {
        alienX[i] = rand() % (screen_width - 50);
        alienY[i] = screen_height + i * 100;
        alienAlive[i] = false;
        alienDelay[i] = i * 80;
        alienType[i] = rand() % alienShip_TYPES;
        explosionActive[i] = false;
        explosionTimer[i] = 0;
        alienDeadPermanently[i] = false;
        alienHitCount[i] = 0;
    }

    // Reset boss
    bossActive = false;
    bossX = 300;
    bossY = screen_height + 100;
    bossSpeedX = 4;
    bossSpeedY = 2;
    bossWaveAngle = 0;
    bossEntering = true;
    health_boss1 = 500;
    bossHitCount = 0;

    // Reset boss bullets
    for (int i = 0; i < bossBulletCount; i++) {
        bossBulletActive[i] = false;
    }
    bossBulletCount = 0;

    // Reset score and flags
    score = 0;
    gameOverTriggered = false;
}



void returnToMenu() {
    gameState = 1;
    menuShown = true;
    gameOverTriggered = false;
    playerHealth = 100;
    score = 0;

    // Reset all game objects
    for (int i = 0; i < MAX_BULLETS; i++) bulletActive[i] = false;
    bulletCount = 0;

    for (int i = 0; i < MAX_ALIENS; i++) {
        alienAlive[i] = false;
        explosionActive[i] = false;
    }

    bossActive = false;
    bossBulletCount = 0;
}






/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    iClear();

    if (gameState == 0) {
        // Intro screen
        iShowLoadedImage(0, 0, &introImage);

    }
    else if (gameState == 1) {
        // Menu screen
        iShowLoadedImage(0, 0, &menuBackground);



        iShowLoadedImage(startX, startY, &startIcon);
        iShowLoadedImage(aboutX, aboutY, &aboutIcon);
        iShowLoadedImage(leaderboardX, leaderboardY, &leaderboardIcon);
        iShowLoadedImage(exitX, exitY, &exitIcon);

    }
    else if (gameState == 2) {
        // Game running screen - show game objects here


        // Background slices
        for (int i = 0; i < bg_slices; i++) {
            iShowImage(0, bg_y[i], bgimages[i], screen_width, bg_slice_height);
        }

        // Show spaceship
        iShowLoadedImage(spaceshipX, spaceshipY, &ship1);

        // Show bullets
        for (int i = 0; i < bulletCount; i++) {
            if (bulletActive[i]) {
                iShowLoadedImage(bulletX[i], bulletY[i], &bullet1);
            }
        }

        // Show aliens
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (alienAlive[i] && !explosionActive[i]) {
                iShowLoadedImage(alienX[i], alienY[i], &alienImages[alienType[i]]);
            }
        }

        // Show explosions
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (explosionActive[i]) {
                iShowLoadedImage(explosionX[i], explosionY[i], &explosion);
                explosionTimer[i]--;
                if (explosionTimer[i] <= 0) {
                    explosionActive[i] = false;
                }
            }
        }

        // Show boss if active
        if (bossActive) {
            iShowLoadedImage(bossX, bossY, &bossImage);
            char statusText[100];
sprintf(statusText, "Boss Health: %d", health_boss1);
iSetColor(255, 255, 255);
iText(700, screen_height - 30, statusText, GLUT_BITMAP_HELVETICA_18);
            for (int i = 0; i < bossBulletCount; i++) {
                if (bossBulletActive[i]) {
                    iShowLoadedImage(bossBulletX[i], bossBulletY[i], &bossBulletImage);
                }
            }

//            // Boss health bar
//            iSetColor(255, 0, 0);
//            iFilledRectangle(0, 570, 1000, 20);  // Red background bar
//
//            iSetColor(0, 255, 0);
//            int greenBarWidth = (int)(300.0 * health_boss1 / 1000.0); // boss health scale
//            iFilledRectangle(0, 570, greenBarWidth, 20);
        }

        // Show score and health
        char statusText[100];
sprintf(statusText, "Score: %d ", score );
iSetColor(255, 255, 255);  // White color
iText(10, screen_height - 30, statusText, GLUT_BITMAP_HELVETICA_18);
//char statusText[100];
sprintf(statusText, "Health: %d ",playerHealth );
iSetColor(255, 255, 255);  // White color
iText(10, screen_height - 50, statusText, GLUT_BITMAP_HELVETICA_18);
    }
    else if (gameState == 3) {
        // About screen, draw your about text or images here
         iShowLoadedImage(0, 0, &aboutImage);
         iShowLoadedImage(backIconX, backIconY, &backIcon);
    }

    else if (gameState == 4) {
        // Leaderboard screen, draw leaderboard here


}
else if (gameState == 5) {
   iShowLoadedImage(0, 0, &gameOverImage);
   iShowLoadedImage(continueBtnX, continueBtnY, &continueIcon);

}

else if (gameState == GAMESTATE_WIN) {
    iShowLoadedImage(0, 0, &winImage);
//    iShowLoadedImage(continueBtnX, continueBtnY, &continueIcon);

}



}

void changeBG(void)
{
    for (int i = 0; i < bg_slices; i++)
    {
        if (bg_y[i] < bg_slice_height)
        {
            bg_y[i] = screen_height - bg_slice_height;
        }
        else
        {
            bg_y[i] -= bg_slice_height;
        }
    }
}


/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here

    // Horizontal movement
    spaceshipX=mx-32;
     if (spaceshipX+12 > iScreenWidth || spaceshipX-12 < 0)
    {
        if (spaceshipX+12 > iScreenWidth)
            spaceshipX = iScreenWidth - 12;
        else
            spaceshipX = -8;

    }
     // Limit vertical movement (from Y = 0 to Y = 150)
    if (my >= 0 && my <= 250)
        spaceshipY = my;

  }


/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
       printf("Clicked at: %d %d | gameState = %d\n", mx, my, gameState);

        // Game Over or Win screen continue button
         if (gameState == 5 /*|| gameState == GAMESTATE_WIN*/) {
    if (mx >= continueBtnX &&
        mx <= continueBtnX + iconWidth &&
        my >= continueBtnY &&
        my <= continueBtnY + iconHeight)
    {
        printf("Continue clicked! X[%d-%d] Y[%d-%d]\n",
               continueBtnX, continueBtnX + iconWidth,
               continueBtnY, continueBtnY + iconHeight);
        returnToMenu();
        return;
    }
}

        //Check Main Menu buttons
        if (gameState == 1)
        {
            if (mx >= startX && mx <= startX + iconWidth &&
                my >= startY && my <= startY + iconHeight) {
                startGame(); // change gameState
                return;
            }

           if (mx >= aboutX && mx <= aboutX + iconWidth &&
                my >= aboutY && my <= aboutY + iconHeight) {
                gameState = 3; // About screen
                return;
            }

            if (mx >= leaderboardX && mx <= leaderboardX + iconWidth &&
                my >= leaderboardY && my <= leaderboardY + iconHeight) {
                gameState = 4; // Leaderboard screen
                return;
            }

            if (mx >= exitX && mx <= exitX + iconWidth &&
                my >= exitY && my <= exitY + iconHeight) {
                exit(0);
            }
        }

if ((gameState == 3 || gameState == 4) &&
            mx >= backIconX && mx <= backIconX + backIconWidth &&
            my >= backIconY && my <= backIconY + backIconHeight) {
            gameState = 1; // Return to menu
            return;
        }

    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // Right click logic (optional)
    }
}









/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key, int state)
{
    if (key == ' ') {
        if (bulletCount + 5 < MAX_BULLETS) {
            // Bullet 1
            bulletX[bulletCount] = spaceshipX + 30;
            bulletY[bulletCount] = spaceshipY + 20;
            bulletAngle[bulletCount] = 90;
            bulletActive[bulletCount] = true;
            bulletCount++;

            // Bullet 2
            bulletX[bulletCount] = spaceshipX + 30;
            bulletY[bulletCount] = spaceshipY + 20;
            bulletAngle[bulletCount] = 100;
            bulletActive[bulletCount] = true;
            bulletCount++;

            // Bullet 3
            bulletX[bulletCount] = spaceshipX + 30;
            bulletY[bulletCount] = spaceshipY + 20;
            bulletAngle[bulletCount] = 75;
            bulletActive[bulletCount] = true;
            bulletCount++;

            // Bullet 4
            bulletX[bulletCount] = spaceshipX + 30;
            bulletY[bulletCount] = spaceshipY + 20;
            bulletAngle[bulletCount] = 110;
            bulletActive[bulletCount] = true;
            bulletCount++;

            // Bullet 5
            bulletX[bulletCount] = spaceshipX + 30;
            bulletY[bulletCount] = spaceshipY + 20;
            bulletAngle[bulletCount] = 65;
            bulletActive[bulletCount] = true;
            bulletCount++;
        }
    }

    if (key == 'q' || key == 'Q') {
        exit(0);
    }

    if (gameState == GAMESTATE_WIN && (key == 'p' || key == 'P')) {
        returnToMenu();  // Return to main menu
        gameState = 1; // Reset game state
        returnToMenu();        //start a fresh game
    }



}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key, int state)
{
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

void bulletChange(void)
{



   for (int i = 0; i < bulletCount; i++) {
    if (!bulletActive[i]) continue;

    float rad = bulletAngle[i] * 3.1415926 / 180.0; // Convert degree to radian

    bulletX[i] += cos(rad) * 10;  // X moves by cosine
    bulletY[i] += sin(rad) * 10;  // Y moves by sine

    if (bulletY[i] > screen_height || bulletX[i] < 0 || bulletX[i] > screen_width) {
        bulletActive[i] = false;
        continue;
    }


    for (int j = 0; j < MAX_ALIENS; j++) {
        if (alienAlive[j]) {
            if (bulletX[i] < alienX[j] + 50 && bulletX[i] + 10 > alienX[j] &&
                bulletY[i] < alienY[j] + 50 && bulletY[i] + 10 > alienY[j]) {

                bulletActive[i] = false;          // Bullet disappears
                alienHitCount[j]++;               // Hit count +1

                if (alienHitCount[j] >= 2) {      // If hit twice
                    alienAlive[j] = false;        // Alien dies
                    alienHitCount[j] = 0;         // Reset hit count
                    alienDeadPermanently[j] = true;
                    score += 10;
                    // Trigger explosion
                    explosionActive[j] = true;
                    explosionX[j] = alienX[j];
                    explosionY[j] = alienY[j];
                    explosionTimer[j] = 10;
                }
                break;  // Exit loop after hit
            }
        }
    }

    if (bossActive) {
            if (bulletX[i] < bossX + 150 && bulletX[i] + 10 > bossX &&
                bulletY[i] < bossY + 150 && bulletY[i] + 10 > bossY) {
                bulletActive[i] = false;
score += 10;

            bossHitCount++;
        if (bossHitCount >= 1) {
            health_boss1--;             // reduce health by 1
            bossHitCount = 0;           // reset hit counter
        }

        if (health_boss1 <= 0) {
            health_boss1 = 0;
            bossActive = false;
            gameState = GAMESTATE_WIN;  // Show game over or victory screen
        }
}
        }
}
}


void updateBoss() {


    if (gameState != 2) return; // Only update boss during gameplay
    if (!bossActive) return;


    if (bossY > screen_height - 300) {
        bossY -= 2;
    }
   // Zig-Zag horizontal movement
    if (bossMovingRight) {
        bossX += bossSpeedX;
        if (bossX + 150 >= screen_width) {
            bossX = screen_width - 150;
            bossMovingRight = false;
        }
    } else {
        bossX -= bossSpeedX;
        if (bossX <= 0) {
            bossX = 0;
            bossMovingRight = true; // Hit left wall → go right
        }
    }

    //Random direction switch (for randomness if you want)
    if (rand() % 200 < 2) {
        bossMovingRight = !bossMovingRight;
    }

 if (bossEntering) {
        bossY -= 2;  // descend slowly
        if (bossY <= screen_height - 250) {
            bossY = screen_height - 250;
            bossEntering = false;
        }
    } else {
        // 2. After entrance → Wave vertical movement
        bossWaveAngle += bossWaveSpeed;
        bossY = (screen_height - 250) + sin(bossWaveAngle) * 100;
    }










    static int bossFireTimer = 0;
    bossFireTimer++;
    if (bossFireTimer > 40 && bossBulletCount < MAX_BOSS_BULLETS) {
        bossBulletX[bossBulletCount] = bossX + 70;
        bossBulletY[bossBulletCount] = bossY;
        bossBulletActive[bossBulletCount] = true;
        bossBulletCount++;
        bossFireTimer = 0;
    }




    for (int i = 0; i < bossBulletCount; i++) {
        if (bossBulletActive[i]) {
            bossBulletY[i] -= 6;
            if (bossBulletY[i] < 0) {
                bossBulletActive[i] = false;
            }
        }
    }


    for (int i = 0; i < bossBulletCount; i++) {
    if (bossBulletActive[i]) {
        if (bossBulletX[i] < spaceshipX + 70 && bossBulletX[i] + 20 > spaceshipX &&
            bossBulletY[i] < spaceshipY + 70 && bossBulletY[i] + 20 > spaceshipY) {
                 playerHealth -= 20;

bossBulletActive[i] = false;
        }
    }
}
checkPlayerHealth();

}


void switchToMenu() {
    if (gameState == 0 &&!menuShown) {
        gameState = 1;  // Change to menu state
        menuShown = true; // mark as shown
    }
}



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    initialize_bg_y();
    initializeAliens();
    iSetTimer(80, changeBG);
    iSetTimer(7, moveAliens);
    iSetTimer(30, updateBoss);
    loadResources();
    iSetTimer(20,bulletChange);
    iSetTimer(2000, switchToMenu);

    // place your own initialization codes here.
    iInitialize(1000,600, "Space Shooter-Save The World");
    return 0;
}
