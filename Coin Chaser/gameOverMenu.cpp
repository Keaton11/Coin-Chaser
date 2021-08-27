#include <stdio.h>

#include <allegro5/allegro.h>

#include "header.h"

//function recives the allegro event queue, the regular font, the title font, and the character's cause of death
//this function displays the game over screen and determines whether player wants to exit or play again
//true is returned if player wants to play again, and false is returned if not
bool gameOver(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font, char death[10]) {

    //lava background image is loaded
    ALLEGRO_BITMAP *lavaBackground;
    lavaBackground = al_load_bitmap("static/backgrounds/gameOverBackgroundLava.jpeg");
    if (!lavaBackground){
        al_show_native_message_box(display, "Error", "Error", "Could not load static/backgrounds/gameOverBackgroundLava.jpeg!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //enemy background image is loaded
    ALLEGRO_BITMAP *enemyBackground;
    enemyBackground = al_load_bitmap("static/backgrounds/gameOverBackgroundEnemy.jpeg");
    if (!enemyBackground){
        al_show_native_message_box(display, "Error", "Error", "Could not load static/backgrounds/gameOverBackgroundEnemy.jpeg!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //Game Over font is loaded
    ALLEGRO_FONT *game_over_font;
    game_over_font = al_load_ttf_font("static/fonts/mauikea.otf", 130, 0);
    if (!game_over_font){
        al_show_native_message_box(display, "Error", "Error", "Could not load mauikea.otf!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //backround is cleared
    al_clear_to_color(Black);

    //a message is display on the screen based on the player's cause of death
    //the backgroun on the screen corresponds to the player's cause of death
    if (strcmp(death, "lava") == 0) {
        al_draw_bitmap(lavaBackground, 0, 0, 0);
        al_draw_text(regular_font, Red, 640, 340, ALLEGRO_ALIGN_CENTER, "You were killed by lava!");
    } else if (strcmp(death, "enemy") == 0) {
        al_draw_bitmap(enemyBackground, 0, 0, 0);
        al_draw_text(regular_font, Red, 640, 340, ALLEGRO_ALIGN_CENTER, "You were killed by an enemy!");
    }

    //"Game over" is displayed on the Allegro screen as well as an exit and a play again button
    al_draw_text(game_over_font, Red, 640, 210, ALLEGRO_ALIGN_CENTER, "Game Over");

    al_draw_text(regular_font, White, 1242, 649, ALLEGRO_ALIGN_RIGHT, "Exit");

    al_draw_text(regular_font, White, 38, 645, ALLEGRO_ALIGN_LEFT, "Play Again");

    al_flip_display();

    //while loop continues until plays exits the game or decides to play again
    while (1) {

        //Allegro event queue checks if the player has clicked the screen's close button, the exit button, or the play again button
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (button(ev, 1180, 1250, 650, 690) || checkExit(ev)) {
            return false;
        } else if (button(ev, 30, 649, 209, 691)) {
            return true;
        }
    }
}
