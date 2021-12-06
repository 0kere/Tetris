// Fill out your copyright notice in the Description page of Project Settings.


#include "stickSegment.h"
#include "Playfield.h"

// Sets default values
AstickSegment::AstickSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AstickSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

void AstickSegment::Move()
{
	for (size_t i = 0; i < minos.Num(); i++)
	{
		minos[i]->Move(nextCoords[i], fluxTimer);
	}
	bool canMove = true;
	if (fluxTimer >= 1.f)
	{
		for (size_t i = 0; i < minos.Num(); i++)
		{
			minos[i]->curCoords = nextCoords[i];
			nextCoords[i].Y += direction;
		}
		canMove = CheckMinos();
		if (canMove)
		{
			fluxTimer = 0.f;
		}
	}

	if (!canMove || !CheckMinos())
	{
		isFluxing = false;
		PlaceMinos();
	}
}

void AstickSegment::PlaceMinos()
{
	for (size_t i = 0; i < minos.Num(); i++)
	{
		minos[i]->SetPosition(minos[i]->curCoords);
		pf->InsertMino(minos[i], minos[i]->curCoords);
		pf->CheckRow(minos[i]->curCoords.Y);
		minos[i]->inSegment = false;
		pf->MinoCompletedFlux();
	}
}

bool AstickSegment::CheckMinos()
{
	for (size_t i = 0; i < minos.Num(); i++)
	{
		if (!pf->CanMoveHere(nextCoords[i]))
		{
			return false;
		}
	}
	return true;
}

// Called every frame
void AstickSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFluxing)
	{
		fluxTimer += DeltaTime * fluxSpeed;
		Move();
	}
}

void AstickSegment::OnFlux(int dir, float speed)
{
	isFluxing = true;
	fluxSpeed = speed;
	direction = dir;
	fluxTimer = 0.f;
	for (size_t i = 0; i < minos.Num(); i++)
	{
		nextCoords.Add(minos[i]->curCoords);
		nextCoords[i].Y += direction;
	}
}

void AstickSegment::AddMino(AMino* mino)
{
	minos.Add(mino);
	mino->inSegment = true;
}

