
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.

  PNG picture;
  picture.readFromFile("tests/pacman.png");

  FloodFilledImage img(picture);
  BFS bfs(picture, Point(50, 50), 0.2);
  MyColorPicker what(0.2);
  img.addFloodFill(bfs, what);
  Animation animation2=img.animate(1000);


  FloodFilledImage image(img);
  DFS dfs(picture, Point(100, 50), 0.4);
  MyColorPicker what2(0.001);
  image.addFloodFill(dfs, what2);

  Animation animation=image.animate(1000);

  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("myFloodFill.png");
  animation.write("myFloodFill.gif");



  return 0;
}
