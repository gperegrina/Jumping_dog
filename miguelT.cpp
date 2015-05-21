// Miguel Tlatelpa
// Did background, transparent background, and gameover
// still have more to do
// Duck Hunt

//Function prototypes
void init_opengl(void);
int check_keys(XEvent *e, Game *game);
void render(Game *game);
void makeDuck(Game *game);

//-----------------------------------------------------------------------------------------------------------------------------------------
Ppmimage *backgroundImage = NULL;
Ppmimage *backgroundTransImage = NULL;
Ppmimage *gameoverbgImage = NULL;
GLuint backgroundTexture;
GLuint backgroundTransTexture;
GLuint gameoverbgTexture;
int background = 1;
int trees = 1;
bool gameover = false;
//-----------------------------------------------------------------------------------------------------------------------------------------

unsigned char *buildAlphaData(Ppmimage *img) {
	// add 4th component to RGB stream...
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	//newdata = (unsigned char *)malloc(img->width * img->height * 4);
	newdata = new unsigned char[img->width * img->height * 4];
	ptr = newdata;
	for (int i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		data += 3;
	}
	return newdata;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

	//added for background
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//Allows fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	duckImage = ppm6GetImage("./images/duck.ppm");
	backgroundImage = ppm6GetImage("./images/background.ppm");
	backgroundTransImage = ppm6GetImage("./images/backgroundTrans.ppm");
	gameoverbgImage = ppm6GetImage("./images/gameoverbg.ppm");
	
	//create opengl texture elements
	glGenTextures(1, &duckTexture);
	glGenTextures(1, &duckSil);
	glGenTextures(1, &backgroundTexture);
	glGenTextures(1, &gameoverbgTexture);
	//-------------------------------------------------------------------
	//duck sprite
	int w = duckImage->width;
	int h = duckImage->height;
	//added to test
	//glGenTextures(1, &duckTexture);
	glBindTexture(GL_TEXTURE_2D, duckTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, duckImage->data);
	//-------------------------------------------------------------------
	
	//-------------------------------------------------------------------
	//duck silhouette 
	//
	glBindTexture(GL_TEXTURE_2D, duckSil);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	////must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(duckImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	//-------------------------------------------------------------------
	//background
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, backgroundImage->width, backgroundImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data); 
	//-------------------------------------------------------------------
	//forest transparent part
	//
	glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	w = backgroundTransImage->width;
	h = backgroundTransImage->height;
	unsigned char *ftData = buildAlphaData(backgroundTransImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	delete [] ftData;
	//-------------------------------------------------------------------
	//gameover
	glBindTexture(GL_TEXTURE_2D, gameoverbgTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, gameoverbgImage->width, gameoverbgImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, gameoverbgImage->data); 
	//-------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------------------------------------------------------------
int check_keys(XEvent *e, Game *game)
{
	Duck *d = game->duck;
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_Escape) {
			return 1;
		}
		//You may check other keys here.
		if(key == XK_1)
		{
		    	gameover = false;
			while(d)
			{
				deleteDuck(game, d);
				d = d->next;
			}	
			game->rounds = 1;
			game->duckCount = 0;
			game->duckShot = 0;
			std::cout << "ROUND " << game->rounds << std::endl;
			game->bullets = 3;
			std::cout << "makeduck" << std::endl;
			std::cout << "bullets = " << game->bullets << std::endl;
			if(!game->oneDuck)
				game->oneDuck = true;
			else
				game->oneDuck = false;
			game->twoDuck = false;
		}
		if(key == XK_2)
		{
		    	gameover = false;
			while(d)
			{
				deleteDuck(game, d);
				d = d->next;
			}	
			game->rounds = 1;
			game->duckCount = 0;
			game->duckShot = 0;
			std::cout << "ROUND " << game->rounds << std::endl;
			game->bullets = 3;
			std::cout << "2 makeduck" << std::endl;
			std::cout << "bullets = " << game->bullets << std::endl;
			if(!game->twoDuck)
				game->twoDuck = true;
			else
				game->twoDuck = false;
			game->oneDuck = false;
		}
//-----------------------------------------------------------------------------------------------------------------------------------------
		if(key == XK_3)
		{
			background ^= 1;
			std::cout << "background: " << background << std::endl;
		}
		if(key == XK_4)
		{
			silhouette ^= 1;
			std::cout << "silhouette: " << silhouette << std::endl;
		}
		if(key == XK_5)
		{
			trees ^= 1;
			std::cout << "trees: " << trees << std::endl;
		}
		if(key == XK_6)
		{
			gameover ^= 1;
			std::cout << "gameover: " << gameover << std::endl;
		}
//-----------------------------------------------------------------------------------------------------------------------------------------
	}
	return 0;
}

void render(Game *game)
{
	float w, h, x, y;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
//-----------------------------------------------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------------------------------------------
	if(game->oneDuck || game->twoDuck)
	{
		Duck *d = game->duck;
		if(!d && game->duckCount >= 10 && game->duckShot >= 6)
		{
			game->rounds++;
			game->duckCount = 0;
			game->duckShot = 0;
		}
		if(!d && game->duckCount >= 10 && game->duckShot < 6)
		{
			while(d)
			{
				deleteDuck(game, d);
				d = d->next;
			}
			game->oneDuck = false;
			game->twoDuck = false;
//-----------------------------------------------------------------------------------------------------------------------------------------
			gameover = true;
//-----------------------------------------------------------------------------------------------------------------------------------------
			std::cout << "GAME OVER" << std::endl;
		}
		glColor3ub(255, 255, 255);
		while(d)
		{
			w = d->s.width;
			h = d->s.height;
			x = d->s.center.x;
			y = d->s.center.y;
			glBegin(GL_QUADS);
			glVertex2f(x-w, y+h);
			glVertex2f(x-w, y-h);
			glVertex2f(x+w, y-h);
			glVertex2f(x+w, y+h);
			glEnd();
			d = d->next;
			//duck sprite
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
//-----------------------------------------------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------------------------------------------
			}
		}
	}
}
