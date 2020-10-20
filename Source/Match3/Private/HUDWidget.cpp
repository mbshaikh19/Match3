// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Kismet/GameplayStatics.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHUDWidget::GameTimer, 1, true, 0);
	GameOverPanel->SetVisibility(ESlateVisibility::Collapsed);
	Restart_btn->OnClicked.AddDynamic(this, &UHUDWidget::RestartGame);
}

void UHUDWidget::SetScore(int32 newScore)
{
	score += newScore;
	Score_txt->SetText(FText::FromString(FString::FromInt(score)));
}

void UHUDWidget::GameTimer()
{
	if (timeInSeconds > 0)
	{
		timeInSeconds = timeInSeconds - 1;
		TimeStr = TEXT("");
		TimeStr.AppendInt(timeInSeconds / 60);
		TimeStr.Append(TEXT(":"));
		TimeStr.AppendInt(timeInSeconds % 60);
		Time_txt->SetText(FText::FromString(TimeStr));
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GameOverPanel->SetVisibility(ESlateVisibility::Visible);
		FinalScore_txt->SetText(FText::FromString(FString::FromInt(score)));
	}
}

void UHUDWidget::RestartGame()
{
	UE_LOG(LogTemp,Warning,TEXT("gggg restart game function called."));
	UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
}