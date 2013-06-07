// Benjamin Laws, CSE 21211-04

#include <math.h>
#include <stdlib.h>
#include "aquaterm/aquaterm.h"

void cloudbox(int x1,int y1,int x2,int y2,int g1,int g2,int g3,int g4){
  int margin = 8;
  if(abs(x1-x2)<margin+1 && abs(y1-y2)<margin+1){
    aqtRenderPlot();
    return;
  }

  int g,g12,g13,g24,g34,x12,y12;
  g12 = (g1+g2)/2;
  g13 = (g1+g3)/2;
  g24 = (g2+g4)/2;
  g34 = (g3+g4)/2;
  x12 = (x1+x2)/2;
  y12 = (y1+y2)/2;

  // color corners
  aqtSetColor(g1/510.0,g1/255.0,255);
  aqtAddFilledRect(x1,y1,2,2);
  aqtSetColor(g2/510.0,g2/255.0,255);
  aqtAddFilledRect(x2,y1,2,2);
  aqtSetColor(g3/510.0,g3/255.0,255);
  aqtAddFilledRect(x1,y2,2,2);
  aqtSetColor(g4/510.0,g4/255.0,255);
  aqtAddFilledRect(x2,y2,2,2);

  // color edges
  aqtSetColor(g12/510.0,g12/255.0,255);
  aqtAddFilledRect(x12,y1,margin,margin);
  aqtSetColor(g13/510.0,g13/255.0,255);
  aqtAddFilledRect(x1,y12,margin,margin);
  aqtSetColor(g24/510.0,g24/255.0,255);
  aqtAddFilledRect(x2,y12,margin,margin);
  aqtSetColor(g34/510.0,g34/255.0,255);
  aqtAddFilledRect(x12,y2,margin,margin);

  // color center
  g = (g1+g2+g3+g4)/4 + pow(-1,rand()%2)*(rand()%(abs(x2-x1)/2));
  if(g>255) g=255;
  if(g<0) g=0;
  aqtSetColor(g/510.0,g/255.0,255);
  aqtAddFilledRect(x12,y12,margin,margin);

  // recursion
  cloudbox(x1,y1,x12,y12,g1,g12,g13,g);
  cloudbox(x12,y1,x2,y12,g12,g2,g,g24);
  cloudbox(x1,y12,x12,y2,g13,g,g3,g34);
  cloudbox(x12,y12,x2,y2,g,g24,g34,g4);

  // update screen
  //aqtRenderPlot();
}

int makeCloudBackground(int xsize,int ysize){
  int a,b,c,d;
  a = rand()%256;
  b = rand()%256;
  c = rand()%256;
  d = rand()%256;
  cloudbox(0,0,xsize,ysize,a,b,c,d);
}
