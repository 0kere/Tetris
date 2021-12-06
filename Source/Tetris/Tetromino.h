// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mino.h"
#include "Tetromino.generated.h"

class APlayfield;

UCLASS()
class TETRIS_API ATetromino : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetromino();
	void Init(APlayfield* pf, bool first, int dir) { playfield = pf; firstTetromino = first; moveDir = dir; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float autoMoveTimer;
	UMaterialInterface* thisMat;
	UMaterialInterface* fluxMat;
	UMaterialInterface* ghostMat;

	UPROPERTY(EditAnywhere) FLinearColor minoColor;

	bool firstTetromino = true;
	int curRotateState = 0;

	void Rotate(TArray<FVector2D> state);
	void Drop();
	void OnRelease(); //Should only need the one

	void SpawnGhostBlock();
	void UpdateGhostBlock();
	TArray<AMino*> ghostMinos;

	float autoShiftTimer = 0.f;
	float autoShiftStartTimer = 0.f;
	float autoShiftDelay = 1.f; //wait between each move 
	float autoShiftStartDelay = 0.2f;
	bool canAutoShift = false;
	UPROPERTY(VisibleAnywhere) FVector2D dasDirection = FVector2D(0,1);
	UPROPERTY(VisibleAnywhere) int moveDir = 1;
	int index;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnShape(bool isFluxBlock, FVector2D startPos, bool isNextBlock);
	bool isAnyMinoColliding(); //Check wether any of the minos in the tetromino would be colliding with another
	void Move(FVector2D dir);
	void Flux(int dir, float speed);
	void DestroySelf();

	APlayfield* playfield;
	int amountOfMinos;
	TArray<AMino*> minos;
	TArray<FVector2D> spawningCoords; // structure : center block (always 0,0), following contain translate coordinates (all y posotions in my doc need to be neg flipped since going down required a +1 to the the array position)
	TArray<FVector2D> rotStateOne;
	TArray<FVector2D> rotStateTwo;
	TArray<FVector2D> rotStateThree;

	void OnMoveLeft();
	void OnMoveRight();
	void OnRotate();


	float delayBetweenAutoMove = 1.f;

	void BindInput();

	FVector2D standardStartPos = FVector2D(4,1);
};