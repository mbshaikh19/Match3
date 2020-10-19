// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3_GM.h"
#include "Match3_PC.h"

AMatch3_GM::AMatch3_GM(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = AMatch3_PC::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APawn::StaticClass();
}

void AMatch3_GM::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("ggg Game mode"));
}