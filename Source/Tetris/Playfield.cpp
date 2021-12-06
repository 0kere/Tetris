// Fill out your copyright notice in the Description page of Project Settings.

#include "Playfield.h"
#include "Components/LineBatchComponent.h"
#include "LTetromino.h"
#include "MainHUD.h"


// Sets default values
APlayfield::APlayfield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FNested row;
	row.row.Init(nullptr, rows);
	grid.Init(row, columns);
}

// Called when the game starts or when spawned
void APlayfield::BeginPlay()
{
	Super::BeginPlay();


	BindInput();

	autoMoveDelayDefault = autoMoveDelay;
}

void APlayfield::StartGame()
{
	gameOver = false;
	
	GenerateTetrominoSpawnList();

	SpawnTetromino(true);
}

// Called every frame
void APlayfield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (canMoveTetromino && !gameOver && !isFluxing)
	{
		autoMoveTimer += DeltaTime;
		if (autoMoveTimer > autoMoveDelay)
		{
			autoMoveTimer = 0.f;
			curTetromino->Move(FVector2D(0, direction));
		}
	}
}

bool APlayfield::CanMoveHere(FVector2D coords)
{
	int x = coords.X;
	int y = coords.Y;

	bool horizontalOutBound = false;
	if (direction == 1)
	{
		if (y > playEndPos) { horizontalOutBound = true; }
	}
	else if (direction == -1)
	{
		if (y < playStartPos) { horizontalOutBound = true; }
	}

	if (x < 0 || x >= 10 || horizontalOutBound) //out of bounds
	{
		return false;
	}
	if (grid[x].row[y]->IsValidLowLevel())
	{
		if (horizontalOutBound || (x < 0 || x >= 10) || grid[x].row[y] != nullptr)
		{
			return false;
		}
	}
	return true;
}

void APlayfield::InsertMino(AMino* mino, FVector2D coords)
{
		grid[coords.X].row[coords.Y] = mino;
}

void APlayfield::CheckRow(int row)
{
	bool isFull = true;
	for (size_t i = 0; i < columns; i++) //loop through each element in the row 
	{
		if (grid[i].row[row] == nullptr)
		{
			isFull = false;
			break; //one was false so row is not full yet
		}
	}
	if (isFull) //this needs flipping on flux... probabaly break some of it out into a function 
	{
		//Row clear increment score
		rowsCleared++;
		rowsClearedThisLevel++;
		for (size_t i = 0; i < columns; i++) //loop through each element in the row 
		{
			if (grid[i].row[row] != nullptr)//Check if mino is flux and increment value if so
			{
				if (grid[i].row[row]->isFlux)
				{
					fluxCleared++;

					if (AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
					{
						MainHUD->FluxChargeUpdated((FMath::Clamp((float)fluxCleared/(float)fluxFullCharge, 0.f, 1.f)) * 100.f); //convert to percent
					}
				}
				grid[i].row[row]->Destroy();
				grid[i].row[row] = nullptr;
			}
		}

		RemoveRows(row);
	}
}

void APlayfield::RemoveRows(int row)
{
	if (direction == -1)
	{
		for (int i = row; i < rows; ++i) //goes from top of grid down 
		{
			if (i <= 3 || i >= 20) { continue; }

			for (size_t j = 0; j < columns; j++) //loop through each mino moving it down and updating its grid position instead i should probabaly just tell the mino its needs to move down and then have it check it self when to stop and insert it self
			{
				FVector2D originalCoord = FVector2D(j, i);
				FVector2D newCoord = originalCoord;
				newCoord.Y += direction;

				if (grid[j].row[i] != nullptr)
				{
					grid[j].row[i]->SetPosition(newCoord);
					InsertMino(grid[j].row[i], newCoord);
					RemoveMino(originalCoord);
				}
			}
		}
	}
	else if (direction == 1)
	{
		for (int i = row; i >= 0; --i) //goes from bottom of grid up 
		{
			if (i <= 3 || i >= 20) { continue; }

			for (size_t j = 0; j < columns; j++) //loop through each mino moving it down and updating its grid position instead i should probabaly just tell the mino its needs to move down and then have it check it self when to stop and insert it self
			{
				FVector2D originalCoord = FVector2D(j, i);
				FVector2D newCoord = originalCoord;
				newCoord.Y += direction;

				if (grid[j].row[i] != nullptr)
				{
					grid[j].row[i]->SetPosition(newCoord);
					InsertMino(grid[j].row[i], newCoord);
					RemoveMino(originalCoord);
				}
			}
		}
	}
}

void APlayfield::IncrementScore()
{


	switch (rowsCleared)
	{
	case 1: score += 40 * (curLevel + 1);
		break;
	case 2: score += 100 * (curLevel + 1);
		break;
	case 3: score += 300 * (curLevel + 1);
		break;
	case 4: score += 1200 * (curLevel + 1);
		fluxCleared = fluxFullCharge; //tetris bonus charge
		break;
	default:
		break;
	}
	if (rowsClearedThisLevel >= rowsToIncreaseLevel)
	{
		fluxCleared = fluxFullCharge; //next level bonus charge
		curLevel++;
		rowsClearedThisLevel = rowsClearedThisLevel-rowsToIncreaseLevel;
		autoMoveDelay *= autoMoveDelayDecreasePLevel;
		autoMoveDelay = FMath::Clamp(autoMoveDelay, autoMoveMin, autoMoveDelay);

		if (AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
		{
			MainHUD->LevelUpdated(curLevel);
		}
	}

	if (AMainHUD* MainHUD = Cast<AMainHUD>( UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
	{
		MainHUD->ScoreUpdated(score);
		MainHUD->FluxChargeUpdated((int)(((float)fluxCleared / (float)fluxFullCharge) * 100.f)); //convert to percent
	}
}

void APlayfield::RemoveMino(FVector2D coords)
{
	grid[coords.X].row[coords.Y] = nullptr;
}

void APlayfield::TetrominoPlaced()
{
	if (rowsCleared == 4) //give extra rows for a tetris so level increases quicker
	{
		rowsClearedThisLevel += rowsCleared;
	}
	canMoveTetromino = false;
	IncrementScore();
	rowsCleared = 0; //reset rows cleared after score has been incremented
	SpawnTetromino(false);
}

void APlayfield::GenerateTetrominoSpawnList() //generates a ranom ofer of all tetrominos to spawn in
{

	tetrominosToSpawn = tetrominos;
	for (int32 i = tetrominosToSpawn.Num() - 1; i > 0; i--) {
		int32 j = FMath::FloorToInt(FMath::Rand() * (i + 1)) % tetrominosToSpawn.Num();

		TSubclassOf<ATetromino> temp = tetrominosToSpawn[i];
		tetrominosToSpawn[i] = tetrominosToSpawn[j];
		tetrominosToSpawn[j] = temp;
	}
}

void APlayfield::SpawnTetromino(bool first)
{
	if (isFluxing) { return; }//dont spawn a tetromino while fluxing is occuring
	ATetromino* newTetromino = GetWorld()->SpawnActor<ATetromino>(tetrominosToSpawn[0]);
	tetrominosToSpawn.RemoveAt(0);
	if (tetrominosToSpawn.Num() <= 0) 
	{
		GenerateTetrominoSpawnList();
	}
	HandleNextTetromino();
	bool fluxSpawn = false;
	float roll = FMath::FRandRange(0, 100);
	if (roll <= fluxBlockRollChance)
	{
		fluxSpawn = true;
	}

	FVector2D start = downStartPos;
	if (direction == -1)
	{
		start = upStartPos;
	}

	newTetromino->Init(this, first, direction);
	newTetromino->SpawnShape(fluxSpawn, start, false);
	curTetromino = newTetromino;
	canMoveTetromino = true;
}

void APlayfield::GameOver()
{
	if (AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
	{
		MainHUD->ShowGameOverMenu();
	}

	gameOver = true;
}

void APlayfield::RestartGame()
{
	if (!gameOver) { return; }
	//needs to delete everything from the grid, set score to 0, set delay back to default, level back to 0 and then call StartGame
	for (size_t i = 0; i < columns; i++)
	{
		for (size_t j = 0; j < rows; j++)
		{
			if (grid[i].row[j] != nullptr)
			{
				grid[i].row[j]->Destroy();
				grid[i].row[j] = nullptr;
			}

		}
	}

	score = 0;
	autoMoveDelay = autoMoveDelayDefault;
	curLevel = 0;
	fluxCleared = 0;
	direction = 1;
	StartGame();
	gameOver = false;
}

void APlayfield::OnFlux()
{
	if (fluxCleared >= fluxFullCharge)
	{
		isFluxing = true;
		fluxCleared = 0;
		if (AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
		{
			MainHUD->FluxChargeUpdated((int)(((float)fluxCleared / (float)fluxFullCharge) * 100.f)); //convert to percent
		}
		direction = (direction == 1 ? -1 : 1);
		curTetromino->Flux(direction,0.f);

		CreateSegments();
		for (size_t i = 0; i < segments.Num(); i++)
		{
			segments[i]->OnFlux(direction, fluxMoveMaxSpeed);
		}
	}
}

void APlayfield::MinoCompletedFlux()
{
	--movingFluxMinos;
	if (movingFluxMinos == 0)
	{
		isFluxing = false;
		SpawnTetromino(false);
		for (size_t i = 0; i < segments.Num(); i++)
		{
			segments[i]->Destroy();
		}
		segments.Empty();
	}
}

void APlayfield::CreateSegments()
{
	if (direction == 1)
	{
		for (int i = columns-1; i >= 0; --i)
		{
			for (int j = rows-1; j >= 0; --j)
			{
				if (grid[i].row[j] != nullptr)
				{
					if (!grid[i].row[j]->inSegment)
					{
						AstickSegment* temp = GetWorld()->SpawnActor<AstickSegment>();
						temp->Init(this);
						AMino* tempMino = grid[i].row[j];
						grid[i].row[j] = nullptr;
						temp->AddMino(tempMino);
						PopulateSegment(temp, tempMino);
						segments.Add(temp);
						movingFluxMinos++;
					}
				}
			}
		}
	}
	if (direction == -1)
	{
		for (size_t i = 0; i < columns; i++)
		{
			for (size_t j = 0; j < rows; j++)
			{
				if (grid[i].row[j] != nullptr)
				{
					if (!grid[i].row[j]->inSegment)
					{
						AstickSegment* temp = GetWorld()->SpawnActor<AstickSegment>();
						temp->Init(this);
						AMino* tempMino = grid[i].row[j];
						grid[i].row[j] = nullptr;
						temp->AddMino(tempMino);
						PopulateSegment(temp, tempMino);
						segments.Add(temp);
						movingFluxMinos++;
					}
				}
			}
		}
	}
}

void APlayfield::PopulateSegment(AstickSegment* segment, AMino* centralMino)
{
	//checks in 4 directions around the central mino for other members of the same shape
	FVector2D checkCoord = centralMino->curCoords;
	checkCoord.Y += 1; //check grid space above centralMino
	if (checkCoord.X >= 0 && checkCoord.X < columns && checkCoord.Y >= 0 && checkCoord.Y < rows)
	{
		SurroundingMinoCheck(checkCoord, segment, centralMino);
	}
	checkCoord.Y -= 2; //check below center
	if (checkCoord.X >= 0 && checkCoord.X < columns && checkCoord.Y >= 0 && checkCoord.Y < rows)
	{
		SurroundingMinoCheck(checkCoord, segment, centralMino);
	}
	checkCoord.Y += 1;
	checkCoord.X -= 1; //check left of center
	if (checkCoord.X >= 0 && checkCoord.X < columns && checkCoord.Y >= 0 && checkCoord.Y < rows) //check bounds
	{
		SurroundingMinoCheck(checkCoord, segment, centralMino);
	}
	checkCoord.X += 2; //check right of center
	if (checkCoord.X >= 0 && checkCoord.X < columns && checkCoord.Y >= 0 && checkCoord.Y < rows)
	{
		SurroundingMinoCheck(checkCoord, segment, centralMino);
	}
}

void APlayfield::SurroundingMinoCheck(FVector2D coord, AstickSegment* segment, AMino* centralMino)
{
	if (grid[coord.X].row[coord.Y] != nullptr)
	{
		AMino* tempMino = grid[coord.X].row[coord.Y];
		grid[coord.X].row[coord.Y] = nullptr;
		if (!tempMino->inSegment && tempMino->tetrominoIndex == centralMino->tetrominoIndex)
		{
			movingFluxMinos++;
			segment->AddMino(tempMino);
			PopulateSegment(segment, tempMino); //each mino added will check for any of the same type mino connecting to it that isnt already apart of the segment
		}
		else
		{
			grid[coord.X].row[coord.Y] = tempMino;
		}
	}
}


void APlayfield::HandleNextTetromino()
{
	if (next != nullptr)
	{
		next->DestroySelf();
	}
	ATetromino* newTetromino = GetWorld()->SpawnActor<ATetromino>(tetrominosToSpawn[0]);
	//-13,-5
	newTetromino->Init(this, false, 0);
	newTetromino->SpawnShape(false, FVector2D(13, 5), true);
	next = newTetromino;
}

void APlayfield::BindInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("RestartGame", IE_Pressed, this, &APlayfield::RestartGame);
		InputComponent->BindAction("Flux", IE_Pressed, this, &APlayfield::OnFlux);

		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

