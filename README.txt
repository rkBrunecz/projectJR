-----------------------------------------------------------------
* Developers:	Randall Brunecz, Joshua Gray			*
* Last Updated:	03/28/2016		   			*					   
-----------------------------------------------------------------

DESCRIPTION
--------------------------------------------
Game description will be updated when the game developes more. Currently, the project is focused on building the tools and the engine to run the game. The engine is being build from scratch utilizing the SFML library. The scope of the project is difficult to measure since the features and additions are decided on a regular basis. Initially, the game was going to be very simple with a simplistic engine. As time goes on, the complexity increases and I realize I want to do more with the game.

CONTROLS
--------------------------------------------
Esc             : Quits the game (Will change when the game develops more)
F11             : Resizes the window to windowed mode or fullscreen mode depending on which mode it was in previously. (Game will not run well in windowed mode)
W / Up Arrow    : Moves sprite up
S / Down Arrow  : Moves sprite down
D / Right Arrow : Moves sprite right
A / Left Arrow  : Moves sprite left
Space           : Pauses the game

DEBUG CONTROLS
1  		: Perform a fade in transition
2  		: Perform a fade out transition
TAB		: Displays collision boundaries for objects in the game
G  		: Displays grid lines to show where tiles start and finish
T  		: Displays all the transition points on the map

PERFORMANCE
--------------------------------------------
V-Sync has been enabled for the game. If you notice your CPU usage is 20%+, and you use an Nvidia card, go into the Nvidia control panel and turn off "Threaded Optimization".

GETTING THE CODE TO COMPILE AND RUN
--------------------------------------------
1. Download SFML http://www.sfml-dev.org/download.php

2. Make sure your project name is highlighted in the Solution Explorer

3. Go to "PROJECT" tab at the top and click on "Properties"

4. Under "Configuration Properties", click on "C/C++" and click "General". Make sure "All Configurations" is selected in the drop down called "Configuration" at the top left.

5: At the top where it says "Additional Include Directories", copy your path to SFML's "include" folder and paste it here.

6. Click on "Preprocessor" which is under "C/C++" and copy: "SFML_STATIC" into the box that is called "Preprocessor Definitions"

7. Now navigate to "Linker" under "Configuration Properties" and click "General". Copy the path to the SFML "lib" folder and paste it where it says "Additional Library Directories". (9th box down)

8. Navigate to "Input" under "Linker".

9. Now, switch "Configuration" to "Debug". Click on "Additional Dependencies" and copy these to the text box:

sfml-graphics-s-d.lib
freetype.lib
jpeg.lib
sfml-window-s-d.lib
opengl32.lib
winmm.lib
gdi32.lib
sfml-system-s-d.lib
sfml-audio-s-d.lib
openal32.lib
flac.lib
vorbisenc.lib
vorbisfile.lib
vorbis.lib
ogg.lib

9. Finally, switch "Configuration" to "Release". Click on "Additional Dependencies" and copy these to the text box:

sfml-graphics-s.lib
freetype.lib
jpeg.lib
sfml-window-s.lib
opengl32.lib
winmm.lib
gdi32.lib
sfml-system-s.lib
sfml-audio-s.lib
openal32.lib
flac.lib
vorbisenc.lib
vorbisfile.lib
vorbis.lib
ogg.lib

MAP EDITOR
--------------------------------------------

Map editor will allow for quick content creation and the implementation of interesting maps.

CONTROLS
--------------------------------------------
ESC 		   : Closes the application
L  	  	   : Allows the user to load in a map
N  		   : Allows the user to create a new map 
S  		   : Allows the user to save a map
TAB		   : Displays collision boundaries for objects in the game
G  		   : Displays grid lines to show where tiles start and finish
T  		   : Displays all the transition points on the map
U  		   : Forces the editor to update all layers
R		   : Allows the user to perform a test run from the current map being worked on. 

CAMERA CONTROLS
Right Mouse Button : When a map is created or loaded, right clicking and holding will allow for the user to drag the camera. The further the user pulls, the faster the camera moves.
Left Mouse Button  : Click a tile in the tile pane (right side of the window) to select a tile. A transparent yellow box will highlight a tile if it is valid. Left click anywhere in the map to place the tile.

REFERENCES:
--------------------------------------------
http://www.sfml-dev.org/ ~ Please checkout SFML! It truly is an amazing library! 