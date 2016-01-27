// TL_Visual.cpp: A program using the TL-Engine

#define InfluenceVisualiserEnabled

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
#include <map>
#include <sstream>
#include <iomanip>
using std::stringstream;
using std::map;
#include "Picking.h"

#include "CrowdDynamics.hpp"
using namespace gen;
EKeyCode quitKey = Key_Escape;
EKeyCode pauseKey = Key_P;

const int kNoStartingAgents = 10;
const gen::CVector2 kWorldSize = CVector2(50.0f, 50.0f);
const float kTimeStep = 1.0f / 30.0f;
const int kXSubDiv = 3;
const int kYSubDiv = 3;

const int kInfluenceSubDivX = static_cast<int>(kWorldSize.x);
const int kInfluenceSubDivY = static_cast<int>(kWorldSize.y);



float frameTime = 0;

bool FindNearestAgent(map<UID, IModel*> agents, GSceneManager* manager, UID &nearestID, I3DEngine* gameEngine, ICamera* cam)
{
	CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
	CVector2 mousePosition = CVector2(tempPos.x, tempPos.z);

	CVector2 agentPosition;
	bool set = false;
	UID nearest;
	CVector2 nearestPosition;

	for (auto agent : agents)
	{
		if (manager->GetAgentPosition(agent.first, agentPosition))
		{
			if (!set)
			{
				nearest = agent.first;
				nearestPosition = agentPosition;
				set = true;
			}
			else if ((agentPosition - mousePosition).Length() < (nearestPosition - mousePosition).Length())
			{
				nearest = agent.first;
				nearestPosition = agentPosition;
			}
		}
	}

	nearestID = nearest;

	return set;
}

void CameraControls(I3DEngine* gameEngine, ICamera* cam)
{
	if (gameEngine->KeyHeld(Key_Numpad4))
	{
		cam->MoveX(-80.0f * frameTime);
	}
	if (gameEngine->KeyHeld(Key_Numpad6))
	{
		cam->MoveX(80.0f * frameTime);
	}
	if (gameEngine->KeyHeld(Key_Numpad2))
	{
		cam->MoveZ(-80.0f * frameTime);
	}
	if (gameEngine->KeyHeld(Key_Numpad8))
	{
		cam->MoveZ(80.0f * frameTime);
	}
	if (gameEngine->KeyHeld(Key_Numpad7))
	{
		cam->MoveY(-80.0f * frameTime);
	}
	if (gameEngine->KeyHeld(Key_Numpad9))
	{
		cam->MoveY(80.0f * frameTime);
	}
}

void UpdateAgentsFromCrowdData(GSceneManager* crowdEngine, map<UID, IModel*>& AGENTS, map<UID, IModel*>& DestinationVectors, map<UID, IModel*>& MovementVectors)
{
	gen::CMatrix3x3 tempAgentMat;
	float tempModelMat[16];

	for (auto agent : AGENTS)
	{
		if (crowdEngine->GetAgentMatrix(agent.first, tempAgentMat))
		{
			agent.second->GetMatrix(tempModelMat);
			tempModelMat[0] = tempAgentMat.e00;
			tempModelMat[2] = tempAgentMat.e01;
			tempModelMat[8] = tempAgentMat.e10;
			tempModelMat[10] = tempAgentMat.e11;
			tempModelMat[12] = tempAgentMat.e20;
			tempModelMat[14] = tempAgentMat.e21;
			agent.second->SetMatrix(tempModelMat);
		}
	}

	gen::CVector2 tempDestination;
	for (auto destinationVector : DestinationVectors)
	{
		if (crowdEngine->GetAgentDestination(destinationVector.first, tempDestination))
		{
			IModel* thisAgentModel = AGENTS.at(destinationVector.first);

			//Set the vector model to the agent model's position
			destinationVector.second->SetPosition(thisAgentModel->GetX(),
				thisAgentModel->GetY() + 10.0f,		//+10 to have to vector model rest atop the agent model
				thisAgentModel->GetZ());

			destinationVector.second->LookAt(tempDestination.x, 10.0f, tempDestination.y);
			
			CVector2 vectorToEnd = tempDestination - CVector2(thisAgentModel->GetX(), thisAgentModel->GetZ());
			destinationVector.second->ScaleZ(vectorToEnd.Length());
		}
	}

	for (auto movementVector : MovementVectors)
	{
		//Will only fetch the results of global collision avoidance
		if (crowdEngine->GetAgentDesiredVector(movementVector.first, tempDestination))
		{
			IModel* thisAgentModel = AGENTS.at(movementVector.first);

			//Set the vector model to the agent model's position
			movementVector.second->SetPosition(thisAgentModel->GetX(),
				thisAgentModel->GetY() + 11.0f,		//+11 to have to vector model rest atop the agent model
				thisAgentModel->GetZ());

			movementVector.second->LookAt(tempDestination.x + thisAgentModel->GetX(), 10.0f, tempDestination.y + thisAgentModel->GetZ());

			movementVector.second->ScaleZ(tempDestination.Length());
		}
	}
}

void UpdateAgentFromCrowdData(UID agentID, GSceneManager* crowdEngine, map<UID, IModel*>& AGENTS, map<UID, IModel*>& DestinationVectors, map<UID, IModel*>& MovementVectors)
{
	gen::CMatrix3x3 tempAgentMat;

	if (crowdEngine->GetAgentMatrix(agentID, tempAgentMat))
	{
		try		//Find the agent 'holding' and set the holding skin
		{
			float tempModelMat[16];
			IModel* thisAgentModel;
			thisAgentModel = AGENTS.at(agentID);
			thisAgentModel->GetMatrix(tempModelMat);
			tempModelMat[0] = tempAgentMat.e00;
			tempModelMat[2] = tempAgentMat.e01;
			tempModelMat[8] = tempAgentMat.e10;
			tempModelMat[10] = tempAgentMat.e11;
			tempModelMat[12] = tempAgentMat.e20;
			tempModelMat[14] = tempAgentMat.e21;
			thisAgentModel->SetMatrix(tempModelMat);
		}
		catch (std::out_of_range err)
		{
			cout << err.what();
		}
	}

	gen::CVector2 tempDestination;
	if (crowdEngine->GetAgentDestination(agentID, tempDestination))
	{
		IModel* thisVectorModel;
		IModel* thisAgentModel;
		thisAgentModel = AGENTS.at(agentID);
		thisVectorModel = DestinationVectors.at(agentID);
		
		//Set the vector model to the agent model's position
		thisVectorModel->SetPosition(thisAgentModel->GetX(),
		thisAgentModel->GetY() + 10.0f,
		thisAgentModel->GetZ());
		
		thisVectorModel->LookAt(tempDestination.x, 10.0f, tempDestination.y);

		CVector2 vectorToEnd = tempDestination - CVector2(thisAgentModel->GetX(), thisAgentModel->GetZ());
		thisVectorModel->ScaleZ(vectorToEnd.Length());
	}

	if (crowdEngine->GetAgentDesiredVector(agentID, tempDestination))
	{
		IModel* thisVectorModel;
		IModel* thisAgentModel;
		thisAgentModel = AGENTS.at(agentID);
		thisVectorModel = MovementVectors.at(agentID);

		//Set the vector model to the agent model's position
		thisVectorModel->SetPosition(thisAgentModel->GetX(),
			thisAgentModel->GetY() + 10.0f,
			thisAgentModel->GetZ());

		thisVectorModel->LookAt(tempDestination.x + thisAgentModel->GetX(), 10.0f, tempDestination.y + thisAgentModel->GetZ());

		thisVectorModel->ScaleZ(tempDestination.Length());
	}
}

void UpdateInfluenceFromCrowdData(GSceneManager* crowdEngine, IModel** influenceTiles)
{
	GInfluenceMap* influenceMap = crowdEngine->GetInfluenceMap();

	float influence;

	for (int i = 0; i < kInfluenceSubDivX; i++)
	{
		for (int j = 0; j < kInfluenceSubDivY; j++)
		{
			influence = influenceMap->GetValue(i, j);

			influenceTiles[i * kInfluenceSubDivX + j]->ResetScale();
			if (influence > 0)
			{
				influenceTiles[i * kInfluenceSubDivX + j]->ScaleY(2 * influence);
			}
		}
	}
}

void DrawPointingPosition(I3DEngine* gameEngine, ICamera* cam, IFont* theFont, stringstream &sstream)
{
	CVector3 mousePos = WorldPosFromPixel(gameEngine, cam);
	sstream << "X: " << std::setprecision(4) << mousePos.x;
	theFont->Draw(sstream.str(), 0.0f, 0.0f, kBlack);
	int height = theFont->MeasureTextHeight(sstream.str());
	sstream.str("");
	sstream << "Z: " << std::setprecision(4) << mousePos.z;
	theFont->Draw(sstream.str(), 0.0f, height, kBlack);
	sstream.str("");
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* gameEngine = New3DEngine(kTLX);
	gameEngine->StartWindowed();

	// Add default folder for meshes and other media
	gameEngine->AddMediaFolder( ".\\Media" );

	IFont* mousePosFont = gameEngine->LoadFont("Font1.bmp");


	IMesh* agentMesh = gameEngine->LoadMesh("Box.x");
	IMesh* floorTileMesh = gameEngine->LoadMesh("FloorTile.x");
	IMesh* vectorMesh = gameEngine->LoadMesh("Vector.x");

	map<UID, IModel*> AGENTS;
	map<UID, IModel*> DestinationVectors;
	map<UID, IModel*> MovementVectors;
	IModel** FloorTiles;
	
	FloorTiles = new IModel*[kXSubDiv * kYSubDiv];
	for (int i = 0; i < kXSubDiv; i++)
	{
		for (int j = 0; j < kYSubDiv; j++)
		{
			FloorTiles[i * kXSubDiv + j] = floorTileMesh->CreateModel(i * (kWorldSize.x / kXSubDiv), 0.0f, j * (kWorldSize.y / kYSubDiv));
			FloorTiles[i * kXSubDiv + j]->ScaleX(kWorldSize.x / kXSubDiv);
			FloorTiles[i * kXSubDiv + j]->ScaleZ(kWorldSize.y / kYSubDiv);
			FloorTiles[i*kXSubDiv + j]->Scale(1.0f);
		}
	}

#ifdef InfluenceVisualiserEnabled
	IModel** InfluenceTiles;
	
	InfluenceTiles = new IModel*[kInfluenceSubDivX * kInfluenceSubDivY];
	for (int i = 0; i < kInfluenceSubDivX; i++)
	{
		for (int j = 0; j < kInfluenceSubDivY; j++)
		{
			InfluenceTiles[i * kInfluenceSubDivX + j] = floorTileMesh->CreateModel(static_cast<float>(i) , 0.5f, static_cast<float>(j) );
			InfluenceTiles[i * kInfluenceSubDivX + j]->RotateX(180.0f);
		}
	}
#endif

	//--------------------------
	// CrowdDynamics Setup
	//--------------------------
	SWorldInfo worldData;
	worldData.TimeStep = kTimeStep;
	worldData.WorldSize = kWorldSize;
	worldData.xSubdivisions = kXSubDiv;
	worldData.ySubdivisions = kYSubDiv;
	GSceneManager* crowdEngine = GSceneManager::GetInstance(&worldData);
	vector<UID> IDs = crowdEngine->AddXAgents(kNoStartingAgents);
	for (int i = 0; i < kNoStartingAgents; i++)
	{
		//Use the Y Column as the height (0 column), CrowdDynamics uses X and Y as this will use X and Z
		//Where the model spawns is irrelevant as it's matrix is built from the CrowdDynamics matrix
		AGENTS.insert(make_pair(IDs[i], agentMesh->CreateModel()));
		DestinationVectors.insert(make_pair(IDs[i], vectorMesh->CreateModel()));
		MovementVectors.insert(make_pair(IDs[i], vectorMesh->CreateModel()));
	}
	for (auto item : MovementVectors)
	{
		item.second->SetSkin("vectortex2.jpg");
	}

	//ICamera* cam = gameEngine->CreateCamera(kFPS, kWorldSize.x/2.0f, 220.0f, kWorldSize.y/2.0f);
	ICamera* cam = gameEngine->CreateCamera(kManual, kWorldSize.x / 2.0f, 220.0f, kWorldSize.y / 2.0f);
	cam->RotateX(90.0f);
	cam->SetMovementSpeed(100.0f);
	cam->SetRotationSpeed(180.0f);

	float frameRate;
	stringstream frameStream;

	gen::CMatrix3x3 tempAgentMat;
	string tempString;
	int tempInt, tempInt2;
	float tempModelMat[16];

	//Assign the simulation matrices to the model matrices - first time
	UpdateAgentsFromCrowdData(crowdEngine, AGENTS, DestinationVectors, MovementVectors);


	UID holding = -1;
	crowdEngine->SetPaused(true);
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
		
		DrawPointingPosition(gameEngine, cam, mousePosFont, frameStream);
		
		frameRate = 1 / frameTime;
		frameStream << setprecision(4) << (frameRate);
		gameEngine->SetWindowCaption(frameStream.str());
		frameStream.str("");



		CameraControls(gameEngine, cam);

		/**** Update your scene each frame here ****/
		crowdEngine->Update(frameTime);	//Update the CrowdDynamics simulation

		//Update the influence representation from the crowd engine
#ifdef InfluenceVisualiserEnabled
		UpdateInfluenceFromCrowdData(crowdEngine, InfluenceTiles);
#endif

		//Assign the simulation matrices to the model matrices
		UpdateAgentsFromCrowdData(crowdEngine, AGENTS, DestinationVectors, MovementVectors);


		if (gameEngine->KeyHit(pauseKey))
		{
			crowdEngine->SetPaused(!crowdEngine->GetIsPaused());	//Toggle paused
		}
		if (gameEngine->KeyHeld(Key_T) && crowdEngine->GetIsPaused())
		{
			crowdEngine->PerformOneTimeStep();	//Update the CrowdDynamics simulation

			//Assign the simulation matrices to the model matrices
			UpdateAgentsFromCrowdData(crowdEngine, AGENTS, DestinationVectors, MovementVectors);

		}
		if (gameEngine->KeyHit(Key_N))
		{
			CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
			CVector2 mousePosition = CVector2(tempPos.x, tempPos.z);

			crowdEngine->GetSquareString(mousePosition, tempString);
			cout << tempString;
		}
		if (gameEngine->KeyHeld(Mouse_LButton))
		{	
			if (holding == -1)	//If not holding anything, pick the nearest item up
			{
				UID tempHold;
				if (FindNearestAgent(AGENTS, crowdEngine, tempHold, gameEngine, cam))
				{
					holding = tempHold;
					string agentString;
					if (crowdEngine->GetAgentString(holding, agentString))
					{
						cout << "Picked up: " << endl;
						cout << agentString << endl;
					}
					try		//Find the agent 'holding' and set the holding skin
					{
						AGENTS.at(holding)->SetSkin("tiles3.jpg");

					}
					catch (std::out_of_range err)
					{
						cout << err.what();
					}
				}
			}
			if (holding != -1)
			{
				CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
				crowdEngine->SetAgentPosition(holding, CVector2(tempPos.x, tempPos.z));

				UpdateAgentFromCrowdData(holding, crowdEngine, AGENTS, DestinationVectors, MovementVectors);

			}
		}
		else //Release the held item if there is a held item
		{
			if (holding != -1)
			{
				string agentString;
				if (crowdEngine->GetAgentString(holding, agentString))
				{
					cout << "Dropped: " << endl;
					cout << agentString << endl;
				}
				try
				{
					if (crowdEngine->GetAgentWatched(holding))
					{
						AGENTS.at(holding)->SetSkin("tiles2.jpg");
					}
					else
					{
						AGENTS.at(holding)->SetSkin("tiles1.jpg");
					}
				}
				catch (std::out_of_range err)
				{
					cout << err.what();
				}
				
				holding = -1;
			}
		}
		if (gameEngine->KeyHit(Mouse_RButton))
		{
			UID foundNearest;
			if (FindNearestAgent(AGENTS, crowdEngine, foundNearest, gameEngine, cam))
			{
				bool isWatched = crowdEngine->GetAgentWatched(foundNearest);
				if (crowdEngine->SetAgentWatched(foundNearest, !crowdEngine->GetAgentWatched(foundNearest)))
				{
					cout << "Agent: " << foundNearest << " Is ";
					if (isWatched)	//Was being watched before change, is now not being watched
					{
						try
						{
							AGENTS.at(foundNearest)->SetSkin("tiles1.jpg");
						}
						catch (std::out_of_range err)
						{
							cout << endl << err.what() << endl;
						}
						cout << "Not ";
					}
					else
					{
						try
						{
							AGENTS.at(foundNearest)->SetSkin("tiles2.jpg");
						}
						catch (std::out_of_range err)
						{
							cout << endl << err.what() << endl;
						}
					}
					cout << "Being Watched" << endl;
				}
			}
		}
		if (gameEngine->KeyHit(Key_Space))
		{
			CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
			UID newID = crowdEngine->AddAgent(CVector2(tempPos.x, tempPos.z), true);
			AGENTS.insert(make_pair(newID, agentMesh->CreateModel(tempPos.x, 0.0f, tempPos.z)));
			DestinationVectors.insert(make_pair(newID, vectorMesh->CreateModel(tempPos.x, 10.0f, tempPos.z)));
			MovementVectors.insert(make_pair(newID, vectorMesh->CreateModel(tempPos.x, 11.0f, tempPos.z)));
			MovementVectors.at(newID)->SetSkin("vectortex2.jpg");
		}
	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();
	delete[] FloorTiles;
#ifdef InfluenceVisualiserEnabled
	delete[] InfluenceTiles;
#endif
	delete crowdEngine;
}
