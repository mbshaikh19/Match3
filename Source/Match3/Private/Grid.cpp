// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/SceneComponent.h"
//#include "Tile.h"

// Sets default values
AGrid::AGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	TilesInGrid.Empty(Grid_Columns*Grid_Rows);
	TilesInGrid.AddZeroed(TilesInGrid.Max());
	CreateGrid();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::CreateGrid()
{
	FVector tileSpawnLocation,gridLocation;
	gridLocation = GetActorLocation();
	int32 gridIndex;

	for (int32 col = 0; col < Grid_Columns; col++)
	{
		for (int32 row = 0; row < Grid_Rows; row++)
		{
			gridIndex = GetGridIndex(0,col,row);
			tileSpawnLocation = FVector(-(Grid_Columns/2)*Tile_Width+(Tile_Width/2),0.0f, -(Grid_Rows / 2) * Tile_Height + (Tile_Height / 2));
			tileSpawnLocation += FVector(Tile_Width *(float(gridIndex %Grid_Columns)),0.0f, Tile_Height * (float(gridIndex / Grid_Rows)));
			tileSpawnLocation += gridLocation;

			UE_LOG(LogTemp,Warning,TEXT("ggg Grid index = %d"),gridIndex)
			/*TilesInGrid[gridIndex] = */SpawnTile(GetRandomTile(), tileSpawnLocation);			
		}
	}
}

int32 AGrid::GetRandomTile()
{
	int32 tileIndex;
	tileIndex=FMath::FRandRange(0,TileArray.Num());

	return tileIndex;
}

int32 AGrid::GetGridIndex(int32 pivot, int32 x, int32 y)
{
	return (pivot+x+(y*Grid_Columns));
}

ATile* AGrid::SpawnTile(int32 tileToSpawnIndex, FVector spawnLocation)
{
	FActorSpawnParameters spawnParams;


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileArray[tileToSpawnIndex].TileClass, spawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	NewTile->SetTileSprite(TileArray[tileToSpawnIndex].TileSprite);
	NewTile->tileIndexInGrid = tileToSpawnIndex;
	//NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
	return NewTile;		
}

void AGrid::OnTileClicked(ATile* selectedTile)
{
	UE_LOG(LogTemp,Warning,TEXT("Tile selected index = %d"),selectedTile->tileIndexInGrid);

	if (selectedTiles.Num() == 0)
	{
		selectedTiles.AddUnique(selectedTile);
	}
	else
	{
		if (checkAdjacentTile(selectedTile->tileIndexInGrid))
		{
			UE_LOG(LogTemp,Warning,TEXT("gggg selected adjacent tile"))
			selectedTiles.AddUnique(selectedTile);
		}
		else
		{
			stopTileSelection = true;
			selectedTiles.Empty();
			selectedTileDifference = 0;
		}
	}

}

bool AGrid::checkAdjacentTile(int32 selectedTileIndex)
{
	int32 indexDifference = FMath::Abs(selectedTileIndex - selectedTiles[selectedTiles.Num()-1]->tileIndexInGrid);
	
	if (selectedTiles.Num() == 1 && ( indexDifference == 1 || indexDifference == Grid_Columns))
	{
		selectedTileDifference = indexDifference;
		return true;
	}

	if (indexDifference == selectedTileDifference)
	{
		return true;
	}
	return false;
}