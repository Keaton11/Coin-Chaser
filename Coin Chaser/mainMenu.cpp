#include <stdio.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "header.h"

//function recieves the Allegro display, the Allegro event queue, the regular font, and the title font
//the main menu contains the game title, a play option, and a how to play option
//true is returned if the player clicks play and false is returned if the player clicks exit
bool mainMenu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font) {

    //variables are declared
    bool doExit = false;    //boolean is used to continue loop until instructed to exit
    bool menuScreen = true; //boolean determines if the menu screen is being shown
    bool rulesScreen = false;   //boolean determines if the rules screen is being shown
    bool drawArrow[2];  //boolean helps determine where the arrow should be drawn on the screen when the play hovers over a button
    int counter = 0;    //counter helps spin coin on main menu screen
    Collectible coin(835, 520, 1); //coin contains images of the spinning coin to display to the screen

    //background image is loaded
    ALLEGRO_BITMAP *background;
    background = al_load_bitmap("static/backgrounds/mainMenuBackground.jpeg");
    if (!background) {
        al_show_native_message_box(display, "Error", "Error", "Could not load static/backgrounds/mainMenuBackground.jpeg!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //arrow image is loaded
    ALLEGRO_BITMAP *arrow;
    arrow = al_load_bitmap("static/arrow.png");
    if (!arrow) {
        al_show_native_message_box(display, "Error", "Error", "Could not load static/arrow.png!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //Coin Chaser font is loaded
    ALLEGRO_FONT *coin_chaser_font;
    coin_chaser_font = al_load_ttf_font("static/fonts/mauikea.otf", 130, 0);
    if (!coin_chaser_font) {
        al_show_native_message_box(display, "Error", "Error", "Could not load mauikea.otf!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //while loop contains the main menu and the how to play screen
    while(!doExit) {

        //Allegro event queue checks what button the user has pressed on the main menu screen
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //arrows are removed from the screen if mouse is moved
        if (ev.mouse.x != 0 && ev.mouse.y != 0) {
            drawArrow[0] = false;
            drawArrow[1] = false;
        }

        //counter increases every frame to allow coin image to spin on the screen
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            counter++;

        //if play button is clicked on the main menu screen
        } else if (button(ev, 320, 500, 215, 315) == true && menuScreen == true) {
            doExit = true;

        //if how to play button is clicked on the main menu screen
        } else if (button(ev, 130, 690, 350, 450) == true && menuScreen == true) {
            rulesScreen = true;
            menuScreen = false;

        //if back button is clicked on the how to play screen
        } else if (button(ev, 35, 110, 650, 690) == true) {
            rulesScreen = false;
            menuScreen = true;

        //if exit or close button is clicked
        } else if (button(ev, 1180, 1250, 650, 690) == true || checkExit(ev)) {
            return false;

        //if mouse is hovering over the play button on the main menu screen
        } else if (ev.mouse.x > 320 && ev.mouse.x < 500 && ev.mouse.y > 215 && ev.mouse.y < 315) {
            drawArrow[0] = true;

        //if mouse is hovering over the play button on the main menu screen
        } else if (ev.mouse.x > 130 && ev.mouse.x < 690 && ev.mouse.y > 350 && ev.mouse.y < 450) {
            drawArrow[1] = true;
        }

        //screen is updated based on the user's input on the screen
        if (menuScreen) {

            //backround image is used for the main menu screen
            al_clear_to_color(Black);
            al_draw_bitmap(background, 0, 0, 0);

            //arrow image is drawn on the screen
            if (drawArrow[0]) {
                al_draw_bitmap(arrow, 230, 239, 0);
            } else if (drawArrow[1]) {
                al_draw_bitmap(arrow, 40, 371, 0);
            }

            //coin image is drawn on the screen
            al_draw_bitmap(coin.collectibleImage[counter % 10], coin.pos[0] + coin.displacement[counter % 10], coin.pos[1], 0);

            al_draw_text(coin_chaser_font, Black, 410, 70, ALLEGRO_ALIGN_CENTER, "Coin Chaser");
            al_draw_text(title_font, Black, 410, 215, ALLEGRO_ALIGN_CENTER, "Play");
            al_draw_text(title_font, Black, 410, 350, ALLEGRO_ALIGN_CENTER, "How to Play");
            al_draw_text(regular_font, Black, 38, 649, ALLEGRO_ALIGN_LEFT, "By Keaton Lee");
        } else {

            //screen is cleared to blue for the how to play screen
            al_clear_to_color(Blue);
            al_draw_text(title_font, Black, 640, 50, ALLEGRO_ALIGN_CENTER, "How to Play");
            al_draw_text(regular_font, Black, 35, 650, ALLEGRO_ALIGN_LEFT, "Back");

            //file containing information on how to play is opened
            FILE *fptr;
            fptr = fopen("howToPlay.txt", "r");
            if (!fptr) {
                al_show_native_message_box(display, "Error", "Error", "Failed to load highscore.txt!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }

            //variables to store and place lines in test file are declared
            int lineNum = 1;    //variable helps place lines in the correct position on the Allegro screen
            char line[100];     //variable recieves words from each line of text file

            //while loop displays text from test file
            while (fgets(line, sizeof(line), fptr)) {

                //one line is read from the text file and a time, then displayed on the screen
                line[strlen(line) - 1] = '\0';
                al_draw_text(regular_font, Black, 35, lineNum * 50 + 135, ALLEGRO_ALIGN_LEFT, line);
                lineNum++;
            }

            //file is closed
            fclose(fptr);
        }

        //screen is displayed with exit button
        al_draw_text(regular_font, Black, 1242, 649, ALLEGRO_ALIGN_RIGHT, "Exit");
        al_flip_display();
    }
    return true;
}
