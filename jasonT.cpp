// Jason Thai
// Duck Hunt
// Lab 6 / Homework 3
// 
// what i'm working on:
// movement and physics
// collision detection
// score keeping
// allocation/deallocation
// mouse/keyboard inputs

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
#include <unistd.h> //for sleep function ??

//800, 600
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define MAX_DUCKS 2

#define rnd() (float)rand() / (float)RAND_MAX

extern "C"
{
#include "fonts.h"
}

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

//Structures

struct Vec {
    float x, y, z;
};

struct Shape {
    float width, height, radius;
    Vec center;
    struct timespec time;
};

struct Duck
{
    Shape s;
    Vec velocity;
    struct timespec time;
    struct Duck *prev;
    struct Duck *next;
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
    //struct timespec time;  <-- for sprite animation?
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

struct Game {
    int bullets, n, rounds, score, duckCount, duckShot;
    Duck *duck;
    deadDuck *deadD;
    freeDuck *freeD;
    Dog *dog;
    float floor;
    struct timespec duckTimer, dogTimer;
    Shape box[6];
    bool oneDuck, twoDuck, animateDog;
    ~Game()
    {
	delete [] duck;
	delete [] deadD;
	delete [] freeD;
	delete [] dog;
    }
    Game()
    {
	duck = NULL;
	deadD = NULL;
	freeD = NULL;
	dog = NULL;
	bullets = 0;
	n = 0;
	floor = WINDOW_HEIGHT / 5.0;
	rounds = 1;
	score = 0;
	duckCount = 0;
	duckShot = 0;
	oneDuck = false;
	twoDuck = false;
	animateDog = false;

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

	//score on shot  <-- still working on it
	box[4].width = 45;
	box[4].height = 35;
	box[4].center.x = 0;
	box[4].center.y = 0;
	box[4].center.z = 0;
    }
};

void check_mouse(XEvent *e, Game *game);
void movement(Game *game);
void makeDuck(Game *game);
void makeDeadDuck(Game *game);
void makeFreeDuck(Game *game);
void makeDog(Game *game);
void deleteDuck(Game *game, Duck *duck);
void deleteDeadDuck(Game *game, deadDuck *deadD);
void deleteFreeDuck(Game *game, freeDuck *freeD);
void deleteDog(Game *game, Dog *dog);

void makeDeadDuck(Game *game, Duck *duck)
{
    //if(game->n >= MAX_DUCKS)	<-- keep just in case
    //  return;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    timeCopy(&game->duckTimer, &dt);
    deadDuck *dd;
    try
    {
	dd = new deadDuck;
    }
    catch(std::bad_alloc)
    {
	return;
    }
    timeCopy(&dd->time, &dt);
    dd->s.center.x = duck->s.center.x;
    dd->s.center.y = duck->s.center.y;
    dd->s.center.z = 0.0;
    dd->velocity.x = 0.0;
    dd->velocity.z = 0.0;
    dd->s.width = 50.0;
    dd->s.height = 50.0;
    dd->next = game->deadD;
    if(game->deadD != NULL)
    {
	game->deadD->prev = dd;
    }
    game->deadD = dd;
}

void makeFreeDuck(Game *game, Duck *duck)
{
    //if(game->n >= MAX_DUCKS)
    //  return;
    //struct timespec dt;
    //clock_gettime(CLOCK_REALTIME, &dt);
    //timeCopy(&game->duckTimer, &dt);		<-- might need for sprite animation
    freeDuck *fd;
    try
    {
	fd = new freeDuck;
    }
    catch(std::bad_alloc)
    {
	return;
    }
    //timeCopy(&dd->time, &dt);
    fd->s.center.x = duck->s.center.x;
    fd->s.center.y = duck->s.center.y;
    fd->s.center.z = 0.0;
    fd->velocity.x = duck->velocity.x;
    fd->velocity.y = duck->velocity.y;
    fd->velocity.z = 0.0;
    fd->s.width = 50.0;
    fd->s.height = 50.0;
    fd->next = game->freeD;
    if(game->freeD != NULL)
    {
	game->freeD->prev = fd;
    }
    game->freeD = fd;
}

void makeDog(Game *game, float x, float y)	// <-- working on dog
{
    //if(game->n >= MAX_DUCKS)
    //  return;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    timeCopy(&game->dogTimer, &dt);
    //std::cout << "makeDog() " << x << " " << y << std::endl;	// <-- for debugging
    Dog *doge;
    try
    {
	doge = new Dog;
    }
    catch(std::bad_alloc)
    {
	return;
    }
    timeCopy(&doge->time, &dt);
    doge->s.center.x = x;
    doge->s.center.y = y;
    doge->s.center.z = 0.0;
    doge->velocity.x = 1.0;
    doge->velocity.y = 1.0;
    doge->velocity.z = 0.0;
    doge->s.width = 50.0;
    doge->s.height = 50.0;
    doge->next = game->dog;
    if(game->dog != NULL)
    {
	game->dog->prev = doge;
    }
    game->dog = doge;
}

void check_mouse(XEvent *e, Game *game)		// <-- plan to make it shorter
{
    int y = WINDOW_HEIGHT - e->xbutton.y;

    Duck *d = game->duck;
    Duck *saved;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    double ts;

    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button was pressed
	    while(d)
	    {
		if(e->xbutton.x >= d->s.center.x - d->s.width &&
			e->xbutton.x <= d->s.center.x + d->s.width &&
			y <= d->s.center.y + d->s.height &&
			y >= d->s.center.y - d->s.height)
		{
		    ts = timeDiff(&d->time, &dt);
		    if(ts < 1.5)
			game->score += 200;
		    else
			game->score += 100;
		    makeDeadDuck(game, d);
		    saved = d->next;
		    deleteDuck(game, d);
		    d = saved;
		    game->bullets--;
		    game->duckShot++;
		    if(game->bullets < 1)
		    {
			if(game->n == 1)
			{
			    makeFreeDuck(game, d);
			    saved = d->next;
			    deleteDuck(game, d);
			    d = saved;
			}
			return;
		    }
		    return;
		}
		if(game->n == 2)	// <-- copy/paste, plan to make a separate function or something
		{
		    d = d->next;
		    if(e->xbutton.x >= d->s.center.x - d->s.width &&
			    e->xbutton.x <= d->s.center.x + d->s.width &&
			    y <= d->s.center.y + d->s.height &&
			    y >= d->s.center.y - d->s.height)
		    {
			ts = timeDiff(&d->time, &dt);
			if(ts < 1.5)
			    game->score += 200;
			else
			    game->score += 100;
			makeDeadDuck(game, d);
			saved = d->prev;
			deleteDuck(game, d);
			d = saved;
			game->bullets--;
			game->duckShot++;
			if(game->bullets < 1)
			{
			    if(game->n == 1)
			    {
				makeFreeDuck(game, d);
				saved = d->next;
				deleteDuck(game, d);
				d = saved;
			    }
			    return;
			}
			return;
		    }
		}
		if(game->bullets <= 1)
		{
		    if(game->n == 2)
		    {
			d = d->prev;
		    }
		    makeFreeDuck(game, d);
		    saved = d->next;
		    deleteDuck(game, d);
		    d = saved;
		    if(game->n == 1)
		    {
			makeFreeDuck(game, d);
			saved = d->next;
			deleteDuck(game, d);
			d = saved;
		    }
		    return;
		}
		game->bullets--;
		d = d->next;
	    }
	}
    }
    if (e->xbutton.button==3) {
	//Right button was pressed
	return;
    }
}

int check_keys(XEvent *e, Game *game)
{
    Duck *d = game->duck;
    Dog *doge = game->dog;
    //Was there input from the keyboard?
    if (e->type == KeyPress) {
	int key = XLookupKeysym(&e->xkey, 0);
	if (key == XK_Escape) {
	    return 1;
	}
	//You may check other keys here.
	if(key == XK_1)
	{
	    while(d)
	    {
		deleteDuck(game, d);
		d = d->next;
	    }
	    while(doge)
	    {
		deleteDog(game, doge);
		doge = doge->next;
	    }
	    game->rounds = 1;
	    game->duckCount = 0;
	    game->duckShot = 0;
	    game->bullets = 3;
	    game->score = 0;
	    if(!game->oneDuck)
		game->oneDuck = true;
	    else
		game->oneDuck = false;
	    game->twoDuck = false;
	    if(!game->animateDog)
		game->animateDog = true;
	    else
		game->animateDog = false;
	}
	if(key == XK_2)
	{
	    while(d)
	    {
		deleteDuck(game, d);
		d = d->next;
	    }
	    while(doge)
	    {
		deleteDog(game, doge);
		doge = doge->next;
	    }
	    game->rounds = 1;
	    game->duckCount = 0;
	    game->duckShot = 0;
	    game->bullets = 3;
	    game->score = 0;
	    if(!game->twoDuck)
		game->twoDuck = true;
	    else
		game->twoDuck = false;
	    game->oneDuck = false;
	    if(!game->animateDog)
		game->animateDog = true;
	    else
		game->animateDog = false;
	}
    }
    return 0;
}

void movement(Game *game)
{
    Duck *d = game->duck;
    deadDuck *dd = game->deadD;
    freeDuck *fd = game->freeD;
    Dog *doge = game->dog;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    int randDirectionNumX, randDirectionNumY;

    if (game->n <= 0)
	return;

    while(d)
    {
	double ts = timeDiff(&d->time, &dt);
	randDirectionNumX = rand() % 101;
	randDirectionNumY = rand() % 101;
	if(ts > 5.0)
	{
	    Duck *saved = d->next;
	    deleteDuck(game, d);
	    d = saved;
	    continue;
	}
	if(1.0 < ts && ts < 1.01)
	{
	    if(randDirectionNumX >= 50)
		d->velocity.x *= -1;
	    if(randDirectionNumY >= 50)
		d->velocity.y *= -1;
	}
	if(2.0 < ts && ts < 2.01)
	{
	    if(randDirectionNumX >= 50)
		d->velocity.x *= -1;
	    if(randDirectionNumY >= 50)
		d->velocity.y *= -1;
	}
	if(3.0 < ts && ts < 3.01)
	{
	    if(randDirectionNumX >= 50)
		d->velocity.x *= -1;
	    if(randDirectionNumY >= 50)
		d->velocity.y *= -1;
	}
	if(4.0 < ts && ts < 4.01)
	{
	    if(randDirectionNumX >= 50)
		d->velocity.x *= -1;
	    if(randDirectionNumY >= 50)
		d->velocity.y *= -1;
	}

	if(d->s.center.x - d->s.width <= 0.0)
	{
	    d->s.center.x = d->s.width + 0.1;
	    d->velocity.x *= -1.0;
	}
	if(d->s.center.x + d->s.width >= WINDOW_WIDTH)
	{
	    d->s.center.x = WINDOW_WIDTH - d->s.width - 0.1;
	    d->velocity.x *= -1.0;
	}
	if(d->s.center.y - d->s.height <= game->floor)
	{
	    d->s.center.y = game->floor + d->s.height + 0.1;
	    d->velocity.y *= -1.0;
	}
	if(d->s.center.y + d->s.height >= WINDOW_HEIGHT)
	{
	    d->s.center.y = WINDOW_HEIGHT - d->s.height - 0.1;
	    d->velocity.y *= -1.0;
	}

	d->s.center.x += d->velocity.x;
	d->s.center.y += d->velocity.y;

	d = d->next;
    }
    while(dd)
    {
	double ts = timeDiff(&dd->time, &dt);
	float velocity = -3.5;
	if(ts < 0.3)
	    dd->velocity.y = 0.0;
	if(ts > 0.3)
	    dd->velocity.y = velocity;
	if(dd->s.center.y - dd->s.height <= game->floor)
	    deleteDeadDuck(game, dd);
	dd->s.center.y += dd->velocity.y;
	dd = dd->next;
    }

    while(fd)
    {
	if(fd->s.center.x + fd->s.width <= 0.0)
	{
	    deleteFreeDuck(game, fd);
	}
	if(fd->s.center.x - fd->s.width >= WINDOW_WIDTH)
	{
	    deleteFreeDuck(game, fd);
	}
	if(fd->velocity.y < 0.0)
	    fd->velocity.y *= -1.0;
	if(fd->s.center.y - fd->s.height >= WINDOW_HEIGHT)
	{
	    deleteFreeDuck(game, fd);
	}

	fd->s.center.x += fd->velocity.x;
	fd->s.center.y += fd->velocity.y;

	fd = fd->next;
    }

    while(doge)		// <-- working on it
    {
	double ts = timeDiff(&doge->time, &dt);
	if(ts > 5.0)
	{
	    Dog *saved = doge->next;
	    deleteDog(game, doge);
	    doge = saved;
	    continue;
	}
	if(1.0 < ts && ts < 1.01)
	    doge->velocity.x = 0.0;
	if(2.0 < ts && ts < 2.01)
	    doge->velocity.x = 1.0;
	if(3.0 < ts && ts < 3.01)
	    doge->velocity.x = 0.0;
	if(4.0 < ts && ts < 4.01)
	    doge->velocity.x = 1.0;
	doge->s.center.x += doge->velocity.x;
	doge->s.center.y += doge->velocity.y;

	doge = doge->next;
    }
}

void deleteDuck(Game *game, Duck *node)
{
    if(node->prev == NULL)
    {
	if(node->next == NULL)
	{
	    game->duck = NULL;
	}
	else
	{
	    node->next->prev = NULL;
	    game->duck = node->next;
	}
    }
    else
    {
	if(node->next == NULL)
	{
	    node->prev->next = NULL;
	}
	else
	{
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    delete node;
    node = NULL;
    game->n--;
}

void deleteDeadDuck(Game *game, deadDuck *node)
{
    if(node->prev == NULL)
    {
	if(node->next == NULL)
	{
	    game->deadD = NULL;
	}
	else
	{
	    node->next->prev = NULL;
	    game->deadD = node->next;
	}
    }
    else
    {
	if(node->next == NULL)
	{
	    node->prev->next = NULL;
	}
	else
	{
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    delete node;
    node = NULL;
}

void deleteFreeDuck(Game *game, freeDuck *node)
{
    if(node->prev == NULL)
    {
	if(node->next == NULL)
	{
	    game->freeD = NULL;
	}
	else
	{
	    node->next->prev = NULL;
	    game->freeD = node->next;
	}
    }
    else
    {
	if(node->next == NULL)
	{
	    node->prev->next = NULL;
	}
	else
	{
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    delete node;
    node = NULL;
}

void deleteDog(Game *game, Dog *node)	// <-- working on it
{
    if(node->prev == NULL)
    {
	if(node->next == NULL)
	{
	    game->dog = NULL;
	}
	else
	{
	    node->next->prev = NULL;
	    game->dog = node->next;
	}
    }
    else
    {
	if(node->next == NULL)
	{
	    node->prev->next = NULL;
	}
	else
	{
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    delete node;
    node = NULL;
}

