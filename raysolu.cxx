#include "generic.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "tools/vectorr3.h"

using namespace std;

VectorR3 span_x, span_y;
VectorR3 center;

VectorR3 observer;
VectorR3 target;

VectorR3 light;

SDL_Surface *screen;
int          window_width, window_height;

inline double sqr(double v)
{
  return v*v;
} // srq

inline void SetPixelColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
  ((unsigned char*)(screen->pixels))[x*3+0 + y*screen->pitch] = blue;
  ((unsigned char*)(screen->pixels))[x*3+1 + y*screen->pitch] = green;
  ((unsigned char*)(screen->pixels))[x*3+2 + y*screen->pitch] = red;
} // SetPixelColor

inline void SetPixel(int x, int y)
{
  SetPixelColor(x, y, 255, 255, 255);
} // SetPixel

bool CalcProPlane(VectorR3 B, VectorR3 Z, int Res_x, int Res_y, int angle,  VectorR3 &A, VectorR3 &x, VectorR3 &y)
{
  VectorR3 b;
  float    laenge;

  // Blickvektor berechnen
  
  b = Z - B;
  
  if((b[0] != 0.0) || (b[1] != 0.0) || (b[2] != 0.0))
    {
      // Aufhänger A der Ebene berechnen
      
      laenge = b.Norm2();
      
      A = B + b/laenge;
      
      // Berechnung des Spannvektors x
      
      x[1] = 0.0; // Kein geneigter Betrachter -> Projektionsebene parallel zur X-Z Ebene
      
      if(b[2] != 0.0)
	{
	  x[0] = 1/((float)sqrt((double)(1.0+sqr(b[0]/b[2]))));
	  x[2] = (-x[0]*b[0])/b[2];
	} // if
      else
	{
	  if(b[0] != 0.0)
	    {
	      x[0] = 0.0;
	      x[2] = 1.0;
	    } // if
	  else
	    {
	      x[0] = 1.0;
	      x[2] = 0.0;
	    } // else
	} // else
      
      // Berechnung des Spannvektors y
      
      y = VectorR3::CrossProduct(x, b);
      y.Normalize();
      
      // Vektorrichtung bestimmen
      
      if(y[1] > 0.0)
	{
	  y *= -1.0;
	} // if
      
      if(b[2] <= 0.0)
	{
	  if(x[0] < 0.0)
	    {
	      x *= -1.0;
	    } // if
	} // if
      else
	{
	  if(x[0] > 0.0)
	    {
	      x *= -1.0;
	    } // if
	} // else
      
      // Berechnung der Vektorlänge
      
      laenge = ((float)tan((double)((float)angle/114.591559))*2.0)/(float)Res_x;
      
      x *= -laenge;
      y *= laenge;
      
      return(true);
    } // if
  else
    return(false); // Betrachter = Ziel (darf nicht sein)
} // CalcProPlane

VectorR3 CalcReflexion(const VectorR3& in, const VectorR3& normal)
{
} // CalcReflexion

VectorR3 CalcRefraction(const VectorR3& in, const VectorR3& normal, double i, double t)
{
} // CalcRefraction

double IntersectSphere(const VectorR3 center, const double radius, const VectorR3 observer, const VectorR3 direction)
{
  float t = -1;

  // intersect ray (observer, direction) with sphere (center, radius)
  // return line-parameter t in case of intersection, otherwise -1
  
  VectorR3 S = center - observer;

  float a, b, c;

  a = direction*direction;
  b = -2.0*direction*S;
  c = S*S - radius*radius;

  float t1, t2;

  float det = b*b - 4*a*c;

  if(det < 0) return -1;

  t1 = (-b + sqrt(det))/(2*a);
  t2 = (-b - sqrt(det))/(2*a);

  if(t1 > 0) t = t1;
  if((t2 > 0) && (t2 < t)) t = t2;

  return t;
} // IntersectSphere

void RenderImage()
{
  unsigned char red, green, blue;

  VectorR3 observer(0,0,-30);
  VectorR3 target(0,0,0);

  VectorR3 center_sphere(0,0,0);
  float    radius_sphere = 8;

  VectorR3 light_source = VectorR3(20, 60, -45);

  CalcProPlane(observer, target, window_width, window_height, 
	       60, center, span_x, span_y);
    
  for(int y = 0; y < window_height; y++) {
    for(int x = 0; x < window_width; x++) {

      VectorR3 pixel = center + (x-window_width/2)*span_x + (y-window_height/2)*span_y;

      VectorR3 sight = pixel - observer;
      
      double parameter = IntersectSphere(center_sphere, radius_sphere, observer, sight);

      VectorR3 cut = observer + parameter*sight;

      VectorR3 n = cut - center_sphere; n.Normalize();
      
      VectorR3 light_incoming = cut - light_source; light_incoming.Normalize();
      
      double shade = (-1)*n*light_incoming;

      if(shade < 0.1) shade = 0.1;
            
      VectorR3 color_sphere = shade*VectorR3(255, 192, 0);

      if(color_sphere[0] > 255) color_sphere[0] = 255;
      if(color_sphere[1] > 255) color_sphere[1] = 255;
      if(color_sphere[2] > 255) color_sphere[2] = 255;
      
      if(parameter > 0) SetPixelColor(x, y, color_sphere[0], color_sphere[1], color_sphere[2]);
      else
	SetPixelColor(x, y, 0, 0, 32);

    } // for		    
  } // for  
} // RenderImage

int main(int argc, char** argv)
{
    SDL_Event    event;
    SDL_Rect     dst_rect, full_image_rect, scrolled_rect, location;
    long         pixel_set = 0;

    window_width = 350; window_height = 350;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
	cerr << "Could not initialize SDL video" << endl;
	exit(-1);
    } // if
    atexit(SDL_Quit);

    Uint32 flags = 0;

    // flags |= SDL_FULLSCREEN;

    screen = SDL_SetVideoMode(window_width, window_height, 24, flags);

    if(!screen) {
	cerr << "could not SetVideoMode() - abort" << endl;
	exit(-1);
    } // if

    RenderImage();
    
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, window_width, window_height);
    SDL_LockSurface(screen);
    
    for(;;) {
      
      while(SDL_WaitEvent(&event) == 0) {
      } // while
      /*
	if(( == 0) && (event.type == SDL_KEYDOWN)) 
	    goto exit_app;
      */
	switch(event.type) {
		
	    case SDL_MOUSEMOTION:	      
		break;
		
	    case SDL_MOUSEBUTTONDOWN:
		break;
		
	    case SDL_MOUSEBUTTONUP:
		break;
		
	    case SDL_KEYDOWN:
		
		switch(event.key.keysym.sym) {
		
		case '+':
		  break;
		  
		case '-':
		  break;
		    
		case ' ':
		  break;

		case 'q':
		case 'Q':
		  goto exit_app;
			
		default:
		  break;
		} // switch
	} // switch

    } // for
	    
 exit_app:

    SDL_UnlockSurface(screen);	    
    SDL_FreeSurface(screen);

    exit(0);
} // main
