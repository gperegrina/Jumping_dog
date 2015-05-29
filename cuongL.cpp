//Cuong Le
//Duck Hunt Game

//I worked on some part of the background, the background transparent, and sprites with group .
//Im working on now for the menu with clickable buttons to Play and Exit  the game. 
//Next I will working for the background change along with the game's rounds.

#include "header.h"

int check_keys(XEvent *e, Game *game) ////////////////leave for cuong?
{
    Duck *d = game->duck;
    deadDuck *dd = game->deadD;
    freeDuck *fd = game->freeD;
    Dog *doge = game->dog;
    happyDog *hdoge = game->hdog;
    laughingDog *ldoge = game->ldog;

    //Was there input from the keyboard?
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_Escape) {
            return 1;
        }
        //You may check other keys here.
        if(key == XK_1)
        {
            game->menutest = false;
            while(d)
            {
                deleteDuck(game, d);
                d = d->next;
            }
            while(dd)
            {
                deleteDeadDuck(game, dd);
                dd = dd->next;
            }
            while(fd)
            {
                deleteFreeDuck(game, fd);
                fd = fd->next;
            }
            while(doge)
            {
                deleteDog(game, doge);
                doge = doge->next;
            }
            while(hdoge)
            {
                deleteHappyDog(game, hdoge);
                hdoge = hdoge->next;
            }
            while(ldoge)
            {
                deleteLaughingDog(game, ldoge);
                ldoge = ldoge->next;
            }
            game->n = 0;
            game->onScreen = 0;
            game->rounds = 1;
            game->duckCount = 0;
            game->duckShot = 0;
            game->bullets = 3;
            game->score = 0;
            game->duckCaptured = 0;
            game->oneDuck = true;
            game->twoDuck = false;
            game->animateDog = true;
            game->dogGone = false;
            game->afterDog = true;
            game->waitForDog = false;
        }
        if(key == XK_2)
        {
            game->menutest = false;
            while(d)
            {
                deleteDuck(game, d);
                d = d->next;
            }
            while(dd)
            {
                deleteDeadDuck(game, dd);
                dd = dd->next;
            }
            while(fd)
            {
                deleteFreeDuck(game, fd);
                fd = fd->next;
            }
            while(doge)
            {
                deleteDog(game, doge);
                doge = doge->next;
            }
            while(hdoge)
            {
                deleteHappyDog(game, hdoge);
                hdoge = hdoge->next;
            }
            while(ldoge)
            {
                deleteLaughingDog(game, ldoge);
                ldoge = ldoge->next;
            }
            game->n = 0;
            game->onScreen = 0;
            game->rounds = 1;
            game->duckCount = 0;
            game->duckShot = 0;
            game->bullets = 3;
            game->score = 0;
            game->duckCaptured = 0;
            game->oneDuck = false;
            game->twoDuck = true;
            game->animateDog = true;
            game->dogGone = false;
            game->afterDog = true;
            game->waitForDog = false;
        }
    }
    return 0;
}

void menu(Game *game)
{
    float w, h;

    Rect r;
    r.bot = WINDOW_HEIGHT - 550;
    r.left = WINDOW_WIDTH - 715;
    r.center = 0;

    //-------------------------------------------------------------------
    //Drawing Boxes
    Shape *s;

    if (game->menutest == true) {
        const char* text[3] = {"One Duck Hunt", "Two Duct Hunt", "        Exit"}; // the Text need fixing to look better.
        for(int i=5; i<8; i++) {
            glColor3ub(90, 140, 90);
            s = &game->box[i];
            glPushMatrix();
            glTranslatef(s->center.x, s->center.y, s->center.z);
            w = s->width;
            h = s->height;
            r.bot = s->height - 75;
            r.left = s->width - 170;
            glBegin(GL_QUADS);
            glVertex2i(-w,-h);
            glVertex2i(-w, h);
            glVertex2i( w, h);
            glVertex2i( w,-h);
            glEnd();
            if (i == 5)
                ggprint16(&r, 35, 0x00ffffff, text[0]);
            if (i == 6)
                ggprint16(&r, 35, 0x00ffffff, text[1]);
            if (i == 7)
                ggprint16(&r, 35, 0x00ffffff, text[2]);
            r.bot = s->height - 120;
            r.left = s->width - 170;
            if (i == 5)
    		ggprint16(&r, 35, 0x00ffffff, "   Key \" 1 \"");
            if (i == 6)
                ggprint16(&r, 35, 0x00ffffff, "   Key \" 2 \"");
            if (i == 7)
                ggprint16(&r, 35, 0x00ffffff, "   Key \" Esc \"");
            glPopMatrix();
        }
    }
}
