// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "Tile.h"
#include "Match3_PC.h"
#include "Grid.generated.h"

//class ATile;
USTRUCT (BlueprintType)
struct FTileProperties
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<TileType> tileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* TileSprite;
};

UCLASS()
class MATCH3_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FTileProperties> TileArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Grid_Columns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Grid_Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Tile_Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Tile_Height;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<ATile*> TilesInGrid;

	UPROPERTY()
	TArray<ATile*> selectedTiles;

	int32 selectedTileDifference;

	bool stopTileSelection = false;

	UPROPERTY()
	AMatch3_PC* playerController;

	int32 minimumSelectionLength=3;

	TEnumAsByte<TileType> selectedTileType;

	TArray<int32> destroyingTileIDs;

	void FillEmptySpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateGrid();

	int32 GetRandomTile();
	int32 GetGridIndex(int32 pivot,int32 x,int32 y);
	ATile* SpawnTile(int32 tileToSpawnIndex,FVector spawnLocation, int32 tileGridIndex);

	bool checkAdjacentTile(int32 selectedTileIndex);

	UFUNCTION()
	void OnTileClicked(ATile* selectedTile);

	UFUNCTION()
	void MouseBtnReleased();

	FVector GridIndexToLocation(int32 index);
};
