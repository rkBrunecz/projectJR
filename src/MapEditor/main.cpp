// Headers
//#include "vld.h" // Check for memory leaks. Comment out if not being used
#include "MapEditor/Map_Editor.h"

int main()
{
	// Local variables
	std::string versionNum = "v.2.0a";
	Map_Editor editor(versionNum);

	editor.runEditor();

	return 0; //Close the game
}