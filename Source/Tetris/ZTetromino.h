// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "ZTetromino.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API AZTetromino : public ATetromino
{
	GENERATED_BODY()
public:
	AZTetromino();
	virtual void BeginPlay() override;
};
