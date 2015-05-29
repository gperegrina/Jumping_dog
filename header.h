#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include <stdio.h>
#include <unistd.h> //for sleep function

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAX_DUCKS 2

#define rnd() (float)rand() / (float)RAND_MAX

extern "C"
{
#include "fonts.h"
}

#define USE_SOUND
#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"
#endif

//X Windows variables
extern Display *dpy;
extern Window win;
extern GLXContext glc;


extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double movementCountdown;
extern double timeSpan;
//const double oobillion = 1.0 / 1e9;
//const double movementRate = 1.0 / 60.0;
extern const double oobillion;
extern const double movementRate;
extern double timeDiff(struct timespec*, struct timespec*);
extern void timeCopy(struct timespec*, struct timespec*);
/*double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double)(end->tv_sec - start->tv_sec) + (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source)
{
    memcpy(dest, source, sizeof(struct timespec));
}*/

//duck sprite
typedef double Arr[3];
struct Sprite {
    Arr pos;
    Arr vel;
};

//First Duck Sprite
extern Sprite duck_sprite;
extern Ppmimage *duckImage;
extern GLuint duckTexture;
extern GLuint duckSil;
//Second Duck Sprite
extern Sprite duck_sprite2;
extern Ppmimage *duckImage2;
extern GLuint duckTexture2;
extern GLuint duckSil2;
extern int show_duck;
extern int silhouette;
//Bullet Sprite
extern Sprite bullet_sprite;
extern Ppmimage *bulletImage;
extern GLuint bulletTexture;
//White duck Sprite
extern Sprite duckscore_sprite;
extern Ppmimage *duckscoreImage;
extern GLuint duckscoreTexture;
//Red duck sprite
extern Sprite duckscore_sprite2;
extern Ppmimage *duckscoreImage2;
extern GLuint duckscoreTexture2;

struct Vec {
        float x, y, z;
};

struct Shape {
        float width, height;
	    float radius;
	        Vec center;
};


struct Duck
{
    Shape s;
    Vec velocity;
    struct timespec time;
    struct Duck *prev;
    struct Duck *next;
    //bool shot;
    Duck()
    {
        prev = NULL;
        next = NULL;
    }
};
struct deadDuck
{
    Shape s;
    Vec velocity;
    struct timespec time;
    struct deadDuck *prev;
    struct deadDuck *next;
    bool points;
    deadDuck()
    {
        prev = NULL;
        next = NULL;
    }
};
struct freeDuck
{
    Shape s;
    Vec velocity;
    //struct timespec time;
    struct freeDuck *prev;
    struct freeDuck *next;
    freeDuck()
    {
        prev = NULL;
        next = NULL;
    }
};

struct Dog
{
    Shape s;
    Vec velocity;
    struct timespec time;
    struct Dog *prev;
    struct Dog *next;
    Dog()
    {
        prev = NULL;
        next = NULL;
    }
};
struct happyDog
{
    Shape s;
    Vec velocity;
    struct timespec time;
    struct happyDog *prev;
    struct happyDog *next;
    happyDog()
    {
        prev = NULL;
        next = NULL;
    }
};
struct laughingDog
{
    Shape s;
    Vec velocity;
    struct timespec time;
    struct laughingDog *prev;
    struct laughingDog *next;
    laughingDog()
    {
        prev = NULL;
        next = NULL;
    }
};

struct Game {
    int bullets, n, rounds, score, duckCount, duckShot, onScreen, duckCaptured;
    Duck *duck;
    deadDuck *deadD;
    freeDuck *freeD;
    Dog *dog;
    happyDog *hdog;
    laughingDog *ldog;
    float floor;
    struct timespec duckTimer, dogTimer;
    Shape box[10];
    bool oneDuck, twoDuck, animateDog, dogGone, afterDog, waitForDog;
    bool menutest;
    ~Game()
    {
        delete duck;
        delete deadD;
        delete freeD;
        delete dog;
        delete hdog;
        delete ldog;
    }
    Game()
    {
        duck = NULL;
        deadD = NULL;
        freeD = NULL;
        dog = NULL;
        hdog = NULL;
        ldog = NULL;
        bullets = 0;
        n = 0;
        floor = WINDOW_HEIGHT / 5.0;
        rounds = 1;
        score = 0;
        duckCount = 0;
        duckShot = 0;
        onScreen = 0;
        duckCaptured = 0;
        oneDuck = false;
        twoDuck = false;
        animateDog = false;
        dogGone = true;
        afterDog = false;
        waitForDog = true;

        menutest = true;

        //bullet
        box[0].width = 45;
        box[0].height = 35;
        box[0].center.x = (WINDOW_WIDTH / 10) - (WINDOW_WIDTH / 20);
        box[0].center.y = WINDOW_HEIGHT - (WINDOW_HEIGHT - floor) - (floor / 1.1);
        box[0].center.z = 0;

        //count
        box[1].width = 100;
        box[1].height = 35;
        box[1].center.x = WINDOW_WIDTH / 4;
        box[1].center.y = WINDOW_HEIGHT - (WINDOW_HEIGHT - floor) - (floor / 1.1);
        box[1].center.z = 0;

        //score
        box[2].width = 45;
        box[2].height = 35;
        box[2].center.x = (WINDOW_WIDTH / 2) + (WINDOW_WIDTH / 4);
        box[2].center.y = WINDOW_HEIGHT - (WINDOW_HEIGHT - floor) - (floor / 1.1);
        box[2].center.z = 0;

        //round
        box[3].width = 45;
        box[3].height = 35;
        box[3].center.x = (WINDOW_WIDTH / 10) - (WINDOW_WIDTH / 70);
        box[3].center.y = WINDOW_HEIGHT - (WINDOW_HEIGHT - floor) - (floor / 1.5);
        box[3].center.z = 0;

        //score on shot
        box[4].width = 45;
        box[4].height = 35;
        box[4].center.x = 0;
        box[4].center.y = 0;
        box[4].center.z = 0;

        box[5].width = 140;
        box[5].height = 40;
        box[5].center.x = 400;
        box[5].center.y = 500;
        box[5].center.z = 0;

        box[6].width = 140;
        box[6].height = 40;
        box[6].center.x = 400;
        box[6].center.y = 400;
        box[6].center.z = 0;

        box[7].width = 140;
        box[7].height = 40;
        box[7].center.x = 400;
        box[7].center.y = 300;
        box[7].center.z = 0;



    }
};

//Function prototypes
//
// Miguel's
void render(Game *game);
// Gerardo's
void set_title(void);
void cleanupXWindows(void);
void initXWindows(void);
void reshape_window(int width, int height);
void init_opengl(void);
void check_resize(XEvent *e);
// Jason T's
void makeDuck(Game *game, float x, float y);
void makeDeadDuck(Game *game, Duck *duck);
void makeFreeDuck(Game *game, Duck *duck);
void makeDog(Game *game, float x, float y);
void makeHappyDog(Game *game, float x, float y);
void makeLaughingDog(Game *game, float x, float y);
void movement(Game *game);
void deleteDuck(Game *game, Duck *duck);
void deleteDeadDuck(Game *game, deadDuck *deadD);
void deleteFreeDuck(Game *game, freeDuck *freeD);
void deleteDog(Game *game, Dog *dog);
void deleteHappyDog(Game *game, happyDog *dog);
void deleteLaughingDog(Game *game, laughingDog *dog);
// Cuong's
int check_keys(XEvent *e, Game *game);
void menu(Game *game);
// Ayomide
void check_mouse(XEvent *e, Game *game);
void init_sounds(void);

extern Ppmimage *backgroundImage;
extern GLuint backgroundTexture;
extern int background;
//Transparent background
extern Ppmimage *backgroundTransImage;
extern Ppmimage *gameoverbgImage;
extern GLuint backgroundTransTexture;
extern GLuint gameoverbgTexture;
extern int trees;
extern bool gameover;


#endif
