// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Playfield.h"
#include "Internationalization/Text.h"

/**
 *
 */
class SInGameWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnScoreChanged, const FText&);

	SLATE_BEGIN_ARGS(SInGameWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMainHUD>, OwningHud)

	//SLATE_EVENT(FOnTextChanged, OnTextChanged)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	void OnScoreChanged(int _score);
	void OnLevelChanged(int _level);
	void OnFluxChargeChanged(int _fluxCharge);

	FText GetScoreText() const { return FText::FromString("Score: " + FString::FromInt(score)); }
	FText GetLevelText() const { return FText::FromString("Level: " + FString::FromInt(level)); }
	FText GetFluxChargeText() const { FString temp = "Flux Charge: " + FString::FromInt(fluxCharge); return FText::FromString(temp + "%"); }

	TWeakObjectPtr<class AMainHUD> OwningHud;

	APlayfield* playfield;

	virtual bool SupportsKeyboardFocus() const override { return true; };
private:
	int score = 0;
	int level = 0;
	int fluxCharge = 0;

};
