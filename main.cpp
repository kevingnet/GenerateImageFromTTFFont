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


#include <stdio.h>
#include <stdlib.h>
#include "generate.h"						//generate.h is our header file
#include <iostream>
#include <string>
#include <time.h>
int main(int argc,char* argv[])
{
	int length;						//length of the text to render
	char gen[128];						//array for a rendom text to render(if -r is used)
	bool random = false;
	std::string name;
	std::string txt;
	std::string buffer;
	srand(time(NULL));
	for(int i=0;i<128;i++)
		gen[i]=NULL;
	if(argc >1)
	{
		name = "image";
		for(int i=1;i<argc;i++)
		{
			buffer=argv[i];
			
			if(buffer == "-t")
			{
				txt = argv[i+1];
				
				++i;
			}
			if(buffer == "-r")
			{
				printf("Randomizing!\n");
				random = true;
			}
			if(buffer == "-l")
			{
				length = atoi(argv[i+1]);
				i++;
			}
			if(buffer == "-s")
			{
				name= argv[i+1];
				i++;
			}
					
			
		}
		
		if(random)					//if user selected -r option we generate random text with specified length
		{
			for(int i=0;i<length;i++)
			{
				gen[i] = rand()%25+65;		
				
				txt=gen;
			}
		}
		std::cout << "String to render:" <<  txt << std::endl;
		std::cout << "Saving image to:" <<  name << ".bmp" << std::endl;
		std::cout << "Saving text file to:" <<  name << ".txt" << std::endl;
		
		generate engine;				//our class object
		
		switch (engine.render(txt,name))		//we try to render text,then we parse the output
		{
			case 0:
				printf("Done!\n");		//function returned 0 - no errors
				break;
			case 1:
				printf("SDL INIT ERROR!\n");	//function returned 1 - sdl init error
				break;
			case 2:
				printf("TTF INIT ERROR!\n");	//function returned 2 - ttf init error
				break;
			case 3:
				printf("Can't save image - check write permissions !\n");	//function returned 3 - image save error
				break;
			case 4:
				printf("Can't save txt file - check write permissions !\n");	//function returned 4 - text save error
				break;
			case 5:
				printf("Text to render is too long!\n");			//function returned 5 - text for rendering is too long
				break;
			
		}
			
	}
	else 							//used entered no arguments - we print program help
	{
		printf("Tx2Px 2008 by Wojciech Musialkiewicz (gambiting)\n");
		printf("\tTx2Px is an open source program for rendering text unreadable with OCR programs.\n"
				"\tSuch text may be used on web pages for verification purposes"
				"\n\tTx2Px is distributed on GPL license,and you must accept it if you want to use it."
				"\n\tFull text of this licence is included in GPL.txt file supplied with this program.\n");
		printf("Usage:\n"
				"\t Tx2Px -t -s -r -l\n"
				"\t\t -t <...>\t- use to render specified text,eg. \"-t foobar\" will render text foobar\n"
				"\t\t -s <...>\t- use to save image under specified filename.Default is image.bmp.\n"
				"\t\t -r\t\t- use to randomize text.If you use it,you MUST suply lenght of text with -l\n"
				"\t\t -l <...>\t- use to set length of rendered text - use only with -r option.Max size is 128\n"); 
		
	}
	

	


}
