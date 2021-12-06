// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Playfield.h"

/**
 *
 */
class SGameOverWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGameOverWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMainHUD>, OwningHud)

		SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	FReply OnPlayClicked();
	FReply OnQuitClicked();

	TWeakObjectPtr<class AMainHUD> OwningHud;

	APlayfield* playfield;

	virtual bool SupportsKeyboardFocus() const override { return true; };
};
