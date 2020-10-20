// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"	
#include "PaperSpriteComponent.h"
#include "Grid.h"
#include "Components/PrimitiveComponent.h"
#include "Match3_PC.h"
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
	OnClicked.AddDynamic(this, &ATile::TileClicked);
	Cast<UPaperSpriteComponent>(RootComponent)->OnBeginCursorOver.AddDynamic(this, &ATile::TileHover);
}

void ATile::SetTileSprite(UPaperSprite* tileSprite)
{
	GetRenderComponent()->SetSprite(tileSprite);
}

void ATile::TileClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	UE_LOG(LogTemp,Warning,TEXT("gggg tile clicked"));
	grid->OnTileClicked(this);

}

void ATile::TileHover(class UPrimitiveComponent* OtherComp)//ETouchIndex::Type index, AActor* actor)
{
	AMatch3_PC* PC = Cast<AMatch3_PC>(UGameplayStatics::GetPlayerController(this, 0));
	if(PC->IsInputKeyDown(EKeys::LeftMouseButton))
	if (!grid->stopTileSelection)
	{
		UE_LOG(LogTemp, Warning, TEXT("gggg tile hover"));
		grid->OnTileClicked(this);
	}
}

void ATile::resetSelection(ETouchIndex::Type index, AActor* actor)
{
	UE_LOG(LogTemp, Warning, TEXT("gggg tile reset selection"));
	grid->stopTileSelection = false;
}