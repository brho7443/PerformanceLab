#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Filter.h"

using namespace std;

#include "rdtsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int
main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

class Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  } else {
    cerr << "Bad input in readFilter:" << filename << endl;
    exit(-1);
  }
}


double applyFilter(class Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
     long long cycStart, cycStop;
    
    cycStart = rdtscll();

    short h = input -> height;
    short w = input -> width;
    output -> width = w;
    output -> height = h;
    char get00 = filter -> get(0,0);
    char get01 = filter -> get(0,1);
    char get02 = filter -> get(0,2);
    char get10 = filter -> get(1,0);
    char get11 = filter -> get(1,1);
    char get12 = filter -> get(1,2);
    char get20 = filter -> get(2,0);
    char get21 = filter -> get(2,1);
    char get22 = filter -> get(2,2);
    char divisor = filter -> getDivisor();
    short temp;
 for(int row = 1; row < h - 1 ; row = row + 1) {
  for(int col = 1; col < w - 1; col = col + 1) {
            
         // plane = 0
            temp = 0;
                
            temp += ((input -> color[0][row  - 1][col + 0 - 1])*get00);
            temp +=((input -> color[0][row- 1][col + 1 - 1])*get01);
            temp +=((input -> color[0][row - 1][col + 2 - 1])*get02);
            temp +=((input -> color[0][row ][col + 0 - 1])*get10);
            temp +=((input -> color[0][row ][col + 1 - 1])*get11);
            temp +=((input -> color[0][row ][col + 2 - 1])*get12);
            temp +=((input -> color[0][row +1][col + 0 - 1])*get20);
            temp +=((input -> color[0][row + 1][col + 1 - 1])*get21);
            temp +=((input -> color[0][row + 1][col + 2 - 1])*get22);
          
	
	
        temp = 	temp / divisor;

	if ( temp  < 0 ) {
	  temp = 0;
	}

	if ( temp  > 255 ) { 
	  temp = 255;
	}
      
    output -> color[0][row][col] = temp;   
    // plane = 1
    
          temp = 0;
          
            temp += ((input -> color[1][row + 0 - 1][col + 0 - 1])*get00);
            temp +=((input -> color[1][row + 0 - 1][col + 1 - 1])*get01);
            temp +=((input -> color[1][row + 0 - 1][col + 2 - 1])*get02);
            temp +=((input -> color[1][row + 1 - 1][col + 0 - 1])*get10);
            temp +=((input -> color[1][row + 1 - 1][col + 1 - 1])*get11);
            temp +=((input -> color[1][row + 1 - 1][col + 2 - 1])*get12);
            temp +=((input -> color[1][row + 2 - 1][col + 0 - 1])*get20);
            temp +=((input -> color[1][row + 2 - 1][col + 1 - 1])*get21);
            temp +=((input -> color[1][row + 2 - 1][col + 2 - 1])*get22);
          
	
	
        temp = 	temp / divisor;

	if ( temp  < 0 ) {
	  temp = 0;
	}

	if ( temp  > 255 ) { 
	  temp = 255;
	}
    
    output -> color[1][row][col] = temp;
    
    // plane = 2
          temp = 0;
          
            temp += ((input -> color[2][row + 0 - 1][col + 0 - 1])*get00);
            temp +=((input -> color[2][row + 0 - 1][col + 1 - 1])*get01);
            temp +=((input -> color[2][row + 0 - 1][col + 2 - 1])*get02);
            temp +=((input -> color[2][row + 1 - 1][col + 0 - 1])*get10);
            temp +=((input -> color[2][row + 1 - 1][col + 1 - 1])*get11);
            temp +=((input -> color[2][row + 1 - 1][col + 2 - 1])*get12);
            temp +=((input -> color[2][row + 2 - 1][col + 0 - 1])*get20);
            temp +=((input -> color[2][row + 2 - 1][col + 1 - 1])*get21);
            temp +=((input -> color[2][row + 2 - 1][col + 2 - 1])*get22);
          
              
            
	
	
        temp = 	temp / divisor;

	if ( temp  < 0 ) {
	  temp = 0;
	}

	if ( temp  > 255 ) { 
	  temp = 255;
	}
     
      output -> color[2][row][col] = temp;
    }
  }

    cycStop = rdtscll();
    double diff = cycStop - cycStart;
    double diffPerPixel = diff / (output->width * output->height);
    fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
        diff, diff / (output->width * output->height));
    return diffPerPixel;
}


