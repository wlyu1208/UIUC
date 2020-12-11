#include "cs225/PNG.h"
using cs225::PNG;

#include "cs225/HSLAPixel.h"
using cs225::HSLAPixel;

#include <string>



void rotate(std::string inputFile, std::string outputFile) {
  // TODO: Part 2
  PNG* original = new PNG(); //create space for image
  original->readFromFile(inputFile); //get image

  unsigned width = original->width();
  unsigned height = original->height(); //get image width and height

  PNG* output = new PNG(width, height); //create space for outputFile

  for(unsigned int x=0; x<width; x++){
    for(unsigned int y=0; y<height; y++){

      HSLAPixel& curr = original->getPixel(x, y);
      HSLAPixel& brand_new = output->getPixel(width-1-x, height-1-y);

      brand_new = curr;
    }
  }
  // Save the output file
  output->writeToFile(outputFile);
  //clean up memory
  delete original;
  delete output;
}

PNG myArt(unsigned int width, unsigned int height) {
  PNG png(width, height);
  // TODO: Part 3
for(unsigned int x=0; x<width; x++){
  for(unsigned int y=0; y<height; y++){
    HSLAPixel& italy_flag = png.getPixel(x, y);
    if(0<x && x<(width/3)){
      italy_flag.h=108;
      italy_flag.s=1.0;
      italy_flag.l=0.19;
      italy_flag.a=1.0;
    }
    if((width/3)<x && x<(2*width/3)){
      italy_flag.l=1.0;
    }
    if((2*width/3)<x && x<width){
      italy_flag.h=0;
      italy_flag.s=1.0;
      italy_flag.l=0.5;
      italy_flag.a=1.0;
    }
  }
}
  return png;
}
