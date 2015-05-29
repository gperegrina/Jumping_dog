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

#include "header.h"

void makeDuck(Game *game, float x, float y)
{
    if(game->n >= MAX_DUCKS)
	return;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    timeCopy(&game->duckTimer, &dt);
    int directionNum = rand() % 101;
    Duck *d;
    try
    {
	d = new Duck;
    }
    catch(std::bad_alloc)
    {
	return;
    }
    timeCopy(&d->time, &dt);
    d->s.center.x = x;
    d->s.center.y = y;
    d->s.center.z = 0.0;
    if(directionNum >= 50)
	d->velocity.x = 4.0 * (game->rounds * .5);
    else
	d->velocity.x = -4.0 * (game->rounds * .5);
    d->velocity.y = 4.0 * (game->rounds * .5);
    d->velocity.z = 0.0;
    d->s.width = 50.0;
    d->s.height = 50.0;
    d->next = game->duck;
    if(game->duck != NULL)
    {
	game->duck->prev = d;
    }
    game->duck = d;
    game->n++;
}

void makeDeadDuck(Game *game, Duck *duck)
{
    if(game->onScreen >= MAX_DUCKS)
	return;
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
    //dd->velocity.y = -3.5;
    dd->velocity.z = 0.0;
    dd->s.width = 50.0;
    dd->s.height = 50.0;
    dd->next = game->deadD;
    if(game->deadD != NULL)
    {
	game->deadD->prev = dd;
    }
    game->deadD = dd;
    game->onScreen++;
}

void makeFreeDuck(Game *game, Duck *duck)
{
    if(game->onScreen >= MAX_DUCKS)
	return;
    //struct timespec dt;
    //clock_gettime(CLOCK_REALTIME, &dt);
    //timeCopy(&game->duckTimer, &dt);
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
    game->onScreen++;
}

void makeDog(Game *game, float x, float y)
{
    //if(!game->dogGone)
    //  return;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    timeCopy(&game->dogTimer, &dt);
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
    doge->velocity.y = 0.0;
    doge->velocity.z = 0.0;
    doge->s.width = 50.0;
    doge->s.height = 50.0;
    doge->next = game->dog;
    if(game->dog != NULL)
    {
	game->dog->prev = doge;
    }
    game->dog = doge;
    game->animateDog = true;
    //game->dogGone = false;
}

void makeHappyDog(Game *game, float x, float y)
{
    //if(!game->afterDog)
    //  return;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    timeCopy(&game->dogTimer, &dt);
    happyDog *hdoge;
    try
    {
	hdoge = new happyDog;
    }
    catch(std::bad_alloc)
    {
	return;
    }
    timeCopy(&hdoge->time, &dt);
    hdoge->s.center.x = x;
    hdoge->s.center.y = y;
    hdoge->s.center.z = 0.0;
    hdoge->velocity.x = 0.0;
    hdoge->velocity.y = 2.0;
    hdoge->velocity.z = 0.0;
    hdoge->s.width = 50.0;
    hdoge->s.height = 50.0;
    hdoge->next = game->hdog;
    if(game->hdog != NULL)
    {
	game->hdog->prev = hdoge;
    }
    game->hdog = hdoge;
    //game->animateDog = true;
    game->afterDog = true;
    game->waitForDog = true;
}

void makeLaughingDog(Game *game, float x, float y)
{
    //if(!game->afterDog)
    //  return;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    timeCopy(&game->dogTimer, &dt);
    laughingDog *ldoge;
    try
    {
	ldoge = new laughingDog;
    }
    catch(std::bad_alloc)
    {
	return;
    }
    timeCopy(&ldoge->time, &dt);
    ldoge->s.center.x = x;
    ldoge->s.center.y = y;
    ldoge->s.center.z = 0.0;
    ldoge->velocity.x = 0.0;
    ldoge->velocity.y = 2.0;
    ldoge->velocity.z = 0.0;
    ldoge->s.width = 50.0;
    ldoge->s.height = 50.0;
    ldoge->next = game->ldog;
    if(game->ldog != NULL)
    {
	game->ldog->prev = ldoge;
    }
    game->ldog = ldoge;
    //game->animateDog = true;
    game->afterDog = true;
    game->waitForDog = true;
}

void movement(Game *game)
{
    Duck *d = game->duck;
    deadDuck *dd = game->deadD;
    freeDuck *fd = game->freeD;
    Dog *doge = game->dog;
    happyDog *hdoge = game->hdog;
    laughingDog *ldoge = game->ldog;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    int randDirectionNumX, randDirectionNumY;

    //std::cout << doge << std::endl;
    //if (game->n <= 0)
//	return;

    //std::cout << doge << std::endl;
    while(d)
    {
	double ts = timeDiff(&d->time, &dt);
	randDirectionNumX = rand() % 101;
	randDirectionNumY = rand() % 101;
	if(ts > 5.0)
	{
	    makeFreeDuck(game, d);
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
	float velocity = -4.0;
	if(ts < 0.3)
	    dd->velocity.y = 0.0;
	if(ts > 0.3)
	    dd->velocity.y = velocity;
	if(dd->s.center.y - dd->s.height <= game->floor)
	{
	    game->waitForDog = true;
	    deleteDeadDuck(game, dd);
	    if(game->n == 0)
	    {
		game->afterDog = true;
		return;
	    }
	}
	dd->s.center.y += dd->velocity.y;
	dd = dd->next;
    }

    while(fd)
    {
	if(fd->s.center.x + fd->s.width <= 0.0)
	{
	    game->waitForDog = true;
	    deleteFreeDuck(game, fd);
	    if(game->n == 0)
	    {
		game->afterDog = true;
		return;
	    }
	}
	if(fd->s.center.x - fd->s.width >= WINDOW_WIDTH)
	{
	    game->waitForDog = true;
	    deleteFreeDuck(game, fd);
	    if(game->n == 0)
	    {
		game->afterDog = true;
		return;
	    }
	}
	if(fd->velocity.y < 0.0)
	    fd->velocity.y *= -1.0;
	if(fd->s.center.y - fd->s.height >= WINDOW_HEIGHT)
	{
	    game->waitForDog = true;
	    deleteFreeDuck(game, fd);
	    if(game->n == 0)
	    {
		game->afterDog = true;
		return;
	    }
	}

	fd->s.center.x += fd->velocity.x;
	fd->s.center.y += fd->velocity.y;

	fd = fd->next;
    }

    while(doge)
    {
	double ts = timeDiff(&doge->time, &dt);
	//std::cout << ts << std::endl;
	if(2.0 < ts && ts < 2.1)
	    doge->velocity.x = 0.0;
	if(3.0 < ts && ts < 3.1)
	    doge->velocity.x = 1.0;
	if(5.0 < ts && ts < 5.1)
	    doge->velocity.x = 0.0;
	if(6.0 < ts && ts < 6.1)
	{
	    doge->velocity.x = 1.5;
	    doge->velocity.y = 2.0;
	}
	if(6.5 < ts && ts < 6.6)
	{
	    doge->velocity.x = 0.5;
	    doge->velocity.y = -2.0;
	}
	if(doge->s.center.y - doge->s.height <= game->floor)
	{
	    deleteDog(game, doge);
	    //game->dogGone = true;
	    return;
	}
	doge->s.center.x += doge->velocity.x;
	doge->s.center.y += doge->velocity.y;

	doge = doge->next;
    }

    while(hdoge)
    {
	double ts = timeDiff(&hdoge->time, &dt);
	if(0.5 < ts && ts < 0.6)
	    hdoge->velocity.y = 0.0;
	if(1.5 < ts && ts < 1.6)
	    hdoge->velocity.y = -2.0;
	if(hdoge->s.center.y - hdoge->s.height <= game->floor)
	{
	    deleteHappyDog(game, hdoge);
	    //game->afterDog = false;
	    return;
	}
	hdoge->s.center.y += hdoge->velocity.y;

	hdoge = hdoge->next;
    }

    while(ldoge)
    {
	double ts = timeDiff(&ldoge->time, &dt);
	if(0.5 < ts && ts < 0.6)
	    ldoge->velocity.y = 0.0;
	if(1.5 < ts && ts < 1.6)
	    ldoge->velocity.y = -2.0;
	if(ldoge->s.center.y - ldoge->s.height <= game->floor)
	{
	    deleteLaughingDog(game, ldoge);
	    //game->afterDog = false;
	    return;
	}
	ldoge->s.center.y += ldoge->velocity.y;

	ldoge = ldoge->next;
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
    game->onScreen--;
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
    game->onScreen--;
}

void deleteDog(Game *game, Dog *node)
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
    game->animateDog = false;
    game->dogGone = true;
}

void deleteHappyDog(Game *game, happyDog *node)
{
    if(node->prev == NULL)
    {
	if(node->next == NULL)
	{
	    game->hdog = NULL;
	}
	else
	{
	    node->next->prev = NULL;
	    game->hdog = node->next;
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
    game->afterDog = false;
    game->waitForDog = false;
}

void deleteLaughingDog(Game *game, laughingDog *node)
{
    if(node->prev == NULL)
    {
	if(node->next == NULL)
	{
	    game->ldog = NULL;
	}
	else
	{
	    node->next->prev = NULL;
	    game->ldog = node->next;
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
    game->afterDog = false;
    game->waitForDog = false;
}

