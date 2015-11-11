// TL_Visual.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include "GSceneManager.h"
using namespace gen;
EKeyCode quitKey = Key_Escape;

const int kNoAgents = 2500;
const gen::CVector2 kWorldSize = CVector2(2000.0f, 2000.0f);
const float kTimeStep = 1.0f / 30.0f;
const int kXSubDiv = 20;
const int kYSubDiv = 20;

float frameTime = 0;

struct SAgent
{
	IModel* model;
	UID id;
};

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* gameEngine = New3DEngine(kTLX);
	gameEngine->StartWindowed();

	// Add default folder for meshes and other media
	gameEngine->AddMediaFolder( ".\\Media" );

	IMesh* floorMesh = gameEngine->LoadMesh("Floor.x");
	IModel* floorModel = floorMesh->CreateModel(kWorldSize.x / 2.0f, 0.0f, kWorldSize.y / 2.0f);
	IMesh* agentMesh = gameEngine->LoadMesh("Box.x");
	
	SAgent AGENTS[kNoAgents];
	
	//--------------------------
	// CrowdDynamics Setup
	//--------------------------
	GSceneManager* CrowdDynamics = new GSceneManager(kTimeStep, kWorldSize, kXSubDiv, kYSubDiv);

	for (int i = 0; i < kNoAgents; i++)
	{
		//Use the Y Column as the height (0 column), CrowdDynamics uses X and Y as this will use X and Z
		AGENTS[i].model = agentMesh->CreateModel(i * (kWorldSize.x /kNoAgents), 0.0f, i * (kWorldSize.y / kNoAgents));
		AGENTS[i].id = CrowdDynamics->AddAgent(AGENTS[i].model->GetX(), AGENTS[i].model->GetZ(), true);
	}

	/**** Set up your scene here ****/
	ICamera* cam = gameEngine->CreateCamera(kFPS, kWorldSize.x/2.0f, 2200.0f, kWorldSize.y/2.0f);
	cam->RotateX(90.0f);
	cam->SetMovementSpeed(400.0f);
	cam->SetRotationSpeed(180.0f);

	gen::CMatrix3x3 tempAgentMat;
	float tempModelMat[16];
	// The main game loop, repeat until engine is stopped
	while (gameEngine->IsRunning())
	{
		if (gameEngine->KeyHit(Key_Escape))
		{
			gameEngine->Stop();
		}
		frameTime = gameEngine->Timer();
		// Draw the scene
		gameEngine->DrawScene();

		CrowdDynamics->Update(frameTime);

		for (int i = 0; i < kNoAgents; i++)
		{
			if (CrowdDynamics->GetAgentMatrix(AGENTS[i].id, tempAgentMat))
			{
				//Convert agent matrix to model matrix
				AGENTS[i].model->GetMatrix(tempModelMat);
				tempModelMat[0] = tempAgentMat.e00;
				tempModelMat[2] = tempAgentMat.e01;
				tempModelMat[8] = tempAgentMat.e10;
				tempModelMat[10] = tempAgentMat.e11;
				tempModelMat[12] = tempAgentMat.e20;
				tempModelMat[14] = tempAgentMat.e21;
				AGENTS[i].model->SetMatrix(tempModelMat);
			}
		}


		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();

	delete CrowdDynamics;
}
