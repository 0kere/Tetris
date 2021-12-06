// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "JTetromino.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API AJTetromino : public ATetromino
{
	GENERATED_BODY()
public:
	AJTetromino();
	virtual void BeginPlay() override;
};
