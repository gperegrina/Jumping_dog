//Gerardo Peregrina
//Duckhunt
//Hw3
//Worked on the duck image, 
//bullet score image, and duck score image

#include "header.h"

//duck sprite
typedef double Arr[3];
/*struct Sprite {
    Arr pos;
    Arr vel;
};*/
/*
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
*/
void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Duck Hunt");
}

void cleanupXWindows(void) {
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void initXWindows(void) {
    //do not change
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w=WINDOW_WIDTH, h=WINDOW_HEIGHT;

    XSetWindowAttributes swa;

    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        std::cout << "\n\tcannot connect to X server\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
        std::cout << "\n\tno appropriate visual found\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    //XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask |
        PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
            InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    set_title();
}

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
        ptr += 4;
        data += 3;
    }
    return newdata;
}

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
    backgroundImage = ppm6GetImage("./images/background.ppm");
    backgroundTransImage = ppm6GetImage("./images/backgroundTrans.ppm");
    gameoverbgImage = ppm6GetImage("./images/gameoverbg.ppm");



    //-------------------------------------------------------------------
    //bullet
    glGenTextures(1, &bulletTexture);
    bulletImage = ppm6GetImage("./images/bullet.ppm");
    int w3 = bulletImage->width;
    int h3 = bulletImage->height;
    glBindTexture(GL_TEXTURE_2D, bulletTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w3, h3, 0, GL_RGB, GL_UNSIGNED_BYTE, bulletImage->data);
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //White duck score sprite   
    glGenTextures(1, &duckscoreTexture);
    duckscoreImage = ppm6GetImage("./images/duck_score_1.ppm");
    int w4 = duckscoreImage->width;
    int h4 = duckscoreImage->height;
    glBindTexture(GL_TEXTURE_2D, duckscoreTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w4, h4, 0, GL_RGB, GL_UNSIGNED_BYTE, duckscoreImage->data);
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //Red duck score sprite     
    glGenTextures(1, &duckscoreTexture2);
    duckscoreImage2 = ppm6GetImage("./images/duck_score_2.ppm");
    int w5 = duckscoreImage2->width;
    int h5 = duckscoreImage2->height;
    glBindTexture(GL_TEXTURE_2D, duckscoreTexture2);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w5, h5, 0, GL_RGB, GL_UNSIGNED_BYTE, duckscoreImage2->data);
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //duck sprite
    glGenTextures(1, &duckTexture);
    glGenTextures(1, &duckSil);
    duckImage = ppm6GetImage("./images/duck.ppm");
    int w = duckImage->width;
    int h = duckImage->height;
    glBindTexture(GL_TEXTURE_2D, duckTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, duckImage->data);
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //duck sprite 2
    glGenTextures(1, &duckTexture2);
    glGenTextures(1, &duckSil2);
    duckImage2 = ppm6GetImage("./images/duck2.ppm");
    int w2 = duckImage2->width;
    int h2 = duckImage2->height;
    glBindTexture(GL_TEXTURE_2D, duckTexture2);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0, GL_RGB, GL_UNSIGNED_BYTE, duckImage2->data);
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //duck silhouette 
    glBindTexture(GL_TEXTURE_2D, duckSil);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    ////must build a new set of data...
    unsigned char *silhouetteData = buildAlphaData(duckImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    delete [] silhouetteData;
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //duck silhouette 2 
    glBindTexture(GL_TEXTURE_2D, duckSil2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    ////must build a new set of data...
    unsigned char *silhouetteData2 = buildAlphaData(duckImage2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData2);
    delete [] silhouetteData2;
    //-------------------------------------------------------------------

//-------------------------------------------------------------------
    //background textures
    //create opengl texture elements
    glGenTextures(1, &backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, backgroundImage->width, backgroundImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data);
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //No genTextures for trans image?
    //forest transparent part
    glBindTexture(GL_TEXTURE_2D, backgroundTransTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    int w6 = backgroundTransImage->width;
    int h6 = backgroundTransImage->height;
    unsigned char *ftData = buildAlphaData(backgroundTransImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w6, h6, 0, GL_RGBA, GL_UNSIGNED_BYTE, ftData);
    delete [] ftData;
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //gameover
    glGenTextures(1, &gameoverbgTexture);
    glBindTexture(GL_TEXTURE_2D, gameoverbgTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, gameoverbgImage->width, gameoverbgImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, gameoverbgImage->data);
    //-------------------------------------------------------------------

    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

void check_resize(XEvent *e)
{
    if(e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    if(xce.width != WINDOW_WIDTH || xce.height != WINDOW_HEIGHT)
    {
        reshape_window(xce.width, xce.height);
    }
}
