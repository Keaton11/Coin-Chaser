#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "header.h"

//this function recieves the seconds taken to complete the game, the Allegro display, the regular font, the title font, and the event queue
//the leaderboard is displayed as well as the players score
//true is returned if the player wants to play again, and false is returned if the player clicks exit
bool highscoreMenu(int seconds, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font, ALLEGRO_EVENT_QUEUE *event_queue) {

    //variables are declared
    player currentPlayer;   //struct holds data on current player score
    player leaderboard[10]; //array of structs hold data on past leaderboard scores
    char initialsArray[5] = ""; //character array holds character's initials
    char scoreString[10];   //string is used to display player's score on the screen
    std::string initials = "";  //string recieves character's keyboard input using Allegro event

    //the players score is converted to a string in terms of minutes and seconds
    currentPlayer.score = seconds;
    sprintf(scoreString, "%d:%02d", seconds / 60, seconds % 60);

    //file containing old highscores is opened
    FILE *fptr;
    fptr = fopen("highscore.txt", "r");
    if (!fptr) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load highscore.txt!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    //screen is cleared to blue and highscore, player's score, play again button, and exit button are displayed on the screen
    al_clear_to_color(Blue);
    al_draw_text(title_font, Black, 310, 0, ALLEGRO_ALIGN_CENTER, "Highscores:");
    al_draw_text(title_font, Black, 960, 0, ALLEGRO_ALIGN_CENTER, "Your Score:");
    al_draw_text(title_font, Black, 960, 100, ALLEGRO_ALIGN_CENTER, scoreString);

    al_draw_filled_rounded_rectangle(30, 649, 209, 691, 10, 10, Black);
    al_draw_text(regular_font, White, 38, 645, ALLEGRO_ALIGN_LEFT, "Play Again");

    al_draw_filled_rounded_rectangle(1180, 650, 1250, 690, 10, 10, Black);
    al_draw_text(regular_font, White, 1242, 649, ALLEGRO_ALIGN_RIGHT, "Exit");

    //the past leaderboard scores are recieved from text file
    for (int i = 0; i < 10; i++) {
        fscanf(fptr, "%s %d", leaderboard[i].initials, &leaderboard[i].score);
        leaderboard[i].place = i;
    }

    //leaderboard scores are reorganized based on new players score
    for (int i = 0; i < 10; i++) {
        if (currentPlayer.score < leaderboard[i].score && currentPlayer.place == 10) {
            for (int j = 9; j > i; j--) {
                leaderboard[j] = leaderboard[j - 1];
            }
            currentPlayer.place = i;
            leaderboard[i] = currentPlayer;
        }
    }

    //healines are created for the leaderboard consiting of each players place, name and time
    for (int i = 0; i < 10; i++) {
        sprintf(leaderboard[i].headline, "%d. %3s %d:%02d", i + 1, leaderboard[i].initials, leaderboard[i].score / 60, leaderboard[i].score % 60);
        al_draw_text(regular_font, Black, 230, (i * 60) + 120, ALLEGRO_ALIGN_LEFT, leaderboard[i].headline);
    }

    //player is informed whether they made it to the leaderboard or not
    if (currentPlayer.place != 10) {

        al_draw_text(regular_font, Black, 960, 220, ALLEGRO_ALIGN_CENTER, "You made it to the leaderboards!!");
        al_draw_text(regular_font, Black, 960, 265, ALLEGRO_ALIGN_CENTER, "Enter your initials to join them");
        al_flip_display();

        //new varibles are declared to help check the players key input
        bool initialsImputted = false;
        bool keyPressed = false;
        printf("%s", initials.c_str());

        //while loop continues until the player has inputted three characters or has clicked the enter key
        while (!initialsImputted && initials.length() < 3) {

            //Allegro event recieves player's key inputs and mouse clicks
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);

            //if a letter on the keyboard is clicked by the player, it is added to a string
            //the player can click backspace to remove a character, and enter to input their initials
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z) {
                    initials += ('A' + (ev.keyboard.keycode - ALLEGRO_KEY_A));
                    keyPressed = true;
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    if (initials.length() > 0) {
                        initials.erase(initials.length() - 1);
                    }
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (initials.length() >= 2) {
                        initialsImputted = true;
                    }
                }
            }

            //Allegro event queue checks if the player has clicked the screen's close button, the exit button, or the play again button
            if (button(ev, 1180, 1250, 650, 690) || checkExit(ev)) {
                initialsImputted = true;
                return false;
            } else if (button(ev, 30, 209, 649, 691)) {
                initialsImputted = true;
                return true;
            }

            if (keyPressed) {

                //leaderboard and initials are updated each time the player presses a key
                strcpy(initialsArray, initials.c_str());
                sprintf(leaderboard[currentPlayer.place].headline, "%d. %3s %s", currentPlayer.place + 1, initialsArray, scoreString);

                //screen is cleared to blue and highscore, player's score, play again button, and exit button are displayed on the screen
                al_clear_to_color(Blue);
                al_draw_text(regular_font, Black, 960, 220, ALLEGRO_ALIGN_CENTER, "You made it to the leaderboards!!");
                al_draw_text(regular_font, Black, 960, 265, ALLEGRO_ALIGN_CENTER, "Enter your initials to join them");
                al_draw_text(title_font, Black, 960, 0, ALLEGRO_ALIGN_CENTER, "Your Score:");
                al_draw_text(title_font, Black, 960, 100, ALLEGRO_ALIGN_CENTER, scoreString);
                al_draw_text(title_font, Black, 310, 0, ALLEGRO_ALIGN_CENTER, "Highscores:");

                al_draw_filled_rounded_rectangle(30, 649, 209, 691, 10, 10, Black);
                al_draw_text(regular_font, White, 38, 645, ALLEGRO_ALIGN_LEFT, "Play Again");

                al_draw_filled_rounded_rectangle(1180, 650, 1250, 690, 10, 10, Black);
                al_draw_text(regular_font, White, 1242, 649, ALLEGRO_ALIGN_RIGHT, "Exit");

                //while the player is inputting their intials, they are informed to click enter to save their initials
                if (!initialsImputted && initials.length() < 3) {
                    al_draw_text(regular_font, Black, 960, 310, ALLEGRO_ALIGN_CENTER, "Click enter to save your initials");
                }

                //leaderboard is re-updated with the new players score
                for (int i = 0; i < 10; i++) {
                    al_draw_text(regular_font, Black, 230, (i * 60) + 120, ALLEGRO_ALIGN_LEFT, leaderboard[i].headline);
                }
                al_flip_display();
            }
        }
        //player is informed if initials have been added to the leaderboard
        al_draw_text(regular_font, Black, 960, 310, ALLEGRO_ALIGN_CENTER, "Your initials have been added!");
        al_flip_display();

    } else {

        //player is informed if they did not make it to the leaderboards
        al_draw_text(regular_font, Black, 960, 220, ALLEGRO_ALIGN_CENTER, "Unfortunately, your score was too low");
        al_draw_text(regular_font, Black, 960, 265, ALLEGRO_ALIGN_CENTER, "to make it to the leaderboards :(");
        al_flip_display();
    }

    //file is closed
    fclose(fptr);

    //if player entered their initials, new leaderboard is stored under highscore textfile
    if (initials.length() > 1) {

        //file containing highscores is re-opened
        fptr = fopen("highscore.txt", "w");
        if (!fptr) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load highscore.txt!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        }

        //file highscores are edited to include the new player's score
        for (int i = 0; i < 10; i++) {
            if (currentPlayer.place == i) {
                fprintf(fptr, "%s %d\n", initialsArray, currentPlayer.score);
            } else {
                fprintf(fptr, "%s %d\n", leaderboard[i].initials, leaderboard[i].score);
            }
        }
        //file is closed
        fclose(fptr);
    }

    //while loop continues until plays exits the game or decides to play again
    while (1) {

        //Allegro event queue checks if the player has clicked the screen's close button, the exit button, or the play again button
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (button(ev, 1180, 1250, 650, 690) || checkExit(ev)) {
            return false;
        } else if (button(ev, 30, 209, 649, 691)) {
            return true;
        }
    }
}
