// Jason Thai
// Gerardo Peregrina
// Duck Hunt

#include "header.h"
//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

struct timespec timeStart, timeCurrent;
struct timespec timePause;
double movementCountdown = 0.0;
double timeSpan = 0.0;
const double oobillion = 1.0 / 1e9;
const double movementRate = 1.0 / 60.0;

double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double)(end->tv_sec - start->tv_sec) + (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source)
{
    memcpy(dest, source, sizeof(struct timespec));
}

//First Duck Sprite
Sprite duck_sprite;
Ppmimage *duckImage=NULL;
GLuint duckTexture;
GLuint duckSil;
//Second Duck Sprite
Sprite duck_sprite2;
Ppmimage *duckImage2=NULL;
GLuint duckTexture2;
GLuint duckSil2;
int show_duck = 0;
int silhouette = 1;
//Bullet Sprite
Sprite bullet_sprite;
Ppmimage *bulletImage=NULL;
GLuint bulletTexture;
//White duck Sprite
Sprite duckscore_sprite;
Ppmimage *duckscoreImage=NULL;
GLuint duckscoreTexture;
//Red duck sprite
Sprite duckscore_sprite2;
Ppmimage *duckscoreImage2=NULL;
GLuint duckscoreTexture2;

Ppmimage *backgroundImage = NULL;
GLuint backgroundTexture;
int background = 1;
//Transparent background
Ppmimage *backgroundTransImage = NULL;
Ppmimage *gameoverbgImage = NULL;
GLuint backgroundTransTexture;
GLuint gameoverbgTexture;
int trees = 1;
bool gameover = false;

int main(void)
{
    int done=0;
    srand(time(NULL));
    initXWindows();
    init_opengl();
    init_sounds();

    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    //declare game object
    Game game;

    //start animation
    while(!done) {
	while(XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_resize(&e);
	    check_mouse(&e, &game);
	    done = check_keys(&e, &game);
	}
	clock_gettime(CLOCK_REALTIME, &timeCurrent);
	timeSpan = timeDiff(&timeStart, &timeCurrent);
	timeCopy(&timeStart, &timeCurrent);
	movementCountdown += timeSpan;
	while(movementCountdown >= movementRate)
	{
	    movement(&game);
	    movementCountdown -= movementRate;
	}
	render(&game);
	glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
#ifdef USE_SOUND
    fmod_cleanup();
#endif //USE_SOUND
    return 0;
}

