// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Match3_PC.generated.h"

/**
 * 
 */
UCLASS()
class MATCH3_API AMatch3_PC : public APlayerController
{
	GENERATED_BODY()

public:
	AMatch3_PC(const FObjectInitializer& ObjectInitializer);
	
};
