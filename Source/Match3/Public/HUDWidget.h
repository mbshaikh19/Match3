// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MATCH3_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Score_txt;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FinalScore_txt;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Time_txt;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* GameOverPanel;

	UPROPERTY(meta = (BindWidget))
	UButton* Restart_btn;

public:
	UPROPERTY()
	int32 score = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 timeInSeconds = 0;

	FTimerHandle TimerHandle;

	FString TimeStr;

public:
	
	UFUNCTION()
	void SetScore(int32 newScore);

	virtual void NativeConstruct() override;

	void GameTimer();

	UFUNCTION(BlueprintCallable)
	void RestartGame();
};
