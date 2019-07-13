This is a collection of projects with the HX8347D TFT touchscreen shield for Arduino. I've made some optimizations to the TFT library to draw rectangles faster so copy the HX8347D folder to the Arduino libraries directory before running these.

- Game of Life
  This is a cellular automaton following the default rules of the game. The simulation starts with the [R-pentonimo ](http://www.conwaylife.com/wiki/R-pentomino)

  To Do
   - Use flash memory to store the matrices so the entire screen is used and RAM is freed up. Right now it covers 128 x 124 pixels and uses almost all of the RAM available

- 3D Cube Projection
  A 3D cube is rotated and is projected on a 2D plane which is rendered on the screen