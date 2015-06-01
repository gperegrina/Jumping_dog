// ayomide johnson
// cs335
// Duck Hunt
// my github:
// added sound to jasons work from github: https://github.com/Husky3388/duckhunt

#include "header.h"

void check_mouse(XEvent *e, Game *game)
{
    int y = WINDOW_HEIGHT - e->xbutton.y;

    Duck *d = game->duck;
    Duck *saved = new Duck;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);
    double ts;

    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
//#ifdef USE_SOUND //////////////////////////////////////////////////
          fmod_playsound(0);
            //Left button was pressed
            while(d)
            {
                if(e->xbutton.x >= d->s.center.x - d->s.width &&
                        e->xbutton.x <= d->s.center.x + d->s.width &&
                        y <= d->s.center.y + d->s.height &&
                        y >= d->s.center.y - d->s.height)
                {
//                    fmod_playsound(1);
                    makeDeadDuck(game, d);
                    ts = timeDiff(&d->time, &dt);
                    if(ts < 1.5)
                    {
                        game->deadD->points = true;
                        game->score += 200;
                    }
                    else
                    {
                        game->deadD->points = false;
                        game->score += 100;
                    }
                    saved = d->next;
                    deleteDuck(game, d);
                    d = saved;
                    game->bullets--;
                    game->duckShot++;
                    game->duckCaptured++;
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
                if(game->n == 2)
                {
                    d = d->next;
                    if(e->xbutton.x >= d->s.center.x - d->s.width &&
                            e->xbutton.x <= d->s.center.x + d->s.width &&
                            y <= d->s.center.y + d->s.height &&
                            y >= d->s.center.y - d->s.height)
                    {
                        makeDeadDuck(game, d);
                        ts = timeDiff(&d->time, &dt);
                        if(ts < 1.5)
                        {
                            game->deadD->points = true;
                            game->score += 200;
                        }
                        else
                        {
                            game->deadD->points = false;
                            game->score += 100;
                        }
                        saved = d->prev;
                        deleteDuck(game, d);
                        d = saved;
                        game->bullets--;
                        game->duckShot++;
                        game->duckCaptured++;
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
//#endif
    }
    if (e->xbutton.button==3) {
        //Right button was pressed
        return;
    }
}

//---------------------------------------------------
void init_sounds(void)
{
#ifdef USE_SOUND
    //FMOD_RESULT result;
    if (fmod_init()) {
        std::cout << "ERROR - fmod_init()\n" << std::endl;
        return;
    }
    if (fmod_createsound((char *)"./sounds/gunshot.wav", 0)) {
        std::cout << "ERROR - fmod_createsound()\n" << std::endl;
        return;
    }
    if (fmod_createsound((char *)"./sounds/drip.mp3", 1)) {
        std::cout << "ERROR - fmod_createsound()\n" << std::endl;
        return;
    }
    fmod_setmode(0,FMOD_LOOP_OFF);
    //fmod_playsound(0);
    //fmod_systemupdate();
#endif //USE_SOUND
}
