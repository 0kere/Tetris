// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Playfield.h"

/**
 *
 */
class SPauseWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPauseWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMainHUD>, OwningHud)

		SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	FReply OnResumeClicked();
	FReply OnQuitClicked();

	TWeakObjectPtr<class AMainHUD> OwningHud;

	APlayfield* playfield;

	virtual bool SupportsKeyboardFocus() const override { return true; };
};
