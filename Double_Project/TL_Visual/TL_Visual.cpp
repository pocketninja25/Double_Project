// TL_Visual.cpp: A program using the TL-Engine

#define InfluenceVisualiserEnabled
#define DirectionVisualiserEnabled

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

SWorldBlueprint worldBlueprint;
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
		cam->MoveY(-160.0f * frameTime);
	}
	if (gameEngine->KeyHeld(Key_Numpad9))
	{
		cam->MoveY(160.0f * frameTime);
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

#ifdef DirectionVisualiserEnabled
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
#endif
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

#ifdef DirectionVisualiserEnabled
	
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
#endif
}

void UpdateInfluenceFromCrowdData(GSceneManager* crowdEngine, IModel** influenceTiles)
{
	GInfluenceMap* influenceMap = crowdEngine->GetInfluenceMap();

	float influence;
	CVector2 direction;
	bool isBlocked;
	for (int i = 0; i < worldBlueprint.influenceSubdivisions.x; i++)
	{
		for (int j = 0; j < worldBlueprint.influenceSubdivisions.y; j++)
		{
			influence = influenceMap->GetValue(i, j);

			influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->SetY(2 * influence);
			
			isBlocked = influenceMap->GetIsBlocked(i, j);
			influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->RotateX(90.0f);
			influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->RotateY(90.0f);
			influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->RotateZ(90.0f);

			if (true)//influence != 0)	//Point the sqare in the direction of influence
			{
				direction = influenceMap->GetFlow(i, j);

				influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->LookAt(
					influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->GetX() + direction.x,
					influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->GetY(),
					influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->GetZ() + direction.y
					);
				influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->ScaleX((1.0f / worldBlueprint.influenceSubdivisions.x) * 0.75f);
				
				influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->ScaleZ((1.0f / worldBlueprint.influenceSquaresPerUnit) * 0.75f);
			}
			else
			{
				influenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->ResetOrientation();
			}
		}
	}
}

void DrawPointingPosition(I3DEngine* gameEngine, ICamera* cam, IFont* theFont, stringstream &sstream)
{
	CVector3 mousePos = WorldPosFromPixel(gameEngine, cam);
	sstream << "X: " << std::setprecision(4) << mousePos.x;
	theFont->Draw(sstream.str(), 0, 0, kBlack);
	int height = theFont->MeasureTextHeight(sstream.str());
	sstream.str("");
	sstream << "Z: " << std::setprecision(4) << mousePos.z;
	theFont->Draw(sstream.str(), 0, height, kBlack);
	sstream.str("");
}

enum EQuitState {Quit_Completely = 0, Quit_LoadNewMap = 1};

int EngineMain(string worldFile)
{
	int returnValue = Quit_Completely;

	//--------------------------
	// CrowdDynamics Setup
	//--------------------------
	GSceneManager* crowdEngine = GSceneManager::GetInstance(worldFile);
	worldBlueprint = crowdEngine->GetWorldBlueprint();

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* gameEngine = New3DEngine(kTLX);
	gameEngine->StartWindowed();

	// Add default folder for meshes and other media
	gameEngine->AddMediaFolder(".\\Media");

	IFont* mousePosFont = gameEngine->LoadFont("Font1.bmp");

	IMesh* agentMesh = gameEngine->LoadMesh("Box.x");
	IMesh* agent2Mesh = gameEngine->LoadMesh("Box2.x");
	IMesh* floorTileMesh = gameEngine->LoadMesh("FloorTile.x");
	IMesh* vectorMesh = gameEngine->LoadMesh("Vector.x");
	IMesh* influenceTileMesh = gameEngine->LoadMesh("InfluenceTile.x");
	IMesh* SkyboxMesh = gameEngine->LoadMesh("Skybox.x");

	map<UID, IModel*> Agents;
	map<UID, IModel*> DestinationVectors;
	map<UID, IModel*> MovementVectors;
	IModel** FloorTiles;
	IModel* SkyBox = SkyboxMesh->CreateModel(worldBlueprint.WorldSize.x / 2.0f, -1000.0f, worldBlueprint.WorldSize.y);
	SkyBox->ScaleX(worldBlueprint.WorldSize.x / 300.0f);
	SkyBox->ScaleZ(worldBlueprint.WorldSize.y / 300.0f);

	FloorTiles = new IModel*[worldBlueprint.subdivisions.x * worldBlueprint.subdivisions.y];
	for (int i = 0; i < worldBlueprint.subdivisions.x; i++)
	{
		for (int j = 0; j < worldBlueprint.subdivisions.y; j++)
		{
			FloorTiles[i * worldBlueprint.subdivisions.x + j] = floorTileMesh->CreateModel(i * (worldBlueprint.WorldSize.x / worldBlueprint.subdivisions.x), 0.0f, j * (worldBlueprint.WorldSize.y / worldBlueprint.subdivisions.y));
			FloorTiles[i * worldBlueprint.subdivisions.x + j]->ScaleX(worldBlueprint.WorldSize.x / worldBlueprint.subdivisions.x);
			FloorTiles[i * worldBlueprint.subdivisions.x + j]->ScaleZ(worldBlueprint.WorldSize.y / worldBlueprint.subdivisions.y);
			FloorTiles[i * worldBlueprint.subdivisions.x + j]->Scale(1.0f);
		}
	}

#ifdef InfluenceVisualiserEnabled
	IModel** InfluenceTiles;

	InfluenceTiles = new IModel*[worldBlueprint.influenceSubdivisions.x * worldBlueprint.influenceSubdivisions.y];
	for (int i = 0; i < worldBlueprint.influenceSubdivisions.x; i++)
	{
		for (int j = 0; j < worldBlueprint.influenceSubdivisions.y; j++)
		{
			InfluenceTiles[i * worldBlueprint.influenceSubdivisions.x + j] = influenceTileMesh->CreateModel(static_cast<float>(i) / worldBlueprint.influenceSquaresPerUnit, 0.5f, (static_cast<float>(j) + 1) / worldBlueprint.influenceSquaresPerUnit);
			InfluenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->RotateX(180.0f);
			InfluenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->ScaleX((1.0f / worldBlueprint.influenceSquaresPerUnit) * 0.75f);
			InfluenceTiles[i * worldBlueprint.influenceSubdivisions.x + j]->ScaleZ((1.0f / worldBlueprint.influenceSquaresPerUnit) * 0.75f);
		}
	}
#endif



	vector<UID> AgentIDs = crowdEngine->GetAgentUIDs();

	for (auto agent : AgentIDs)
	{
		//Use the Y Column as the height (0 column), CrowdDynamics uses X and Y as this will use X and Z
		//Where the model spawns is irrelevant as it's matrix is built from the CrowdDynamics matrix
		Agents.insert(make_pair(agent, agentMesh->CreateModel()));
#ifdef DirectionVisualiserEnabled
		DestinationVectors.insert(make_pair(agent, vectorMesh->CreateModel()));
		MovementVectors.insert(make_pair(agent, vectorMesh->CreateModel()));
#endif
	}
	for (auto item : MovementVectors)
	{
		item.second->SetSkin("vectortex2.jpg");
	}

	ICamera* cam = gameEngine->CreateCamera(kFPS, worldBlueprint.WorldSize.x / 2.0f, 220.0f, worldBlueprint.WorldSize.y / 2.0f);
	//ICamera* cam = gameEngine->CreateCamera(kManual, worldBlueprint.WorldSize.x / 2.0f, 220.0f, worldBlueprint.WorldSize.y / 2.0f);
	cam->RotateX(90.0f);
	cam->SetMovementSpeed(100.0f);
	cam->SetRotationSpeed(25.0f);

	float frameRate;
	stringstream frameStream;

	gen::CMatrix3x3 tempAgentMat;
	string tempString;

	//Assign the simulation matrices to the model matrices - first time
	UpdateAgentsFromCrowdData(crowdEngine, Agents, DestinationVectors, MovementVectors);

	UID holding = -1;
	crowdEngine->SetPaused(true);
	// The main game loop, repeat until engine is stopped
	while (gameEngine->IsRunning())
	{
		if (gameEngine->KeyHit(Key_Escape))
		{
			returnValue = Quit_Completely;
			gameEngine->Stop();
		}
		if (gameEngine->KeyHit(Key_Delete))
		{
			returnValue = Quit_LoadNewMap;
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
		UpdateAgentsFromCrowdData(crowdEngine, Agents, DestinationVectors, MovementVectors);


		if (gameEngine->KeyHit(pauseKey))
		{
			crowdEngine->SetPaused(!crowdEngine->GetIsPaused());	//Toggle paused
		}
		if (gameEngine->KeyHeld(Key_T) && crowdEngine->GetIsPaused())
		{
			crowdEngine->PerformOneTimeStep();	//Update the CrowdDynamics simulation

												//Assign the simulation matrices to the model matrices
			UpdateAgentsFromCrowdData(crowdEngine, Agents, DestinationVectors, MovementVectors);

		}
		if (gameEngine->KeyHit(Key_N))
		{
			CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
			CVector2 mousePosition = CVector2(tempPos.x, tempPos.z);
#ifdef _DEBUG
			crowdEngine->GetSquareString(mousePosition, tempString);
			cout << tempString;
#endif
		}
		if (gameEngine->KeyHeld(Mouse_LButton))
		{
			if (holding == -1)	//If not holding anything, pick the nearest item up
			{
				UID tempHold;
				if (FindNearestAgent(Agents, crowdEngine, tempHold, gameEngine, cam))
				{
					holding = tempHold;
					string agentString;
#ifdef _DEBUG
					if (crowdEngine->GetAgentString(holding, agentString))
					{
						cout << "Picked up: " << endl;
						cout << agentString << endl;
					}
#endif
					try		//Find the agent 'holding' and set the holding skin
					{
						Agents.at(holding)->SetSkin("tiles3.jpg");

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

				UpdateAgentFromCrowdData(holding, crowdEngine, Agents, DestinationVectors, MovementVectors);

			}
		}
		else //Release the held item if there is a held item
		{
			if (holding != -1)
			{
#ifdef _DEBUG
				string agentString;
				if (crowdEngine->GetAgentString(holding, agentString))
				{
					cout << "Dropped: " << endl;
					cout << agentString << endl;
				}
#endif
				try
				{
					if (crowdEngine->GetAgentWatched(holding))
					{
						Agents.at(holding)->SetSkin("tiles2.jpg");
					}
					else
					{
						Agents.at(holding)->SetSkin("tiles1.jpg");
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
			if (FindNearestAgent(Agents, crowdEngine, foundNearest, gameEngine, cam))
			{
				bool isWatched = crowdEngine->GetAgentWatched(foundNearest);
				if (crowdEngine->SetAgentWatched(foundNearest, !crowdEngine->GetAgentWatched(foundNearest)))
				{
					cout << "Agent: " << foundNearest << " Is ";
					if (isWatched)	//Was being watched before change, is now not being watched
					{
						try
						{
							Agents.at(foundNearest)->SetSkin("tiles1.jpg");
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
							Agents.at(foundNearest)->SetSkin("tiles2.jpg");
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
			UID newID = crowdEngine->AddAgent("AgentBlueprint1.xml", true, CVector2(tempPos.x, tempPos.z));
			Agents.insert(make_pair(newID, agentMesh->CreateModel(tempPos.x, 0.0f, tempPos.z)));
#ifdef DirectionVisualiserEnabled
			DestinationVectors.insert(make_pair(newID, vectorMesh->CreateModel(tempPos.x, 10.0f, tempPos.z)));
			MovementVectors.insert(make_pair(newID, vectorMesh->CreateModel(tempPos.x, 11.0f, tempPos.z)));
			MovementVectors.at(newID)->SetSkin("vectortex2.jpg");
#endif

		}
	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();
	delete[] FloorTiles;
#ifdef InfluenceVisualiserEnabled
	delete[] InfluenceTiles;
#endif
	delete crowdEngine;

	return returnValue;
}

void main()
{
	string worldBlueprintToLoad;
	do
	{
		cout << "Load Default World?" << endl;
		char input = 'y';
		cin >> input;
		if (input == 'n' || input == 'N')
		{
			cout << "Enter alternate file name" << endl;
			cin >> worldBlueprintToLoad;
		}
		else
		{
			worldBlueprintToLoad = "WorldBlueprint1.xml";
		}
	} while (EngineMain(worldBlueprintToLoad) != Quit_Completely);	//Keep going while the user wants to load another blueprint
	
}
