// Fill out your copyright notice in the Description page of Project Settings.
//code adapted from (Ward, 2020)
#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Playfield.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMainHUD>, OwningHud)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
	
	FReply OnPlayClicked();
	FReply OnQuitClicked();

	TWeakObjectPtr<class AMainHUD> OwningHud;

	APlayfield* playfield;

	virtual bool SupportsKeyboardFocus() const override { return true; };
};
//end of adapted code
