// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetromino.h"
#include "Playfield.h"

// Sets default values
ATetromino::ATetromino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
	

	canAutoShift = false;
	autoShiftStartTimer = 0.f;
	autoShiftTimer = 0.f;
	//SpawnShape();
}

// Called every frame
void ATetromino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (canAutoShift)
	{
		autoShiftStartTimer += DeltaTime;
		if (autoShiftStartTimer > autoShiftStartDelay)
		{
			autoShiftTimer += DeltaTime;
			if (autoShiftDelay < autoShiftTimer)
			{
				Move(dasDirection);
			}
		}
	}
	else
	{
		autoShiftDelay = autoShiftTimer;//avoids extra starting delay
	}
}

void ATetromino::SpawnShape(bool isFluxBlock, FVector2D startPos, bool isNextBlock)
{
	int fluxIndex = 0;
	if (isFluxBlock)
	{
		fluxIndex = FMath::RandRange(0, amountOfMinos - 1);
	}

	FVector2D placeToSpawn = startPos;
	bool isOverlap = false; //game should end when there is an overlap on block spawn
	if (amountOfMinos == 0) { return; }
	for (size_t i = 0; i < amountOfMinos; i++)
	{
		placeToSpawn = startPos + spawningCoords[i];
		if (playfield) //this is breaking the game for some reason and i cant keep working on it right now
		{
			if (!playfield->CanMoveHere(placeToSpawn) && !isNextBlock)
			{
				isOverlap = true;
				break;
			}
		}
		AMino* newMino = (AMino*)GWorld->SpawnActor(AMino::StaticClass());
		if (isFluxBlock && i == fluxIndex && fluxMat)
		{
			newMino->Init(placeToSpawn, fluxMat, playfield, isFluxBlock, index);
		}
		else
		{
			newMino->Init(placeToSpawn, thisMat, playfield, false, index);
		}
		//playfield->InsertMino(newMino, newMino->curCoords);
		minos.Push(newMino);
	}
	if (isOverlap)
	{
		playfield->GameOver();
		AActor::Destroy();
	}
	if (!isNextBlock)
	{
		BindInput();

		SpawnGhostBlock();
	}
}

void ATetromino::SpawnGhostBlock()
{
	FVector2D placeToSpawn = standardStartPos;
	if (amountOfMinos == 0) { return; }
	for (size_t i = 0; i < amountOfMinos; i++)
	{
		placeToSpawn = standardStartPos + spawningCoords[i];
		AMino* newMino = (AMino*)GWorld->SpawnActor(AMino::StaticClass());
		newMino->Init(placeToSpawn, ghostMat, playfield, false, index);

		ghostMinos.Push(newMino);
	}

}

void ATetromino::UpdateGhostBlock()
{
	TArray<FVector2D> validCoords;
	for (size_t i = 0; i < amountOfMinos; i++) //reset ghost position
	{
		validCoords.Push(minos[i]->curCoords);
		ghostMinos[i]->SetPosition(validCoords[i]);
	}
	bool canMove = true;
	for (size_t i = 0; i < playfield->rows; i++) //repeatedly move untill something is hit
	{
		TArray<FVector2D> newCoords;
		for (size_t minoIndex = 0; minoIndex < amountOfMinos; minoIndex++)
		{
			FVector2D tempCoord = (validCoords[minoIndex] + FVector2D(0, moveDir));
			tempCoord.X = FMath::Clamp(tempCoord.X, 0.f, (float)playfield->columns - 1);
			tempCoord.Y = FMath::Clamp(tempCoord.Y, 0.f, (float)playfield->rows - 1);
			newCoords.Push(tempCoord);
			if (!playfield->CanMoveHere(newCoords[minoIndex]))
			{
				canMove = false;
				break;
			}
		}
		if (!canMove)
		{
			break;
		}
		else
		{
			validCoords = newCoords;
		}
	}
	for (size_t i = 0; i < amountOfMinos; i++)
	{
		ghostMinos[i]->SetPosition(validCoords[i]); //Set final position
	}
}

bool ATetromino::isAnyMinoColliding()
{
	return false;
}

void ATetromino::Move(FVector2D dir)
{
	TArray<FVector2D> newCoords;
	bool canMove = true;
	for (size_t i = 0; i < amountOfMinos; i++)
	{
		newCoords.Push((minos[i]->curCoords + dir));
		if (!playfield->CanMoveHere(newCoords[i]))
		{
			canMove = false;
			break;
		}
	}
	if (canMove)
	{
		//needs to update the grid with the new mino positions
		for (size_t i = 0; i < amountOfMinos; i++)
		{
			playfield->RemoveMino(minos[i]->curCoords);
			minos[i]->SetPosition(newCoords[i]);
		}
		if (ghostMinos[0] != nullptr)
			UpdateGhostBlock();
	}
	else
	{
		//object should now be placed in the area if it hit something while trying to move down
		if (dir.X == 0 && dir.Y != 0) //moving downwards
		{
			TArray<int> rowsPlacedIn;
			bool isInPlay = false;
			for (size_t i = 0; i < amountOfMinos; i++)
			{
				playfield->InsertMino(minos[i], minos[i]->curCoords);
				if (!rowsPlacedIn.Contains(minos[i]->curCoords.Y))
				{
					rowsPlacedIn.Push(minos[i]->curCoords.Y);
					if (minos[i]->curCoords.Y < 3) { isInPlay = true; } //block placed outside of the playzone
				}
			}
			rowsPlacedIn.Sort();
			if (rowsPlacedIn.Num() > 0)
			{
				for (int i : rowsPlacedIn)
				{
					playfield->CheckRow(i);
				}
			}
			playfield->TetrominoPlaced();
			for (size_t i = 0; i < amountOfMinos; i++)
			{
				ghostMinos[i]->Destroy();
				ghostMinos[i] = nullptr;
			}
			Destroy();
		}
	}

}

void ATetromino::Rotate(TArray<FVector2D> state)
{
	TArray<FVector2D> newCoords;
	bool canMove = true;
	for (size_t i = 0; i < amountOfMinos; i++)
	{
		newCoords.Push(minos[0]->curCoords + state[i]); //uses the original mino as the center piece
		if (!playfield->CanMoveHere(newCoords[i]))
		{
			curRotateState--;//undo the increment made if rotate isnt valid
			canMove = false;
			break;
		}
	}
	if (canMove) // dont rotate if cant move
	{
		for (size_t i = 0; i < amountOfMinos; i++)
		{
			minos[i]->SetPosition(newCoords[i]);
		}
		UpdateGhostBlock();
	}

}

void ATetromino::Drop()
{
	canAutoShift = true;
	dasDirection = FVector2D(0, moveDir);
}

void ATetromino::OnMoveLeft()
{
	canAutoShift = true;
	dasDirection = FVector2D(-1, 0);
	Move(FVector2D(-1, 0));
}

void ATetromino::OnMoveRight()
{
	canAutoShift = true;
	dasDirection = FVector2D(1, 0);
	Move(FVector2D(1, 0));
}

void ATetromino::OnRotate()
{
	//Update Rotate State
	curRotateState++; //cur state will always be between 0 and 3 due to the switch set up

	switch (curRotateState % 4)//keep number between 0-3
	{
	case 0: Rotate(spawningCoords);
		break;
	case 1: Rotate(rotStateOne);
		break;
	case 2: Rotate(rotStateTwo);
		break;
	case 3: Rotate(rotStateThree);
		break;
	default:
		break;
	}
}

void ATetromino::Flux(int dir, float speed)
{
	moveDir = dir;
	canAutoShift = false;
	for (size_t i = 0; i < amountOfMinos; i++) //places the current minos straight into the grid 
	{//should probs not tell minos to flux here. Instead have this run before the playfield goes through and fluxes all minos. Should avoid any issues when flux happens just before a peice is placed
		if (minos[i]->curCoords.Y <= 3 || minos[i]->curCoords.Y >= 20)
		{//remove minos that are off screen when the flux occurs
			minos[i]->Destroy();
			ghostMinos[i]->Destroy();
			continue;
		}
		playfield->InsertMino(minos[i], minos[i]->curCoords);
		ghostMinos[i]->Destroy();
		AActor::Destroy();
	}
}

void ATetromino::OnRelease()
{
	canAutoShift = false;
	autoShiftStartTimer = 0.f;
	autoShiftTimer = 0.f;
}

void ATetromino::DestroySelf()
{
	for (size_t i = 0; i < amountOfMinos; i++)
	{
		minos[i]->Destroy();
	}
	AActor::Destroy();
}

void ATetromino::BindInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("MoveRight", IE_Pressed, this, &ATetromino::OnMoveRight);
		InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ATetromino::OnMoveLeft);
		InputComponent->BindAction("MoveDown", IE_Pressed, this, &ATetromino::Drop);

		InputComponent->BindAction("MoveRight", IE_Released, this, &ATetromino::OnRelease);
		InputComponent->BindAction("MoveLeft", IE_Released, this, &ATetromino::OnRelease);
		InputComponent->BindAction("MoveDown", IE_Released, this, &ATetromino::OnRelease);

		InputComponent->BindAction("Rotate", IE_Pressed, this, &ATetromino::OnRotate);
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

