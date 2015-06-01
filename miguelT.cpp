// Miguel Tlatelpa
// Did background, transparent background, and gameover
// still have more to do
// Duck Hunt

#include "header.h"

//The Display of the game 
void render(Game *game)
{
    float w, h, x, y;
    Duck *d = game->duck;
    deadDuck *dd = game->deadD;
    freeDuck *fd = game->freeD;
    Dog *doge = game->dog;
    happyDog *hdoge = game->hdog;
    laughingDog *ldoge = game->ldog;
    struct timespec dt;
    clock_gettime(CLOCK_REALTIME, &dt);

    glColor3ub(255, 255, 255);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Background and Gameover being displayed
    if(background) {
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
        glEnd();
    }
    if(gameover) {
        glBindTexture(GL_TEXTURE_2D, gameoverbgTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    //----------------------------------------

    //Drawing Shapes
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2f(0.0, game->floor);
    glVertex2f(WINDOW_WIDTH, game->floor);
    glEnd();

    Rect r;
    r.bot = WINDOW_HEIGHT - 550;
    r.left = WINDOW_WIDTH - 715;
    r.center = 0;

    menu(game);


    Shape *s;

    //Displaying bullets
    glColor3ub(90, 140, 90);
    s = &game->box[0];
    glPushMatrix();
    glTranslatef(s->center.x, s->center.y, s->center.z);
    w = s->width;
    h = s->height;
    int num = 0, dist = 0;
    if (game->bullets == 3) {
        num = 3, dist = 80;
    }
    if (game->bullets == 2) {
        num = 2, dist = 60;
    }
    if (game->bullets == 1) {
        num = 1, dist = 40;
    }
    if (game->bullets == 0) {
        num = 0, dist = 0;
	bullet_sprite.pos[0] = 0;
	bullet_sprite.pos[1] = 0;
	bullet_sprite.pos[2] = 0;
    }
    if (game->bullets != 0) {
        for (int i=0;i<num;i++) {
    		        bullet_sprite.pos[0] = dist - (i * 20);
            bullet_sprite.pos[1] = 40;
            bullet_sprite.pos[2] = 0;
            float wid = 10.0f;
            glPushMatrix();
            glTranslatef(bullet_sprite.pos[0], bullet_sprite.pos[1], bullet_sprite.pos[2]);
            glBindTexture(GL_TEXTURE_2D, bulletTexture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
            glBegin(GL_QUADS);
            if (bullet_sprite.vel[0] > 0.0) {
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            } else {
                glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
            }
            glEnd();
            glPopMatrix();
            glDisable(GL_ALPHA_TEST);
        }
    }
    r.bot = s->height;
    r.left = s->width;
    glVertex2i(-w, -h);
    glVertex2i(-w, h);
    glVertex2i(w, h);
    glVertex2i(w, -h);
    glEnd();
    //ggprint16(&r , 16, 0x00ffffff, "%i", game->bullets);
    glPopMatrix();

    //-------------------------------------------------------------------
    //Displaying duck score sprites
    glColor3ub(90, 140, 90);
    s = &game->box[1];
    glPushMatrix();
    glTranslatef(s->center.x, s->center.y, s->center.z);
    w = s->width;
    h = s->height;
    if (game->duckShot <= 10) {
        for (int i=0;i<=9;i++) {
            duckscore_sprite.pos[0] = 70 + (i * 25);
            duckscore_sprite.pos[1] = 42;
            duckscore_sprite.pos[2] = 0;
            float wid = 10.0f;
            glPushMatrix();
            glTranslatef(duckscore_sprite.pos[0], duckscore_sprite.pos[1], duckscore_sprite.pos[2]);
            glBindTexture(GL_TEXTURE_2D, duckscoreTexture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
            glBegin(GL_QUADS);
            if (duckscore_sprite.vel[0] > 0.0) {
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            } else {
                glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
            }
            glEnd();
            glPopMatrix();
            glDisable(GL_ALPHA_TEST);
        }
    }
    int loop = 0;
    if (game->duckShot == 1) loop = 1;
    if (game->duckShot == 2) loop = 2;
    if (game->duckShot == 3) loop = 3;
    if (game->duckShot == 4) loop = 4;
    if (game->duckShot == 5) loop = 5;
    if (game->duckShot == 6) loop = 6;
    if (game->duckShot == 7) loop = 7;
    if (game->duckShot == 8) loop = 8;
    if (game->duckShot == 9) loop = 9;
    if (game->duckShot == 10) loop = 10;
    if (game->duckShot <= 9) {
        for (int i=0;i<loop;i++) {
            duckscore_sprite2.pos[0] = 70 + (i * 25);
            duckscore_sprite2.pos[1] = 42;
            duckscore_sprite2.pos[2] = 0;
            float wid = 10.0f;
            glPushMatrix();
            glTranslatef(duckscore_sprite2.pos[0], duckscore_sprite2.pos[1], duckscore_sprite2.pos[2]);
            glBindTexture(GL_TEXTURE_2D, duckscoreTexture2);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
            glBegin(GL_QUADS);
            if (duckscore_sprite2.vel[0] > 0.0) {
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            } else {
                glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
            }
            glEnd();
            glPopMatrix();
            glDisable(GL_ALPHA_TEST);
        }
    }
    r.bot = s->height;
    r.left = s->width;
    glVertex2i(-w, -h);
    glVertex2i(-w, h);
    glVertex2i(w, h);
    glVertex2i(w, -h);
    glEnd();
    if(!d && game->duckCount >= 10 && game->duckShot < 6)
    {
        gameover = true;
    }
    glPopMatrix();

    glColor3ub(90, 140, 90);
    s = &game->box[2];
    glPushMatrix();
    glTranslatef(s->center.x, s->center.y, s->center.z);
    w = s->width;
    h = s->height;
    r.bot = s->height;
    r.left = s->width;
    glVertex2i(-w, -h);
    glVertex2i(-w, h);
    glVertex2i(w, h);
    glVertex2i(w, -h);
    glEnd();
    ggprint16(&r , 16, 0x00ffffff, "%i", game->score);
    glPopMatrix();

    glColor3ub(90, 140, 90);
    s = &game->box[3];
    glPushMatrix();
    glTranslatef(s->center.x, s->center.y, s->center.z);
    w = s->width;
    h = s->height;
    r.bot = s->height;
    r.left = s->width;
    glVertex2i(-w, -h);
    glVertex2i(-w, h);
    glVertex2i(w, h);
    glVertex2i(w, -h);
    glEnd();
    ggprint16(&r , 16, 0x00ffffff, "%i", game->rounds);
    glPopMatrix();

    if(game->afterDog && game->onScreen == 0 && game->dogGone && game->n == 0 && game->waitForDog)
    {
        if(game->duckCaptured >= 1)
            makeHappyDog(game, WINDOW_WIDTH / 2, game->floor + 51);
        else
            makeLaughingDog(game, WINDOW_WIDTH / 2, game->floor + 51);
        game->afterDog = false;
        game->duckCaptured = 0;
    }

    if(!d && game->duckCount >= 10 && game->duckShot >= 6 && game->dogGone && !game->waitForDog && game->onScreen == 0 && game->n == 0)
    {
        game->rounds++;
        game->duckCount = 0;
        game->duckShot = 0;
        game->animateDog = true;
        game->dogGone = false;
        game->afterDog = true;
        game->waitForDog = false;
    }
    if((game->oneDuck || game->twoDuck) && game->dogGone && !game->waitForDog)
    {
        if(!d && game->duckCount >= 10 && game->duckShot < 6)
        {
            while(d)
            {
                deleteDuck(game, d);
                d = d->next;
            }
            game->oneDuck = false;
            game->twoDuck = false;
            gameover = false;
            std::cout << "Rounds: " << game->rounds << std::endl;
            std::cout << "Score: " << game->score << std::endl;
            std::cout << "GAME OVER" << std::endl;
        }
        if(!d && game->oneDuck && game->duckCount < 10 && game->onScreen == 0 && game->n == 0)
        {
            game->bullets = 3;
            makeDuck(game, rand() % (WINDOW_WIDTH - 50 - 1) + 50 + 1, game->floor + 50 + 1);
            game->duckCount++;
        }
        if(!d && game->twoDuck && game->duckCount < 9 && game->onScreen == 0 && game->n == 0)
        {
            game->bullets = 3;
            makeDuck(game, rand() % (WINDOW_WIDTH - 50 - 1) + 50 + 1, game->floor + 50 + 1);
            makeDuck(game, rand() % (WINDOW_WIDTH - 50 - 1) + 50 + 1, game->floor + 50 + 1);
            game->duckCount += 2;
        }
    }


    //Main Duck rendering
    duck_sprite.pos[0] = s->center.x;
    glColor3ub(255, 255, 255);
    while(d)
    {
        w = d->s.width;
        h = d->s.height;
        x = d->s.center.x;
        y = d->s.center.y;
        d = d->next;
        show_duck= 1;
        float wid = 50.0f;
        duck_sprite.pos[0] = x;
        duck_sprite.pos[1] = y;
        duck_sprite.pos[2] = s->center.z;
       
        if(show_duck) {
            glPushMatrix();
            glTranslatef(duck_sprite.pos[0], duck_sprite.pos[1], duck_sprite.pos[2]);
           
            if (!silhouette)
            {
                glBindTexture(GL_TEXTURE_2D, duckTexture);
            } else {
                 glBindTexture(GL_TEXTURE_2D, duckSil);
                 glEnable(GL_ALPHA_TEST);
                 glAlphaFunc(GL_GREATER, 0.0f);
                 glColor4ub(255,255,255,255);

            }

            glBegin(GL_QUADS);
            if (duck_sprite.vel[0] > 0.0) {
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            } else {
                glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
            }
            glEnd();
            glPopMatrix();
            //Transparent part
            if (trees && silhouette) {
                glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
                glEnd();
            }
            glDisable(GL_ALPHA_TEST);
        }
    }

    //----------------------------------------------------
    glColor3ub(255, 0, 0);
    while(dd)
    {
        double ts = timeDiff(&dd->time, &dt);
        w = dd->s.width;
        h = dd->s.height;
        x = dd->s.center.x;
        y = dd->s.center.y;
        r.bot = y + h;
        r.left = x - (w/2);
        if(ts < 0.3)
        {
            if(dd->points == true)
                ggprint16(&r , 16, 0x00ffffff, "200");
            else
                ggprint16(&r , 16, 0x00ffffff, "100");
        }
        dd = dd->next;
        show_duck= 1;
        float wid = 50.0f;
        duck_sprite3.pos[0] = x;
        duck_sprite3.pos[1] = y;
        duck_sprite3.pos[2] = s->center.z;

        if(show_duck) {
                glPushMatrix();
                glTranslatef(duck_sprite3.pos[0], duck_sprite3.pos[1], duck_sprite3.pos[2]);
                if (!silhouette)
                {
                        glBindTexture(GL_TEXTURE_2D, duckTexture3);
                } else {
                        glBindTexture(GL_TEXTURE_2D, duckSil3);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                }
                glBegin(GL_QUADS);
                if (duck_sprite3.vel[0] > 0.0) {
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                } else {
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
                }
                glEnd();
                glPopMatrix();
                //Transparent part
                if (trees && silhouette) {
                        glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
                        glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
                        glEnd();
                }
                glDisable(GL_ALPHA_TEST);
        }
    }

    glColor3ub(0, 0, 255);
    while(fd)
    {
        w = fd->s.width;
        h = fd->s.height;
        x = fd->s.center.x;
        y = fd->s.center.y;
        fd = fd->next;
        show_duck= 1;
        float wid = 50.0f;
        duck_sprite5.pos[0] = x;
        duck_sprite5.pos[1] = y;
        duck_sprite5.pos[2] = s->center.z;
        
        if(show_duck) {
                glPushMatrix();
                glTranslatef(duck_sprite5.pos[0], duck_sprite5.pos[1], duck_sprite5.pos[2]);
                if (!silhouette)
                {
                        glBindTexture(GL_TEXTURE_2D, duckTexture5);
                } else {
                        glBindTexture(GL_TEXTURE_2D, duckSil5);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                }

                glBegin(GL_QUADS);
                if (duck_sprite5.vel[0] > 0.0) {
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                } else {
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
                }
                glEnd();
                glPopMatrix();
                //Transparent part
                if (trees && silhouette) {
                        glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
                        glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
                        glEnd();
                }
                glDisable(GL_ALPHA_TEST);
        }
    }

    if(game->animateDog)
    {
        makeDog(game, 100, game->floor + 51);
        game->animateDog = false;
    }
    glColor3ub(0, 0, 0);
    while(doge)
    {
        w = doge->s.width;
        h = doge->s.height;
        x = doge->s.center.x;
        y = doge->s.center.y;
        doge = doge->next;
        show_dog= 1;
        float wid = 50.0f;
        dog_sprite1.pos[0] = x;
        dog_sprite1.pos[1] = y;
        dog_sprite1.pos[2] = s->center.z;

        if(show_dog) {
                glPushMatrix();
                glTranslatef(dog_sprite1.pos[0], dog_sprite1.pos[1], dog_sprite1.pos[2]);
                glBindTexture(GL_TEXTURE_2D, dogTexture1); //test
                        glBindTexture(GL_TEXTURE_2D, dogSil1);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                glBegin(GL_QUADS);
                if (dog_sprite1.vel[0] > 0.0) {
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                } else {
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
                }
                glEnd();
                glPopMatrix();
                glDisable(GL_ALPHA_TEST);
        }
        dog_sprite4.pos[0] = x;
        dog_sprite4.pos[1] = y;
        dog_sprite4.pos[2] = s->center.z;
	if(jumpingdog) {
                glPushMatrix();
                glTranslatef(dog_sprite4.pos[0], dog_sprite4.pos[1], dog_sprite4.pos[2]);
                glBindTexture(GL_TEXTURE_2D, dogTexture4); //test
                        glBindTexture(GL_TEXTURE_2D, dogSil4);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                glBegin(GL_QUADS);
                if (dog_sprite4.vel[0] > 0.0) {
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                } else {
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
                }
                glEnd();
                glPopMatrix();
                glDisable(GL_ALPHA_TEST);
	}
    }

    glColor3ub(100, 100, 255);
    while(hdoge)
    {
	//fmod_playsound(1);
        w = hdoge->s.width;
        h = hdoge->s.height;
        x = hdoge->s.center.x;
        y = hdoge->s.center.y;
        hdoge = hdoge->next;
        show_dog= 1;
        float wid = 50.0f;
        dog_sprite2.pos[0] = x;
        dog_sprite2.pos[1] = y;
        dog_sprite2.pos[2] = s->center.z;

        if(show_dog) {
                glPushMatrix();
                glTranslatef(dog_sprite2.pos[0], dog_sprite2.pos[1], dog_sprite2.pos[2]);
                if (!silhouette)
                {
                        glBindTexture(GL_TEXTURE_2D, dogTexture2);
                } else {
                        glBindTexture(GL_TEXTURE_2D, dogSil2);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                }

                glBegin(GL_QUADS);
                if (dog_sprite2.vel[0] > 0.0) {
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                } else {
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
                }
                glEnd();
                glPopMatrix();
                //Transparent part
                if (trees && silhouette) {
                        glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
                        glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
                        glEnd();
                }
                glDisable(GL_ALPHA_TEST);
        }
    }

    glColor3ub(255, 100, 100);
    while(ldoge)
    {
        w = ldoge->s.width;
        h = ldoge->s.height;
        x = ldoge->s.center.x;
        y = ldoge->s.center.y;
        ldoge = ldoge->next;
        show_dog= 1;
        float wid = 50.0f;
        dog_sprite3.pos[0] = x;
        dog_sprite3.pos[1] = y;
        dog_sprite3.pos[2] = s->center.z;

        if(show_dog) {
                glPushMatrix();
                glTranslatef(dog_sprite3.pos[0], dog_sprite3.pos[1], dog_sprite3.pos[2]);
                if (!silhouette)
                {
                        glBindTexture(GL_TEXTURE_2D, dogTexture3);
                } else {
                        glBindTexture(GL_TEXTURE_2D, dogSil3);
                        glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);
                        glColor4ub(255,255,255,255);
                }

                glBegin(GL_QUADS);
                if (dog_sprite3.vel[0] > 0.0) {
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                } else {
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
                }
                glEnd();
                glPopMatrix();
                //Transparent part
                if (trees && silhouette) {
                        glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
                        glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
                        glEnd();
                }
                glDisable(GL_ALPHA_TEST);
        }
    }
}
