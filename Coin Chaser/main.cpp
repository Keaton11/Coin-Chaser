/********************************************************************
 *  Name: Keaton Lee                                 June 12, 2020  *
 *  Title: Coin Chaser                                              *
 *                                                                  *
 *  In this program, the user uses the arrow keys or the WASD keys  *
 *  to move across an infinite, dynamically generated tilemap. The  *
 *  user attempts to collect 20 coins using power ups while         *
 *  avoiding dangerous enemies.                                     *
 ********************************************************************/

#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "header.h"

ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
ALLEGRO_TIMER *timer = nullptr;
ALLEGRO_FONT *regular_font = nullptr;
ALLEGRO_FONT *title_font = nullptr;
ALLEGRO_SAMPLE *background_music = nullptr;

int main() {

    //Allegro is initialized
    if (!initializeAllegro()) {
        return 1;
    }

    //Random seed is initialized
    srand(time(0));

    //While loop continues to loop through main menu and game until the user has clicked exit
    while (mainMenu(display, event_queue, regular_font, title_font) == true && play(event_queue, regular_font, title_font, display) == true) {}

    //Samples are stopped
    al_stop_samples();

    //display, fonts, timer, event queue, and background music is destroyed
    al_destroy_display(display);
    al_destroy_font(regular_font);
    al_destroy_font(title_font);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(background_music);
    return 0;
}
