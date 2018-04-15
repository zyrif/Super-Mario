#include <iostream>
#include <stdlib.h>
#include <math.h>

#include <GL/GLUT.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include <MMsystem.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;


float _cameraAngle = 0.0;


//cameraProperty
//float cameraX=-5.52;
float cameraX=-5.52;




//Mario Property
float marioPositionX=3;
float marioPositionY=-2.95;
bool jumpMarioKeyPressed=false;
bool jumpTopReached=false;
bool marioDirectionRight=true;
bool moveRight, moveLeft = false;
bool keyarr[4];


//texture variables
GLuint _textureFloor;
GLuint _textureBrick;
GLuint _textureScoreBlock;
GLuint _textureMario;
GLuint _textureHill;
GLuint _textureBlock;
GLuint _textureCloud;
GLuint _textureCylinder;
GLuint _textureEnemy;


//Cloud Property
float cloudPositionX=4.0;



//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void enableTexture(GLuint textureName){

    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);

}

void drawEnemy(){
            enableTexture(_textureEnemy);

        glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.5, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.5, 0.5, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 0.5, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawCylinder(){

        enableTexture(_textureCylinder);

        glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1, 2.0, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 2, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void enableSound(string state){


    if(state=="jump"){
     PlaySound("sounds/jump.wav", NULL, NULL | SND_ASYNC);

    }

}

void drawCloud(){

        enableTexture(_textureCloud);

        glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(3, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(3.0, 1.0, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 1, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawBlock(int length){

float translateFloorX=0.0;


    enableTexture(_textureBlock);


    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();



    glPopMatrix();

     }

     glDisable(GL_TEXTURE_2D);

}

void drawObstacleBlockByMyDefinedFunc(int length){

    glPushMatrix();

              float x=2.0;
              float y=-1.0;

              for(int i=length;i>0;i--){

                    glPushMatrix();
                        glTranslatef(x, y, 0);
                        drawBlock(i);
                    glPopMatrix();

                     x+=0.5;
                     y+=0.55;
              }

    glPopMatrix();

    glPushMatrix();

      glTranslatef(length*2,0.0,0.0);
      glRotatef(180,0.0,1.0,0.0);

              x=2.0;
              y=-1.0;

              for(int i=length;i>0;i--){

                    glPushMatrix();
                        glTranslatef(x, y, 0);
                        drawBlock(i);
                    glPopMatrix();

                     x+=0.5;
                     y+=0.55;
              }

    glPopMatrix();


}

void drawHill(){


    enableTexture(_textureHill);

    glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(3, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(3.0, 1.0, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 1, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void moveMario(){
    if(moveRight == true){
          marioPositionX +=0.1f;
         cameraX -=0.1f;
    }
    else if(moveLeft == true){
            if(marioPositionX>2.8){
                marioPositionX -=0.1f;
                cameraX +=0.1f;
            }
    }
}

void drawFloor(int length){

    float translateFloorX=0.0;


    enableTexture(_textureFloor);


    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX++, 0, 0);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX--, 0.5, 0);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX++, 0.5, 0);
        glEnd();



    glPopMatrix();

     }

     glDisable(GL_TEXTURE_2D);
}

void drawBrick(int length){


    float translateFloorX=0.0;

    enableTexture(_textureBrick);


    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();



    glPopMatrix();

     }

     glDisable(GL_TEXTURE_2D);
}

void drawScoreBrick(int length){

    float translateFloorX=0.0;


    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureScoreBlock);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


      time_t seconds;
      seconds = time (NULL);

      if(seconds%2==0){
        glColor3f(1.0f, 1.0f, 1.0f);
      }
      else{
        glColor3f(0.0f, 1.0f, 0.0f);
      }


    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();



    glPopMatrix();

     }

     glDisable(GL_TEXTURE_2D);
}

void drawMario(){


    enableTexture(_textureMario);


    glPushMatrix();




        if(marioDirectionRight==false){
            glTranslatef(marioPositionX-0.5, marioPositionY, 0);
            glRotatef(180,0.0,1.0,0.0);
        }else{
             glTranslatef(marioPositionX-1.0, marioPositionY, 0);
            glRotatef(0,0.0,0.0,0.0);
        }



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.5, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.5, 0.5, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 0.5, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawMarioNew(){


    glPushMatrix();


        if(marioDirectionRight==false){
            glTranslatef(marioPositionX-0.5, marioPositionY, 0);
            glRotatef(180,0.0,1.0,0.0);
        }else{
             glTranslatef(marioPositionX-1.0, marioPositionY, 0);
            glRotatef(0,0.0,0.0,0.0);
        }



        glBegin(GL_POLYGON);

        glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.90, 0.0);
glVertex3f(0.41, 0.93, 0.0);
glVertex3f(0.26, 0.93, 0.0);
glVertex3f(0.26, 0.90, 0.0);
glVertex3f(0.23, 0.90, 0.0);
glVertex3f(0.23, 0.87, 0.0);
glVertex3f(0.50, 0.87, 0.0);
glVertex3f(0.50, 0.90, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.87, 0.0);
glVertex3f(0.32, 0.87, 0.0);
glVertex3f(0.32, 0.84, 0.0);
glVertex3f(0.23, 0.84, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.29, 0.81, 0.0);
glVertex3f(0.29, 0.84, 0.0);
glVertex3f(0.26, 0.84, 0.0);
glVertex3f(0.26, 0.78, 0.0);
glVertex3f(0.32, 0.78, 0.0);
glVertex3f(0.32, 0.81, 0.0);



glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.32, 0.84, 0.0);
glVertex3f(0.29, 0.84, 0.0);
glVertex3f(0.29, 0.81, 0.0);
glVertex3f(0.32, 0.81, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.38, 0.72, 0.0);
glVertex3f(0.38, 0.87, 0.0);
glVertex3f(0.32, 0.87, 0.0);




glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.26, 0.84, 0.0);
glVertex3f(0.23, 0.84, 0.0);
glVertex3f(0.23, 0.78, 0.0);
glVertex3f(0.26, 0.78, 0.0);



glEnd();


glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.84, 0.0);
glVertex3f(0.20, 0.84, 0.0);
glVertex3f(0.20, 0.78, 0.0);
glVertex3f(0.23, 0.78, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.78, 0.0);
glVertex3f(0.26, 0.78, 0.0);
glVertex3f(0.26, 0.75, 0.0);
glVertex3f(0.23, 0.75, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.26, 0.78, 0.0);
glVertex3f(0.32, 0.78, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.26, 0.72, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 0.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.81, 0.0);
glVertex3f(0.41, 0.87, 0.0);
glVertex3f(0.38, 0.87, 0.0);
glVertex3f(0.38, 0.81, 0.0);
glVertex3f(0.44, 0.81, 0.0);
glVertex3f(0.44, 0.78, 0.0);
glVertex3f(0.41, 0.78, 0.0);



glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.81, 0.0);
glVertex3f(0.38, 0.81, 0.0);
glVertex3f(0.38, 0.78, 0.0);
glVertex3f(0.41, 0.78, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.44, 0.84, 0.0);
glVertex3f(0.44, 0.87, 0.0);
glVertex3f(0.41, 0.87, 0.0);
glVertex3f(0.41, 0.81, 0.0);
glVertex3f(0.44, 0.81, 0.0);
glVertex3f(0.44, 0.78, 0.0);
glVertex3f(0.50, 0.78, 0.0);
glVertex3f(0.50, 0.84, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.50, 0.81, 0.0);
glVertex3f(0.50, 0.78, 0.0);
glVertex3f(0.53, 0.78, 0.0);
glVertex3f(0.53, 0.81, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 0.0, 0.0);//(R,G,B)

glVertex3f(0.38, 0.75, 0.0);
glVertex3f(0.50, 0.75, 0.0);
glVertex3f(0.50, 0.78, 0.0);
glVertex3f(0.38, 0.78, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.38, 0.75, 0.0);
glVertex3f(0.38, 0.72, 0.0);
glVertex3f(0.44, 0.72, 0.0);
glVertex3f(0.44, 0.75, 0.0);



glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.26, 0.63, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.29, 0.72, 0.0);
glVertex3f(0.23, 0.72, 0.0);
glVertex3f(0.23, 0.69, 0.0);
glVertex3f(0.20, 0.69, 0.0);
glVertex3f(0.20, 0.66, 0.0);
glVertex3f(0.17, 0.66, 0.0);
glVertex3f(0.17, 0.63, 0.0);
glVertex3f(0.23, 0.63, 0.0);
glVertex3f(0.23, 0.60, 0.0);
glVertex3f(0.26, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.32, 0.63, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.29, 0.72, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.32, 0.63, 0.0);
glVertex3f(0.32, 0.54, 0.0);
glVertex3f(0.38, 0.54, 0.0);
glVertex3f(0.38, 0.66, 0.0);
glVertex3f(0.32, 0.66, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.38, 0.63, 0.0);
glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.41, 0.72, 0.0);
glVertex3f(0.38, 0.72, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.32, 0.66, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.38, 0.72, 0.0);
glVertex3f(0.38, 0.66, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.44, 0.63, 0.0);
glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.41, 0.72, 0.0);
glVertex3f(0.47, 0.72, 0.0);
glVertex3f(0.47, 0.69, 0.0);
glVertex3f(0.50, 0.69, 0.0);
glVertex3f(0.50, 0.66, 0.0);
glVertex3f(0.53, 0.66, 0.0);
glVertex3f(0.53, 0.63, 0.0);
glVertex3f(0.47, 0.63, 0.0);
glVertex3f(0.47, 0.60, 0.0);
glVertex3f(0.44, 0.60, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.38, 0.63, 0.0);
glVertex3f(0.38, 0.60, 0.0);
glVertex3f(0.41, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.32, 0.63, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.29, 0.60, 0.0);
glVertex3f(0.32, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.44, 0.57, 0.0);
glVertex3f(0.44, 0.63, 0.0);
glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.41, 0.60, 0.0);
glVertex3f(0.38, 0.60, 0.0);
glVertex3f(0.38, 0.51, 0.0);
glVertex3f(0.47, 0.51, 0.0);
glVertex3f(0.47, 0.57, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.26, 0.57, 0.0);
glVertex3f(0.26, 0.63, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.29, 0.60, 0.0);
glVertex3f(0.32, 0.60, 0.0);
glVertex3f(0.32, 0.51, 0.0);
glVertex3f(0.23, 0.51, 0.0);
glVertex3f(0.23, 0.57, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.60, 0.0);
glVertex3f(0.23, 0.63, 0.0);
glVertex3f(0.17, 0.63, 0.0);
glVertex3f(0.17, 0.54, 0.0);
glVertex3f(0.23, 0.54, 0.0);
glVertex3f(0.23, 0.57, 0.0);
glVertex3f(0.26, 0.57, 0.0);
glVertex3f(0.26, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.47, 0.60, 0.0);
glVertex3f(0.47, 0.63, 0.0);
glVertex3f(0.53, 0.63, 0.0);
glVertex3f(0.53, 0.54, 0.0);
glVertex3f(0.47, 0.54, 0.0);
glVertex3f(0.47, 0.57, 0.0);
glVertex3f(0.44, 0.57, 0.0);
glVertex3f(0.44, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.20, 0.48, 0.0);
glVertex3f(0.20, 0.51, 0.0);
glVertex3f(0.29, 0.51, 0.0);
glVertex3f(0.29, 0.45, 0.0);
glVertex3f(0.17, 0.45, 0.0);
glVertex3f(0.17, 0.48, 0.0);


glEnd();


glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.50, 0.48, 0.0);
glVertex3f(0.50, 0.51, 0.0);
glVertex3f(0.41, 0.51, 0.0);
glVertex3f(0.41, 0.45, 0.0);
glVertex3f(0.53, 0.45, 0.0);
glVertex3f(0.53, 0.48, 0.0);

        glEnd();


    glPopMatrix();
}

void jumpMario(){


    if(jumpMarioKeyPressed){
        if(jumpTopReached==false){
            if(marioPositionY<0.6){
                marioPositionY += .2f;
            }
            else{
                jumpTopReached = true;
            }
        }
        else if(jumpTopReached==true){

            if(marioPositionY>-2.95){
                marioPositionY-= .2f;
            }
            else{
                jumpMarioKeyPressed = false;
                jumpTopReached=false;
            }
        }
     }

}

void handleKeypress(unsigned char key, int x, int y) {

if(key=='w' || key==' '){
       enableSound("jump");
      jumpMarioKeyPressed=true;
}
if(key=='d'){
       marioDirectionRight=true;
       moveRight = true;
}
 if(key=='a'){
        marioDirectionRight=false;
        moveLeft = true;
 }

glutPostRedisplay();
}

void handleKeypressUp(unsigned char key, int x, int y) {

if(key=='w' || key==' '){
       enableSound("jump");
}
if(key=='d'){
       marioDirectionRight=true;
       moveRight = false;
}
 if(key=='a'){
        marioDirectionRight=false;
        moveLeft = false;
 }



glutPostRedisplay();

}

//Initializes 3D rendering
void initRendering() {

    glClearColor(0.48,0.47,1.0,1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);





    //texture
//    glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
//	glEnable(GL_COLOR_MATERIAL);

	Image* image = loadBMP("images/baseWall.bmp");
	_textureFloor = loadTexture(image);
	delete image;


	Image* image2 = loadBMP("images/question.bmp");
	_textureScoreBlock = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("images/brick.bmp");
	_textureBrick = loadTexture(image3);
	delete image3;


	Image* image4 = loadBMP("images/mario.bmp");
	_textureMario = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("images/hill.bmp");
	_textureHill = loadTexture(image5);
	delete image5;


	Image* image6 = loadBMP("images/block.bmp");
	_textureBlock = loadTexture(image6);
	delete image6;


	Image* image7 = loadBMP("images/cloud.bmp");
	_textureCloud = loadTexture(image7);
	delete image7;


	Image* image8 = loadBMP("images/cylinder.bmp");
	_textureCylinder = loadTexture(image8);
	delete image8;

	Image* image9 = loadBMP("images/enemy.bmp");
	_textureEnemy = loadTexture(image9);
	delete image9;



}

//Called when the window is resized
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

//Draws the 3D scene
void drawScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective
    glRotatef(-_cameraAngle, 0.0, 1.0, 0.0); //Rotate the camera

   // glTranslatef(0, 0.0, -7.0); //Move forward 5 units

    glTranslatef(cameraX, 0.0, -7.0); //Move forward 5 units




    glPushMatrix();
        drawMarioNew();
    glPopMatrix();

//draw floor
    glPushMatrix();
    glTranslatef(0, -3, 0);
        drawFloor(30);
    glPopMatrix();


//firsst power
    glPushMatrix();
    glTranslatef(5, -0.5, 0);
        drawBrick(1);
    glPopMatrix();

     glPushMatrix();
    glTranslatef(5.5, -0.5, 0);
        drawScoreBrick(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(6, -0.5, 0);
        drawBrick(1);
    glPopMatrix();



//second power
       glPushMatrix();
    glTranslatef(11, -0.5, 0);
        drawBrick(2);
    glPopMatrix();

        glPushMatrix();
    glTranslatef(12, -0.5, 0);
        drawScoreBrick(1);
    glPopMatrix();

      glPushMatrix();
    glTranslatef(12.5, -0.5, 0);
        drawBrick(2);
    glPopMatrix();

    glPushMatrix();
     glTranslatef(13, 0.5, 0);
     drawScoreBrick(1);
    glPopMatrix();

   glPushMatrix();
     glTranslatef(19, -2.5, 0);
     drawHill();
    glPopMatrix();



    //block draw

    glPushMatrix();
        glTranslatef(25.5, -1.5, 0);
        drawObstacleBlockByMyDefinedFunc(5);
    glPopMatrix();




    //draw floor
    glPushMatrix();
    glTranslatef(31, -3, 0);
        drawFloor(25);
    glPopMatrix();


    //draw cloud
    glPushMatrix();
        glTranslatef(cloudPositionX, 2, 0);
        drawCloud();
    glPopMatrix();

        //draw Cylinder
    glPushMatrix();
        glTranslatef(40, -3, 0);
        drawCylinder();
    glPopMatrix();



    //draw score brick group
    glPushMatrix();
    glTranslatef(46, 0, 0);

                glPushMatrix();
                    glTranslatef(0, -0.9, 0);
                    drawScoreBrick(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(1.5, -0.9, 0);
                    drawScoreBrick(1);
                glPopMatrix();

                   glPushMatrix();
                    glTranslatef(3, -0.9, 0);
                    drawScoreBrick(1);
                glPopMatrix();


                 glPushMatrix();
                    glTranslatef(1.5, 0.8, 0);
                    drawScoreBrick(1);
                glPopMatrix();

    glPopMatrix();




    //drawenemy

        glPushMatrix();
                    glTranslatef(17, -2.5, 0);
                    drawEnemy();
        glPopMatrix();

    glutSwapBuffers();
}

void update(int value) {

    jumpMario();

    moveMario();


    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(200,100);

    //Create the window
    glutCreateWindow("Super Mario");
    initRendering();


    //PlaySound("sounds/background.wav", NULL, SND_LOOP | SND_ASYNC);

    //Set handler functions
    glutDisplayFunc(drawScene);

    glutReshapeFunc(handleResize);

    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeypressUp);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();


}








