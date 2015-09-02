--------------------------------------------
* Authors:	Joshua Gray, Randy Brunecz *
* Last Update:	9/1/2015		   *					   
--------------------------------------------


DESCRIPTION
--------------------------------------------


CONTROLS
--------------------------------------------
Esc: Quits the game (Will change when the game develops more)


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