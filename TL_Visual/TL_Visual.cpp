// TL_Visual.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* gameEngine = New3DEngine( kTLX );
	gameEngine->StartWindowed();

	// Add default folder for meshes and other media
	gameEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );
	gameEngine->AddMediaFolder(".\\Media");

	/**** Set up your scene here ****/


	// The main game loop, repeat until engine is stopped
	while (gameEngine->IsRunning())
	{
		// Draw the scene
		gameEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//IMesh* foo;
		//foo->BeginEnumVertices //This lets you find out the vertex data

			IModel* bar;
			bar->rota
	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();
}
