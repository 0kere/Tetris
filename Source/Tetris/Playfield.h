// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mino.h"
#include "Tetromino.h"
#include "stickSegment.h"
#include "Playfield.generated.h"

USTRUCT()
struct FNested
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)TArray<AMino*> row;
};

UCLASS()
class TETRIS_API APlayfield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayfield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GenerateTetrominoSpawnList();

	ATetromino* curTetromino;
	UPROPERTY(EditAnywhere) float autoMoveDelay;
	float autoMoveDelayDefault;
	UPROPERTY(EditAnywhere) float autoMoveDelayDecreasePLevel;
	UPROPERTY(EditAnywhere) float autoMoveMin;
	bool canMoveTetromino;
	float autoMoveTimer;

	int curLevel = 0;
	int rowsToIncreaseLevel = 10;
	int rowsClearedThisLevel;
	UPROPERTY(EditAnywhere) float fluxBlockRollChance; // when tetormino is spawned rand num 0-100 is generated, if this number is lower than that number - spawn a flux block
	UPROPERTY(EditAnywhere) float fluxFullCharge;
	UPROPERTY(EditAnywhere) float fluxMoveMaxSpeed;
	bool isFluxing; //Bool for when flux transition is occuring
	int fluxCleared = 0;
	UPROPERTY(VisibleAnywhere) int direction = 1;
	UPROPERTY(VisibleAnywhere) int movingFluxMinos = 0;

	void CreateSegments();
	void PopulateSegment(AstickSegment* segment, AMino* centralMino);
	void SurroundingMinoCheck(FVector2D coord, AstickSegment* segment, AMino* centralMino);
	TArray<AstickSegment*> segments;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void BindInput();

	bool CanMoveHere(FVector2D coords);
	void InsertMino(AMino* tetromino, FVector2D coords);
	void CheckRow(int row);
	void RemoveRows(int row);
	void RemoveMino(FVector2D coords);
	void TetrominoPlaced();
	void SpawnTetromino(bool first);
	void StartGame();
	void GameOver();
	void RestartGame();
	void OnFlux();
	void MinoCompletedFlux();

	static const int rows = 24; //8 for a vanish zone
	static const int columns = 10;
	UPROPERTY(EditAnywhere) TArray<FNested> grid;
	const int playStartPos = 4;
	const int playEndPos = 19;

	int score = 0;
	int rowsCleared;
	void IncrementScore();
	bool gameOver = true;

	float minoWidth = 10.f;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<ATetromino>> tetrominos;
	TArray<TSubclassOf<ATetromino>> tetrominosToSpawn;

	ATetromino* next;
	void HandleNextTetromino();
	
	FVector2D downStartPos = FVector2D(4, 1);
	FVector2D  upStartPos = FVector2D(4, 21);

	UPROPERTY(EditAnywhere)
		UMaterialInterface* borderMat;
};
