// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mino.generated.h"

class APlayfield;

UCLASS()
class TETRIS_API AMino : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMino();
	void Init(FVector2D coord, UMaterialInterface* mat, APlayfield* pf, bool isFlux_, int index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool moveTillHit = false;
	UPROPERTY(VisibleAnywhere) bool isFluxing;
	UPROPERTY(VisibleAnywhere) float speed;
	UPROPERTY(VisibleAnywhere) bool isMoving;

	UPROPERTY(VisibleAnywhere) float fluxLerpTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Move(FVector2D nextCoord, float timer); //Movement directions through offset on axis
	void Destroy();
	void SetPosition(FVector2D coords);
	void Flux(int dir, float _speed);

	APlayfield* playfield;
	UStaticMeshComponent* visibleComponent;
	UPROPERTY(VisibleAnywhere) FVector2D curCoords;
	FVector2D prevCoords;
	float width = 10;
	bool isFlux;
	int direction = 1; //1 down, -1 up
	bool finalFlux; //if final flux will spawn a new tetromino when placed
	UPROPERTY(VisibleAnywhere) bool inSegment;
	UPROPERTY(VisibleAnywhere) int tetrominoIndex;
};
