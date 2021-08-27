#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "chunkMap.h"

#define White al_map_rgb(255, 255, 255)
#define Black al_map_rgb(0, 0, 0)
#define Blue al_map_rgb(173, 216, 230)
#define Red al_map_rgb(255, 0, 0)
#define Grey al_map_rgb(220, 220, 220)

//struct contains different player variables for highscore
struct player {
    char initials[5] = "___";   //the players intials
    int score = 0;  //the players score in seconds
    int place = 10; //the players place in relation to the leaderboard
    char headline[20] = ""; //the headline displayed for the player's score
};

//struct containing positions for entities
struct entityPos {
    int leftX;  //left x coordinate
    int topY;   //top y coordinate
    int rightX; //right x coordinate
    int btmY;   //bottom y coordinate
    int center[2];  //center coordinates
    int chunkCoords[2]; //the coordinates of the chunk the entity is on
};

//functions are prototyped
int displayTimer(ALLEGRO_FONT *regular_font, bool &resetTimer);

int direction(entityPos item1, entityPos item2);

int collectibleCollision(int r, int collectibleType, entityPos entity, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY);

int displayTimer(ALLEGRO_FONT *regular_font);

int displayPlayerPos(int frameNum, int powerUpTime);

void moveEnemies(int r, entityPos player, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY);

void displayProgress(int health, int coins, ALLEGRO_FONT *regular_font);

bool initializeAllegro();

bool mainMenu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font);

bool button(ALLEGRO_EVENT ev, int x1, int x2, int y1, int y2);

bool checkExit(ALLEGRO_EVENT &ev);

bool play(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font, ALLEGRO_DISPLAY *display);

bool isCollision(entityPos item1, entityPos item2);

bool highscoreMenu(int seconds, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font, ALLEGRO_EVENT_QUEUE *event_queue);

bool gameOver(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font, char death[10]);

entityPos obstacleCollision(int r, entityPos entity, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY);

entityPos updateEnemyBox(ChunkMap &chunkMap, int i, int j, int k);

entityPos enemyCollision(int r, entityPos entity, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY, int &health);

entityPos snap(entityPos playerPos, entityPos Box, int direction);
