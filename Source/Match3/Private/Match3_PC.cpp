// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3_PC.h"

#include "Grid.h"

AMatch3_PC::AMatch3_PC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	EnableInput(this);
	// We need click/touch events to interact with our tiles.
	bEnableTouchEvents = bEnableClickEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;
	if (InputComponent)
	{
		//this->SetupInputComponent();

		InputComponent = NewObject<UInputComponent>(this, TEXT("PC_InputComponent0"));
		InputComponent->RegisterComponent();
	}
			
}

void AMatch3_PC::BeginPlay()
{
	if (InputComponent)
	{
		//InputComponent->BindAction("Mouse_LB_release", IE_Released, this, &AMatch3_PC::MouseLBReleased);
	}
}

void AMatch3_PC::MouseLBReleased()
{
	UE_LOG(LogTemp,Warning,TEXT("gggg mouse released"));
	if (grid)
	{
		grid->stopTileSelection = false;
	}
}

void AMatch3_PC::SetGridRef(AGrid* gridRef)
{
	grid = gridRef;
}

void AMatch3_PC::Tick(float deltaTime)
{
	if (WasInputKeyJustReleased(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("gggg mouse released"));
		if (grid)
		{
			grid->stopTileSelection = false;
			grid->MouseBtnReleased();
		}
	}
}