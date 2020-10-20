// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Match3_PC.generated.h"

/**
 * 
 */
class UHUDWidget;
class AGrid;
UCLASS()
class MATCH3_API AMatch3_PC : public APlayerController
{
	GENERATED_BODY()

public:
	AGrid* grid;
	class UInputComponent* InputComponent=nullptr;

	UPROPERTY()
	UHUDWidget* HUD_wid;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HUD widget")
	TSubclassOf<class UHUDWidget> HUD_wid_class;

	int32 scoreMultiple = 2;
public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	AMatch3_PC(const FObjectInitializer& ObjectInitializer);
	void MouseLBReleased();
	void SetGridRef(AGrid* gridRef);

	UFUNCTION()
	void SetScore(int32 tileCount);
};
