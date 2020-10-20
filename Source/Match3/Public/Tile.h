// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "PaperSprite.h"
#include "Tile.generated.h"

/**
 * 
 */
UENUM(BlueprintType)

enum TileType
{
	T_Yellow,
	T_Blue,
	T_Green
};

UCLASS()
class MATCH3_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()
		
public:
	ATile();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<TileType> tileType;

	void SetTileSprite(UPaperSprite* tileSprite);

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void TileClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION()
	void TileHover(class UPrimitiveComponent* OtherComp);

	void resetSelection(ETouchIndex::Type index, AActor* actor);

	int32 tileIndexInGrid;

private:
	class AGrid* grid;
};
