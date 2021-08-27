#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "header.h"

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_FONT *regular_font;
extern ALLEGRO_FONT *title_font;
extern ALLEGRO_SAMPLE *background_music;

//this function recieves no parameters
//allegro is initialzed
//true is returned if allegro is correctly initialized and false is returned if it is not
bool initializeAllegro() {

    //Allegro is initialized
    al_init();

    //font addons are intialized
    al_init_font_addon();
    al_init_ttf_addon();

    //audio addons are intialized
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);

    //primitives addon is initialized
    al_init_primitives_addon();

    //display is initialized
    display = al_create_display(1280, 720);
    if (!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //title is added to display
    al_set_window_title(display, "Keaton Lee ICS3U Summative: Coin Chaser");

    //keyboard is initialized
    if (!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize the keyboard!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //mouse is initialized
    if (!al_install_mouse()){
        al_show_native_message_box(display, "Error", "Error","Failed to initialize mouse!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //image addon is initialized
    if (!al_init_image_addon()) {
        al_show_native_message_box(display, "Error", "Error","Failed to initialize image addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //font addon is initialized
    if (!al_init_font_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize font addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //ttf addon is initialized
    if (!al_init_ttf_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize ttf addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //timer is created
    timer = al_create_timer(1.0 / 20);
    if (!timer) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create timer!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //event queue is created
    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //regular font is loaded
    regular_font = al_load_ttf_font("static/fonts/exo.ttf", 35, 0);
    if (!regular_font){
        al_show_native_message_box(display, "Error", "Error", "Could not load exo.ttf!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //title font is loaded
    title_font = al_load_ttf_font("static/fonts/quicksand.ttf", 100, 0);
    if (!title_font){
        al_show_native_message_box(display, "Error", "Error", "Could not load quicksand.ttf!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //background music is loaded
    background_music = al_load_sample("sounds/backgroundMusic.wav");
    if (!background_music) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize background_music!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //event sources are registered
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
 	al_register_event_source(event_queue, al_get_keyboard_event_source());
 	al_register_event_source(event_queue, al_get_mouse_event_source());

 	//background music begins playing
 	al_play_sample(background_music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

 	//timer is started
 	al_start_timer(timer);
    return true;
}
