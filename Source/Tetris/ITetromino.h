// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "ITetromino.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API AITetromino : public ATetromino
{
	GENERATED_BODY()
public:
	AITetromino();
	virtual void BeginPlay() override;
};
