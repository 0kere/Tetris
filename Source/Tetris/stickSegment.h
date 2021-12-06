// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "mino.h"
#include "stickSegment.generated.h"

class APlayfield;

UCLASS()
class TETRIS_API AstickSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AstickSegment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move();
	void PlaceMinos();
	bool CheckMinos();
	UPROPERTY(VisibleAnywhere) bool isFluxing;
	UPROPERTY(VisibleAnywhere) float fluxTimer;
	UPROPERTY(VisibleAnywhere) float fluxSpeed;
	float direction;
	UPROPERTY(VisibleAnywhere) TArray<AMino*> minos;
	TArray<FVector2D> nextCoords;
	APlayfield* pf;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init(APlayfield* playfield) { pf = playfield; }
	void OnFlux(int dir, float speed);
	void AddMino(AMino* mino);

};
