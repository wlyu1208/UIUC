#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "Image.h"
#include "StickerSheet.h"

using namespace cs225;
using namespace std;

StickerSheet::StickerSheet(const Image &picture, unsigned max){
  const_max=max;

  list_images=new Image* [const_max];
  original=&picture;
  result=*original;

  xpos=new int[const_max];
  ypos=new int[const_max];

  for(unsigned int i=0; i<const_max; i++){
    list_images[i]=NULL;
    xpos[i]=0;
    ypos[i]=0;
  }
}

StickerSheet::~StickerSheet(){
  this->destroy();
}

StickerSheet::StickerSheet(const StickerSheet & other){
  copy_const(other);
}

const StickerSheet& StickerSheet::operator=(const StickerSheet& other){
if(this!=&other){
  this->destroy();
  this->copy_const(other);
  return *this;
}
return *this;
}

void StickerSheet::changeMaxStickers(unsigned max){
  if(const_max==max){
  return;
}
  int* new_xpos=new int[max];
  int* new_ypos=new int[max];
  Image** result=new Image*[max];

  if(max<const_max){
    for(unsigned int i=0; i<max; i++){
      if(list_images[i]!=NULL){
        result[i]=new Image();
        result[i]=list_images[i];
        new_xpos[i]=xpos[i];
        new_ypos[i]=ypos[i];
      }
      if(list_images[i]==NULL){
        result[i]=NULL;
        new_xpos[i]=xpos[i];
        new_ypos[i]=ypos[i];
      }
    }
  }

  if(max>const_max){
    for(unsigned int i=0; i<const_max; i++){
      if(list_images[i]!=NULL){
        result[i]= new Image();
        result[i]=list_images[i];
        new_xpos[i]=xpos[i];
        new_ypos[i]=ypos[i];
      }
      if(list_images[i]==NULL){
        result[i]=NULL;
        new_xpos[i]=xpos[i];
        new_ypos[i]=ypos[i];
      }
    }
    for(unsigned int i=const_max; i<max; i++){
      result[i]=NULL;
      new_xpos[i]=0;
      new_ypos[i]=0;
    }
  }

  delete[] list_images;
  delete[] xpos;
  delete[] ypos;

  list_images=result;
  const_max=max;
  xpos=new_xpos;
  ypos=new_ypos;
}


int StickerSheet::addSticker(Image& sticker, unsigned x, unsigned y){
  int count=0;
  unsigned int i=0;
  for(i=0; i<const_max; i++){
    if(list_images[i]==NULL){
      list_images[i]=new Image(sticker);
      xpos[i]=x;
      ypos[i]=y;
      count=1;
      break;
    }
  }
  if(count!=0){
    return count;
  }
  return -1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y){
  if((list_images[index]==NULL) || (index>=const_max)){
    return false;
  }
  xpos[index]=x;
  ypos[index]=y;
  return true;
}

void StickerSheet::removeSticker(unsigned index){
    delete list_images[index];
    list_images[index]=NULL;
}

Image* StickerSheet::getSticker(unsigned index) const{
  if(list_images[index]==NULL){
    return NULL;
  }
  return list_images[index];
}

Image StickerSheet::render() const{
  Image result;
  result=*original;
  unsigned int total_maxwidth=0;
  unsigned int total_maxheight=0;

  for(unsigned int i=0; i<const_max; i++){
    if(list_images[i]==NULL){
      break;
    }
    if(list_images[i]->width()>total_maxwidth){
      total_maxwidth=list_images[i]->width();
    }
    if(list_images[i]->height()>total_maxheight){
      total_maxheight=list_images[i]->height();
    }
  }

  for(unsigned int i=0; i<const_max; i++){
    unsigned int change_width=result.width()+xpos[i];
    unsigned int change_height=result.height()+ypos[i];
    if(total_maxwidth>result.width() && total_maxheight>result.height()){
      result.resize(change_width, change_height);
    }
    else if(total_maxwidth>result.width()){
      result.resize(change_width, result.height());
    }
    else if(total_maxheight>result.height()){
      result.resize(result.width(), change_height);
    }
  }

  for(unsigned int x=0; x<const_max; x++){
    if(list_images[x]!=NULL){
      for(unsigned int i=0; i<list_images[x]->width(); i++){
        for(unsigned int j=0; j<list_images[x]->height(); j++){
          if(i+xpos[x]<result.width() && j+ypos[x]<result.height()){
            HSLAPixel& image_pixel=list_images[x]->getPixel(i, j);
            HSLAPixel& result_pixel=result.getPixel(i+xpos[x], j+ypos[x]);
            if(image_pixel.a != 0){
              result_pixel=image_pixel;
              }
            if(image_pixel.a == 0){
              result_pixel.a=1;
              }
            }
          }
        }
      }
    }
  return result;
}

void StickerSheet::copy_const(const StickerSheet &other){
  const_max=other.const_max;
  original=other.original;
  xpos=new int[const_max];
  ypos=new int[const_max];
  list_images=new Image*[const_max];

  for(unsigned int i=0; i<const_max; i++){
    if(other.list_images[i]!=NULL){
      list_images[i]=new Image();
      *list_images[i]=*(other.list_images[i]);
      xpos[i]=other.xpos[i];
      ypos[i]=other.ypos[i];
    }
    if(other.list_images[i]==NULL){
      list_images[i]=NULL;
      xpos[i]=other.xpos[i];
      ypos[i]=other.ypos[i];
    }
  }
}

void StickerSheet::destroy(){
  for(unsigned int i=0; i<const_max; i++){
    delete list_images[i];
    xpos[i]=0;
    ypos[i]=0;
  }
  delete[] list_images;
  delete[] xpos;
  delete[] ypos;
}
