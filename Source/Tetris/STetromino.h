// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "STetromino.generated.h"

/**
 *
 */
UCLASS()
class TETRIS_API ASTetromino : public ATetromino
{
	GENERATED_BODY()
public:
	ASTetromino();
	virtual void BeginPlay() override;
};
