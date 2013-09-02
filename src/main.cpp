#include <cstdio>
#include <unistd.h>
#include <GL/glut.h>
#include "types.h"
#include "emulator.h"
// Display size
#define SCREEN_W_GL 320
#define SCREEN_H_GL 288

#define SCREEN_W_GB 160
#define SCREEN_H_GB 144

using namespace std;

void updateFPS();
void display();
void reshape_window( GLsizei w, GLsizei h );
void initTexture();
void keyPressed( unsigned char key, int x, int y );
void keyReleased( unsigned char key, int x, int y );
void keySpecialPressed( int key, int x, int y );
void keySpecialReleased( int key, int x, int y );

GLsizei sw=SCREEN_W_GL,sh=SCREEN_H_GL;
u32 screen[SCREEN_W_GB*SCREEN_H_GB];

u32 fps=0,lastFps=0;

Emulator* emu;
Input* keys;

//framelimit stuff
u32 waitTime = 1000/60;
u32 frameStartTime = 0;
signed int delayTime;
int main( int argc, char **argv ) {
    //setup emu:
    emu = new Emulator( screen );
    
    // Setup OpenGL
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );

    glutInitWindowSize( SCREEN_W_GL, SCREEN_H_GL );
    glutInitWindowPosition( 320, 288 );
    glutCreateWindow( "GLGB" );

    glutDisplayFunc( display );
    glutIdleFunc( display );
    glutReshapeFunc( reshape_window );
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialReleased);
    initTexture();
    glutMainLoop();
    return 0;
}

void updateFPS() {
    if( ( glutGet( GLUT_ELAPSED_TIME )-lastFps ) > 1000 ) {

        //title = QString("fps %1").arg(fps);
        //setWindowTitle(title);
        static char buffer[20] = {0};
        sprintf( buffer, "%d FPS", fps );

        glutSetWindowTitle( buffer );
        fps = 0;
        lastFps = glutGet( GLUT_ELAPSED_TIME );
    }
    fps++;
}

void initTexture() {
    for( int i=0; i<( SCREEN_H_GB*SCREEN_W_GB ); i++ ) {
        screen[i] = 0x00ffffff;
    }
    // Create a texture
    glTexImage2D( GL_TEXTURE_2D, 0, 3, SCREEN_W_GB, SCREEN_H_GB, 0, GL_BGRA, GL_UNSIGNED_BYTE, ( GLvoid* )screen );

    // Set up the texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    // Enable textures
    glEnable( GL_TEXTURE_2D );
}

void updateTexture() {

    // Update Texture
    glTexSubImage2D( GL_TEXTURE_2D, 0 ,0, 0, SCREEN_W_GB, SCREEN_H_GB, GL_BGRA, GL_UNSIGNED_BYTE, ( GLvoid* )screen );

    glBegin( GL_QUADS );

    glTexCoord2d( 0.0, 0.0 );
    glVertex2d( 0.0, 0.0 );

    glTexCoord2d( 1.0, 0.0 );
    glVertex2d( sw, 0.0 );

    glTexCoord2d( 1.0, 1.0 );
    glVertex2d( sw, sh );

    glTexCoord2d( 0.0, 1.0 );
    glVertex2d( 0.0, sh );
    glEnd();
}

void display() {
    glClear( GL_COLOR_BUFFER_BIT );
    emu->run();
    updateTexture();
    updateFPS();
    glutSwapBuffers();

    delayTime = waitTime - (glutGet( GLUT_ELAPSED_TIME ) - frameStartTime);
    if(delayTime > 0) {
        usleep((u32)delayTime*1000);
    }
    frameStartTime = glutGet( GLUT_ELAPSED_TIME );
}

void reshape_window( GLsizei w, GLsizei h ) {
    glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, w, h, 0 );
    glMatrixMode( GL_MODELVIEW );
    glViewport( 0, 0, w, h );

    // Resize quad
    sw = w;
    sh = h;
}


void keyPressed( unsigned char key, int x, int y ) {
    switch(key) {
        case 'z': //b button
            emu->setKey(GB_KEY_B,GB_KEY_EVENT_PRESS);
            break;
        case 'x': //a button
            emu->setKey(GB_KEY_A,GB_KEY_EVENT_PRESS);
            break;
        case '/': //select button
            emu->setKey(GB_KEY_SELECT,GB_KEY_EVENT_PRESS);
            break;
        case 13: //start button
            emu->setKey(GB_KEY_START,GB_KEY_EVENT_PRESS);
            break;
        case 'q': //activate debugger
            emu->activateDebugger();
            break;
    }
}

void keyReleased( unsigned char key, int x, int y ) {
    switch(key) {
        case 'z': //b button
            emu->setKey(GB_KEY_B,GB_KEY_EVENT_RELEASE);
            break;
        case 'x': //a button
            emu->setKey(GB_KEY_A,GB_KEY_EVENT_RELEASE);
            break;
        case '/': //select button
            emu->setKey(GB_KEY_SELECT,GB_KEY_EVENT_RELEASE);
            break;
        case 13: //start button
            emu->setKey(GB_KEY_START,GB_KEY_EVENT_RELEASE);
            break;
    }
}
void keySpecialPressed( int key, int x, int y ) {
    switch(key) {
        case GLUT_KEY_UP:
            emu->setKey(GB_KEY_UP,GB_KEY_EVENT_PRESS);
            break;
        case GLUT_KEY_DOWN:
            emu->setKey(GB_KEY_DOWN,GB_KEY_EVENT_PRESS);
            break;
        case GLUT_KEY_LEFT:
            emu->setKey(GB_KEY_LEFT,GB_KEY_EVENT_PRESS);
            break;
        case GLUT_KEY_RIGHT:
            emu->setKey(GB_KEY_RIGHT,GB_KEY_EVENT_PRESS);
            break;
    }
}

void keySpecialReleased( int key, int x, int y ) {
    switch(key) {
        case GLUT_KEY_UP:
            emu->setKey(GB_KEY_UP,GB_KEY_EVENT_RELEASE);
            break;
        case GLUT_KEY_DOWN:
            emu->setKey(GB_KEY_DOWN,GB_KEY_EVENT_RELEASE);
            break;
        case GLUT_KEY_LEFT:
            emu->setKey(GB_KEY_LEFT,GB_KEY_EVENT_RELEASE);
            break;
        case GLUT_KEY_RIGHT:
            emu->setKey(GB_KEY_RIGHT,GB_KEY_EVENT_RELEASE);
            break;
    }
}