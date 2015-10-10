--------------------------------------------
* Authors:	Joshua Gray, Randy Brunecz *
* Last Updated:	10/10/2015		   *					   
--------------------------------------------


DESCRIPTION
--------------------------------------------


CONTROLS
--------------------------------------------
Esc   : Quits the game (Will change when the game develops more)
F11   : Resizes the window to windowed mode or fullscreen mode depending on which mode it was in previously.
W     : Moves sprite up
S     : Moves sprite down
D     : Moves sprite right
A     : Moves sprite left
Space : Pauses the game

DEBUG CONTROLS
1  : Perform a fade in transition
2  : Perform a fade out transition
TAB: Displays collision boundaries for objects in the game
G  : Displays grid lines to show where tiles start and finish

PERFORMANCE
--------------------------------------------
V-Sync has been enabled for the game. If you notice your CPU usage is 20%+, and you use an Nvidia card, go into the Nvidia control panel and turn off "Threaded Optimization".

GETTING THE CODE TO COMPILE AND RUN
--------------------------------------------
1. Make sure your project name is highlighted in the Solution Explorer

2. Go to "PROJECT" tab at the top and click on "Properties"

3. Under "Configuration Properties", click on "C/C++" and click "General". Make sure "All Configurations" is selected in the drop down called "Configuration" at the top left.

4: At the top where it says "Additional Include Directories", copy your path to SFML's "include" folder and paste it here.

5. Click on "Preprocessor" which is under "C/C++" and copy: "SFML_STATIC" into the box that is called "Preprocessor Definitions"

6. Now navigate to "Linker" under "Configuration Properties" and click "General". Copy the path to the SFML "lib" folder and paste it where it says "Additional Library Directories". (9th box down)

7. Navigate to "Input" under "Linker".

8. Now, switch "Configuration" to "Debug". Click on "Additional Dependencies" and copy these to the text box:

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