/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
#include <vector>
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

    MosaicCanvas* pic=new MosaicCanvas(theSource.getRows(), theSource.getColumns()); //canvase with correct width and column
    vector<Point<3>> tileimage; //vector to push pixel
    size_t i=0;
    size_t big=theTiles.size(); //set variables to use
    map<Point<3>, int> mapping; //map to put vector

    for(i=0; i<big; i++){ //loop on tile size
      LUVAPixel pixel=theTiles[i].getAverageColor(); //get average color of pixel
      Point<3> x(convertToXYZ(pixel)); //get 3 points
      tileimage.push_back(x); //push points on vector
      mapping[tileimage[i]]=i; //push each vector point to map;
    }

    KDTree<3> tree(tileimage);
    int r, c;
    int width=theSource.getRows();
    int height=theSource.getColumns(); //initialize variables

    for(r=0; r<width; r++){
      for(c=0; c<height; c++){ //loops on width and height
        LUVAPixel temp=theSource.getRegionColor(r, c); //get pixel of region
        Point<3> place=tree.findNearestNeighbor(convertToXYZ(temp)); //get points on tree neighbor with points
        pic->setTile(r, c, &theTiles[mapping[place]]); //set tile with correct color
      }
    }
    return pic; //return the final picture
}
