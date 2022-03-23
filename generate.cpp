/*    
		Tx2Px 2008,Wojciech Musialkiewicz(gambiting)

		This file is part of Tx2Px source code.

    Tx2Px is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Tx2Px is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tx2Px; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "generate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
using namespace std;
generate::generate()
{

}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )		//copied from lazy foo's tutorial
{
 //Make a temporary rectangle to hold the offsets 
	SDL_Rect offset;
 //Give the offsets to the rectangle 
	offset.x = x; 
	offset.y = y;
	SDL_BlitSurface( source, NULL, destination, &offset );
}
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)		//copied straight from SDL docs
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

int generate::render(string txt,string name)
{
if(txt.size()>128)					//if the text is too long we quit
	return 5;
int i2=0;						
string txt2;
SDL_Surface *image = NULL;				//it will be our rendered text
SDL_Surface *image2 = NULL;				//the same text but backwards - for disguise :D
SDL_Surface *background = NULL;				// background 
SDL_Color textColor = { 200, 200, 200 };		//color of rendered text
SDL_Color textColor2 = { 255, 50, 50 };			//color of rendered text
SDL_Color backColor = { 0, 0, 0 };			//color of text shade - you have to use it to enable alfa chanel!
TTF_Font *font = NULL;					//variable for our font
FILE * pFile;
char buffer[128] = {0,};
char buffer_image[128] = {0,};						
int bkW=0;						//background width
int bkH=0;						// background height
Uint32 color;						//random color for background

if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)		//sdl init - in case of error we quit
	return 1;
if(TTF_Init() == -1) 					//ttf init - in case of error we quit
	return 2;
for(int i=0;i<txt.size();i++)				//converting to uppercase
	txt[i]=toupper(txt[i]);
txt2=txt;
font = TTF_OpenFont( "ace.ttf", 40 );			//opening font
for(int i=txt.size()-1;i>=0;i--)			// reversing txt to txt2
{
	txt2[i]=txt[i2];
	++i2;
}
for(int i=0;i<txt.size();i++)				//writing txt to buffer,as TTF_RenderText_Shaded takes only const char* 
	buffer[i]=txt[i];
image = TTF_RenderText_Shaded( font, buffer, textColor,backColor );	//rendering original image
for(int i=0;i<txt.size();i++)				//writing txt2 to buffer
	buffer[i]=txt2[i];
image2 = TTF_RenderText_Shaded( font, buffer, textColor2,backColor );	//rendering second image
SDL_SetColorKey( image, SDL_SRCCOLORKEY, 0x000000 );	//setting black as alpha for both original and second images
SDL_SetColorKey( image2, SDL_SRCCOLORKEY, 0x000000 );
image = rotozoomSurface(image, 15, 1, 1);		//we rotate original image by 15 degrees
bkW = image->w+40;					//we get width and height of original rendered image - we add 40 pixels,so we will be able to center it
bkH = image->h+40;

background = SDL_CreateRGBSurface(SDL_SWSURFACE, bkW, bkH, 32, 0, 0, 0, 255);	//we create new surface with bkW and bkH dimensions

for(int y=0;y<bkH;y++)					//rendering noise on background
{
	for(int x=0;x<bkW;x++)
	{
		color = SDL_MapRGB(background->format, rand()%255, rand()%255, rand()%255);
		putpixel(background,x,y,color);
	}
}
for(int i=0;i<300;i++)					//even more noise with black lines all over :D
{
	lineRGBA(background, rand()%bkW, rand()%bkH, rand()%bkW, rand()%bkH, rand()%30, rand()%30, rand()%30, 255);
}
for(int i=0;i<10;i++)					//now we apply second text in random places for disguise
{
	apply_surface( rand()%(bkW/2-30), rand()%bkH-30, image2, background );
}

apply_surface( 20, 20, image, background );		//and finally we can apply original image on the centre of our prepared background

for(int i=0;i<128;i++) buffer[i]=0;			//empty buffer
for(int i=0;i<name.size();i++) buffer[i] = name[i];	//rewrite name to buffer
sprintf(buffer_image,"%s.bmp",buffer);			//so now we can make a full file name - for image file



if(SDL_SaveBMP(background,buffer_image) == -1)		//we save result - it there is any error we quit
	return 3;

sprintf(buffer,"%s.txt",buffer);			//now we can make a full file name  -for text file
pFile = fopen (buffer,"w");
if(pFile == NULL) return 4;				//if we can't open txt file for writing we quit

for(int i=0;i<128;i++) buffer[i]=0;			//and again,we have to empty buffer
for(int i=0;i<txt.size();i++) buffer[i] = txt[i];	//rewrite txt to buffer

fputs (buffer,pFile);					//we write our rendered text to txt file for comparison
fclose (pFile);						//we close our txt file - very important!

SDL_FreeSurface( image );				//we release surfaces - that's very important!
SDL_FreeSurface( background );
SDL_Quit();						//SDL_Quit and return 0 - we are done!
return 0;

}
