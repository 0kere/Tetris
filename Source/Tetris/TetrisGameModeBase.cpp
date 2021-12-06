// Copyright Epic Games, Inc. All Rights Reserved.


#include "TetrisGameModeBase.h"

#include "MainHUD.h"
#include "MenuController.h"

ATetrisGameModeBase::ATetrisGameModeBase()
{
	PlayerControllerClass = AMenuController::StaticClass();

	HUDClass = AMainHUD::StaticClass();
}