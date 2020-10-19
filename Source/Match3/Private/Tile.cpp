// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"	
#include "PaperSpriteComponent.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"

ATile::ATile()
{
	AutoReceiveInput = EAutoReceiveInput::Player0;
	
	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}
	RootComponent->bAutoActivate = true;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	grid = Cast<AGrid>(GetOwner());
	//OnInputTouchBegin.AddUniqueDynamic(this, &ATile::TileClicked);
	OnInputTouchEnter.AddUniqueDynamic(this, &ATile::TileHover);
	OnInputTouchLeave.AddUniqueDynamic(this, &ATile::TileClicked);
	OnInputTouchEnd.AddUniqueDynamic(this,&ATile::resetSelection);
	//OnInputTouchEnd.AddUniqueDynamic(this, &ATile::TileClicked);
}

void ATile::SetTileSprite(UPaperSprite* tileSprite)
{
	GetRenderComponent()->SetSprite(tileSprite);
}

void ATile::TileClicked(ETouchIndex::Type index, AActor* actor)
{
	UE_LOG(LogTemp,Warning,TEXT("gggg tile clicked"));

}

void ATile::TileHover(ETouchIndex::Type index, AActor* actor)
{
	UE_LOG(LogTemp, Warning, TEXT("gggg tile hover"));

	if (!grid->stopTileSelection)
	{
		grid->OnTileClicked(this);
	}
}


void ATile::resetSelection(ETouchIndex::Type index, AActor* actor)
{
	UE_LOG(LogTemp, Warning, TEXT("gggg tile reset selection"));
	grid->stopTileSelection = false;
}