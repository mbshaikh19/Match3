// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Match3_GM.generated.h"

/**
 * 
 */
UCLASS()
class MATCH3_API AMatch3_GM : public AGameModeBase
{
	GENERATED_BODY()

		AMatch3_GM(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
};
