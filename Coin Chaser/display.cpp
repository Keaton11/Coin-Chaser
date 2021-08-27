#include <stdio.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro.h>

#include "header.h"

//function recieves the regular font and a bool which determines if the timer should be reset
//the time is displayed on the top of the screen in minutes, followed by seconds
//the total number of time in the game so far is returned as an integer
int displayTimer(ALLEGRO_FONT *regular_font, bool &reset) {

    //variables are declared
    static int frameNum;    //counts the number of frames that have passed
    static char time[10];   //contains time that is displayed on the screen

    //timer is reset to 0 seconds if necessary
    if (reset) {
        frameNum = 0;
        reset = false;
    }

    //the current frame number is updated
    frameNum++;

    //the time in minutes and seconds is determined from the frame number
    if ((frameNum / 20) % 60 < 10) {
        sprintf(time, "0%d:0%d", (frameNum / 1200), (frameNum / 20) % 60);
    } else {
        sprintf(time, "0%d:%d", (frameNum / 1200), (frameNum / 20) % 60);
    }

    //the time is displayed on the screen
    al_draw_filled_rounded_rectangle(580, 30, 700, 70, 10, 10, Black);
    al_draw_text(regular_font, White, 1280 / 2, 29, ALLEGRO_ALIGN_CENTER, time);
    return frameNum;
}

//function recieves an Allegro event, and four integers representing the coordinates of a box
//the coordinates of the box are used to determine whether the user has clicked the box
//true is returned if the button has been clicked and false is returned if it hasn't
bool button(ALLEGRO_EVENT ev, int x1, int x2, int y1, int y2) {

    //true is returned if the player has clicked the button
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (ev.mouse.button) {
            if (x1 < ev.mouse.x && x2 > ev.mouse.x && y1 < ev.mouse.y && y2 > ev.mouse.y) {
                return true;
            }
        }
    }
    return false;
}

//function recieves an Allegro event
//the function checks whether the close button has been clicked by the user
//true is returned if the exit button was clicked and false is returned if it hasn't
bool checkExit(ALLEGRO_EVENT &ev) {

    //true is returned if the close button has been clicked
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return true;
    }
    return false;
}

//function recieves health, coins, and regular font
//the health bar, number of coins, exit button, and restart button are displayed on the screen
//no values are returned
void displayProgress(int health, int coins, ALLEGRO_FONT *regular_font) {

    //variables are declared
    char healthString[5];   //variable holds health percentage remaining as string
    char healthBar[15];     //variables contains what will be displayed on the health bar
    char coinsString[5];    //variable holds coin humber as string
    char coinsFound[15];    //variable contains the number of coins left

    //health remaining is converted into a string starting with "Health:"
    sprintf(healthString, "%d%%", health);
    strcpy(healthBar, "Health:");
    strcat(healthBar, healthString);

    //coins remaining is converted into a string starting with "Coins:"
    sprintf(coinsString, "%d", coins);
    strcpy(coinsFound, "Coins:");
    strcat(coinsFound, coinsString);

    //the number of coins found is displayed on the screen
    al_draw_filled_rounded_rectangle(30, 30, 165, 70, 10, 10, Black);
    al_draw_text(regular_font, White, 33, 29, ALLEGRO_ALIGN_LEFT, coinsFound);

    //the health bar is displayed on the screen
    al_draw_filled_rectangle(30, 650, 255, 690, Grey);
    al_draw_filled_rectangle(30, 650, 30 + 225 * (health / 100.0), 690, Red);
    al_draw_rectangle(30, 650, 255, 690, Black, 3);
    al_draw_text(regular_font, Black, 40, 649, ALLEGRO_ALIGN_LEFT, healthBar);

    //the restart button is displayed on the screen
    al_draw_filled_rounded_rectangle(1120, 600, 1250, 640, 10, 10, Black);
    al_draw_text(regular_font, White, 1242, 599, ALLEGRO_ALIGN_RIGHT, "Restart");

    //the exit button is displayed on the screen
    al_draw_filled_rounded_rectangle(1180, 650, 1250, 690, 10, 10, Black);
    al_draw_text(regular_font, White, 1242, 649, ALLEGRO_ALIGN_RIGHT, "Exit");

}

//function recieves the frame number and power up time
//the function determines what position the player's image should be in
//the position the player should be in is returned as an integer
int displayPlayerPos(int frameNum, int powerUpTime) {

    //temporary frame number variable is decreased to slow down walking animation of character
    frameNum /= 3;

    //temporary frame number variable is increased if the player is using a power up
    if (powerUpTime > 0) {
        frameNum *= 2;
    }

    //based on the temporary frame number variable a player position is returned
    //numbers loop from 0 to 4, then back to 0 to simulate walking
    switch (frameNum % 8) {
        case 0:
            return 0;
            break;
        case 1:
        case 7:
            return 1;
            break;
        case 2:
        case 6:
            return 2;
            break;
        case 3:
        case 5:
            return 3;
            break;
        case 4:
            return 4;
            break;
    }
}
