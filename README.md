# MazeSolver #
MazeSolver solves mazes, which are automatically read and interpreted from jpg images. As input it takes a jpg image and the coordinates of the beginning and end of the maze.
It outputs a file called "output.jpg" with the path through the maze drawn on top of the input maze image.

The maze images need to fulfill some conditions:
- the walls need to be black or at least darker than the background,
- there cen be no "tunnels" or "overpassess" in the maze.

## Examples ##
There are two example mazes included in the project. Both are distributed under the creative commons license, the original images are available here:  
maze1.jpg: https://en.wikipedia.org/wiki/Maze#/media/File:Circularmazeexample.jpg  
maze2.jpg: https://upload.wikimedia.org/wikipedia/commons/3/32/MAZE_2000x2000_DFS.png 

In order to see an example of a solved maze visit my website, here:  
http://home.elka.pw.edu.pl/~mkowals6/doku.php?id=wiki:hobby

## How to run it ##
MazeSolver is a console application, to run it you need to open the console and browse to the location of the app.  
The program can be ran with the following command:

MazeSolver.exe IMAGE_FILE_NAME.jpg START_X START_Y END_X END_Y [SAVE_COST_MAP]

Where:  
IMAGE_FILE_NAME.jpg is the name of the image file with the maze.  
START_X/Y are the pixel coordinates of the maze entrance.  
END_X/Y are the pixel coordinates of the maze exit.  
[SAVE_COST_MAP] is an additional paramter, if added the program saves the maze cost map instead of the path through the maze.

So to run the app with one of the example mazes you need to write:
MazeSolver.exe maze1.jpg 224 177 633 620  
or for the other example:  
MazeSolver.exe maze2.jpg 1 1 4000 3999

## Licensing ##
All of the code except the jpg compression/decompression is licensed under the MIT license.
The jpgd library which is used for reading and writing jpg images is in the public domain, more info here:
https://code.google.com/p/jpeg-compressor/
