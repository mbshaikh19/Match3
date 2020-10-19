// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3_PC.h"

AMatch3_PC::AMatch3_PC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;

	// We need click/touch events to interact with our tiles.
	bEnableTouchEvents = bEnableClickEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;
}