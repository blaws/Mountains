// Benjamin Laws, Mountains.c

#include "aquaterm/aquaterm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void changeColor(int* RGB,float y[],int ysize){
  int i;
  for(i=0;i<3;i++){  // change the color slightly
    RGB[i] += rand()%4 * pow(-1,rand()%2);
    if(RGB[i]<50) RGB[i]=50;
    else if(RGB[i]>120/(i*10+1)) RGB[i]=120/(i*30+1);
    if(abs(RGB[i]-RGB[(i+1)%3])>30) RGB[i]=RGB[(i+1)%3];
  }

  if(y[2]>3*ysize/4) aqtSetColor(1-RGB[0]/255.0,1-RGB[1]/255.0,1-RGB[2]/255.0);  // snow cap
  else aqtSetColor(RGB[0]/255.0,RGB[1]/255.0,RGB[2]/255.0);
}

void addTriangles(float x[3],float y[3],const int* RGB,const int ysize){
  int i,xLoc[3],yLoc[3];
  int* newRGB = malloc(3*sizeof(int));
  for(i=0;i<3;i++) newRGB[i] = RGB[i];

  // convert positions to ints for convenient bounds-checking
  for(i=0;i<3;i++){
    xLoc[i] = (int)x[i];
    yLoc[i] = (int)y[i];
  }

  changeColor(newRGB,y,ysize);

  // base case: zero-area triangle
  int margin = 1;
  if( (abs(xLoc[0]-xLoc[1])<margin && abs(xLoc[1]-xLoc[2])<margin) || (abs(yLoc[0]-yLoc[1])<margin && abs(yLoc[1]-yLoc[2])<margin) ){
    aqtAddPolygon(x,y,3);
    aqtRenderPlot();
    free(newRGB);
    return;
  }
  for(i=0;i<3;i++){
    if(abs(xLoc[i]-xLoc[(i+1)%3])<margin && abs(yLoc[i]-yLoc[(i+1)%3])<margin){
      aqtAddPolygon(x,y,3);
      aqtRenderPlot();
      free(newRGB);
      return;
    }
  }

  // base case: small triangle
  margin = 40;
  if((abs(xLoc[0]-xLoc[1])<margin && abs(yLoc[0]-yLoc[1])<margin ) &&
     (abs(xLoc[1]-xLoc[2])<margin && abs(yLoc[1]-yLoc[2])<margin ) &&
     (abs(xLoc[0]-xLoc[2])<margin && abs(yLoc[0]-yLoc[2])<margin ) ){
    aqtAddPolygon(x,y,3);
    aqtRenderPlot();
    free(newRGB);
    return;
  }

  // recursive case: subdivide into four triangles
  float mx[3],my[3];
  for(i=0;i<3;i++){  // get one point on each edge of triangle
    mx[i] = (x[i]+x[(i+1)%3]) / 2;
    my[i] = (y[i]+y[(i+1)%3]) / 2;
    float shiftfactor = pow(-1,rand()%2) * (rand()%76/100.0);
    mx[i] += shiftfactor * fabs(x[i]-x[(i+1)%3]) / 2;
    if((x[i]>x[(i+1)%3] && y[i]<y[(i+1)%3]) ||
       (x[i]<x[(i+1)%3] && y[i]>y[(i+1)%3]) ){  // edge has negative slope
      shiftfactor *= -1;
    }
    my[i] += shiftfactor * abs(y[i]-y[(i+1)%3]) / 2;
  }
  float tri1x[3] = {x[0],mx[0],mx[2]};
  float tri1y[3] = {y[0],my[0],my[2]};
  float tri2x[3] = {mx[0],x[1],mx[1]};
  float tri2y[3] = {my[0],y[1],my[1]};
  float tri3x[3] = {mx[0],mx[1],mx[2]};
  float tri3y[3] = {my[0],my[1],my[2]};
  float tri4x[3] = {mx[2],mx[1],x[2]};
  float tri4y[3] = {my[2],my[1],y[2]};

  // recursion
  addTriangles(tri1x,tri1y,newRGB,ysize);
  addTriangles(tri2x,tri2y,newRGB,ysize);
  addTriangles(tri3x,tri3y,newRGB,ysize);
  addTriangles(tri4x,tri4y,newRGB,ysize);
  free(newRGB);
}

int main(void){
  // set up variables
  int xsize=1200,ysize=750;
  int* RGB = malloc(3*sizeof(int));
  float xLoc[3],yLoc[3];
  char c;
  srand(time(NULL));

  // prompt user
  printf("Add cloud background (y/n)? ");
  scanf("%c",&c);

  // initialize window
  aqtInit();
  aqtOpenPlot(1);
  aqtSetPlotSize(xsize,ysize);
  aqtSetPlotTitle("Mountains");

  // add background
  if(c=='y') makeCloudBackground(xsize,ysize);
  else{
    aqtSetColor(.4,.6,.8);
    aqtAddFilledRect(0,0,xsize,ysize);
  }

  // set initial triangle position and color
  xLoc[0] = pow(-1,rand()%2) * (rand()%20);
  yLoc[0] = -1 * rand()%20;
  xLoc[1] = xsize + pow(-1,rand()%2) * (rand()%20);
  yLoc[1] = -1 * rand()%20;
  xLoc[2] = xsize/4 + rand()%(xsize/2);
  yLoc[2] = ysize + pow(-1,rand()%2) * (rand()%20);
  RGB[0] = RGB[1] = RGB[2] = 50 + rand()%50;

  // draw
  addTriangles(xLoc,yLoc,RGB,ysize);
  free(RGB);

  // wait
  aqtClosePlot();
  aqtTerminate();
}
