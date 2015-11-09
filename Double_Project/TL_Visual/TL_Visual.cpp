// TL_Visual.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

EKeyCode quitKey = Key_Escape;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* gameEngine = New3DEngine( kTLX );
	gameEngine->StartWindowed();

	// Add default folder for meshes and other media
	gameEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* cam = gameEngine->CreateCamera(kFPS);
	IMesh* foo = gameEngine->LoadMesh("Box.x");
	IModel* bar = foo->CreateModel();

	// The main game loop, repeat until engine is stopped
	while (gameEngine->IsRunning())
	{
		if (gameEngine->KeyHit(Key_Escape))
		{
			
		}

		// Draw the scene
		gameEngine->DrawScene();

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();
}
