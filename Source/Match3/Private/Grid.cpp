// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "SceneManagement.h"
#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
//#include "Tile.h"

// Sets default values
AGrid::AGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	Line_PS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Line_PS"));
	Line_PS->SetupAttachment(RootComponent);
	Line_PS->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<AMatch3_PC>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (playerController)
	{
		playerController->SetGridRef(this);
	}
	TilesInGrid.Empty(Grid_Columns*Grid_Rows);
	TilesInGrid.AddZeroed(TilesInGrid.Max());
	CreateGrid();
	Line_PS = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), lineParticleTemplate, GetActorLocation(), FRotator::ZeroRotator, true);
	Line_PS->SetVisibility(false);
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
			tileSpawnLocation = GridIndexToLocation(gridIndex);
			UE_LOG(LogTemp,Warning,TEXT("ggg Grid index = %d"),gridIndex)
			SpawnTile(GetRandomTile(), tileSpawnLocation,gridIndex);			
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

ATile* AGrid::SpawnTile(int32 tileToSpawnIndex, FVector spawnLocation,int32 tileGridIndex)
{
	FActorSpawnParameters spawnParams;


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileArray[tileToSpawnIndex].TileClass, spawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	NewTile->SetTileSprite(TileArray[tileToSpawnIndex].TileSprite);
	NewTile->tileType = TileArray[tileToSpawnIndex].tileType;
	NewTile->tileIndexInGrid = tileGridIndex;
	TilesInGrid[tileGridIndex] = NewTile;
	return NewTile;		
}

void AGrid::OnTileClicked(ATile* selectedTile)
{
	UE_LOG(LogTemp,Warning,TEXT("Tile selected index = %d"),selectedTile->tileIndexInGrid);

	if (selectedTiles.Num() == 0)
	{
		selectedTiles.AddUnique(selectedTile);
		selectedTileType = selectedTile->tileType;
	}
	else
	{
		if (checkAdjacentTile(selectedTile->tileIndexInGrid)&& selectedTile->tileType== selectedTileType)
		{
			UE_LOG(LogTemp,Warning,TEXT("gggg selected adjacent tile"))
			selectedTiles.AddUnique(selectedTile);
			
			DrawLineBetweenTiles(selectedTiles[0]->GetActorLocation()+FVector(0.0f,2.0f,0.0f),selectedTiles.Last()->GetActorLocation() + FVector(0.0f, 2.0f, 0.0f));
		}
		else
		{
			stopTileSelection = true;
			selectedTiles.Empty();
			HideLine();
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

void AGrid::MouseBtnReleased()
{
	if (selectedTiles.Num() >= minimumSelectionLength)
	{
		playerController->SetScore(selectedTiles.Num());
		for (ATile* tileToDestroy : selectedTiles)
		{
			destroyingTileIDs.AddUnique(tileToDestroy->tileIndexInGrid);

			//adding just to be extra sure for respawn ids of tiles
			respawnIds.Add(tileToDestroy->tileIndexInGrid);
			TilesInGrid[tileToDestroy->tileIndexInGrid] = nullptr;
			
			tileToDestroy->Destroy();
		}

		FillEmptySpace();
		stopTileSelection = false;
		selectedTiles.Empty();
		selectedTileDifference = 0;
		destroyingTileIDs.Empty();
		HideLine();
	}
	else
	{
		stopTileSelection = false;
		selectedTiles.Empty();
		HideLine();
		//selectedTileDifference = 0;
	}
}

void AGrid::FillEmptySpace()
{
	UE_LOG(LogTemp,Warning,TEXT("ggg fill empty space called step 01"));
	TArray<int32> fallingTiles;
	//if selection happend in row
	if (selectedTileDifference==1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 02 destroyingTileIDs.Num()=%d"), destroyingTileIDs.Num());
		//Get all falling tile list
		for (int j = 0; j < destroyingTileIDs.Num(); j++)
		{
			int upperTiles = FMath::Abs((destroyingTileIDs[0] / Grid_Rows) - Grid_Rows);
			UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 02.8 upperTiles=%d   , id[0]=%d"), upperTiles, destroyingTileIDs[0]);
			for (int i = 1; i < upperTiles; i++)
			{
				fallingTiles.Add(destroyingTileIDs[j] + (Grid_Rows * i));
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("ggg step 2.5 fallingTiles.Num()= %d"), fallingTiles.Num());
		//push tile ids to empty location
		for (int i = 1; i <= fallingTiles.Num(); i++)
		{
			int32 fallingTilNewID= FMath::Abs(fallingTiles[i - 1] - Grid_Rows);
			ATile* temp;
			if (TilesInGrid[fallingTiles[i - 1]])
			{
				TilesInGrid[fallingTiles[i - 1]]->SetActorLocation(GridIndexToLocation(fallingTilNewID));
				TilesInGrid[fallingTiles[i - 1]]->tileIndexInGrid = fallingTilNewID;
				temp=TilesInGrid[fallingTiles[i - 1]];
				respawnIds.Add(fallingTiles[i - 1]);
				TilesInGrid[fallingTiles[i - 1]] = nullptr;
				TilesInGrid[fallingTilNewID] = temp;
			}
		}
	}//if selection happend in column
	else if (selectedTileDifference == Grid_Rows)
	{
		UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 05"));
		destroyingTileIDs.Sort();
		int32 maxId = destroyingTileIDs.Last(),minId= destroyingTileIDs[0];
		if(fallingTiles.Num()>0)
			fallingTiles.Empty();
		UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 05.1 last = %d , min = %d"),maxId, minId);
		//make falling tile list
		for (int i = 1; i < FMath::Abs(((maxId / Grid_Rows) - Grid_Rows)); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 06"));
			fallingTiles.Add(maxId + (Grid_Rows * i));
		}

		for (int i=1; i < FMath::Abs(((maxId / Grid_Rows)-Grid_Rows)); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 07"));
			int32 fallingTilNewID = FMath::Abs(fallingTiles[i - 1] - (destroyingTileIDs.Num()*Grid_Rows));
			ATile* temp;
			if (TilesInGrid[fallingTiles[i - 1]])
			{
				UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 08"));
				TilesInGrid[fallingTiles[i - 1]]->SetActorLocation(GridIndexToLocation(fallingTilNewID));
				TilesInGrid[fallingTiles[i - 1]]->tileIndexInGrid = fallingTilNewID;
				temp = TilesInGrid[fallingTiles[i - 1]];
				respawnIds.Add(fallingTiles[i - 1]);
				TilesInGrid[fallingTiles[i - 1]] = nullptr;
				TilesInGrid[fallingTilNewID] = temp;
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 09"));
	if (fallingTiles.Num() == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ggg fill empty space called step 10 = %d"), destroyingTileIDs[0]);
		for (int32 tempId : destroyingTileIDs)
		{
			TilesInGrid[tempId] = nullptr;
		}
		respawnTiles(destroyingTileIDs);
	}
	else {
		respawnTiles(respawnIds);
	}
	selectedTileDifference = 0;
}

FVector AGrid::GridIndexToLocation(int32 index)
{
	FVector tileSpawnLocation, gridLocation;
	gridLocation = GetActorLocation();
	tileSpawnLocation = FVector(-(Grid_Columns / 2) * Tile_Width + (Tile_Width / 2), 0.0f, -(Grid_Rows / 2) * Tile_Height + (Tile_Height / 2));
	tileSpawnLocation += FVector(Tile_Width * (float(index % Grid_Columns)), 0.0f, Tile_Height * (float(index / Grid_Rows)));
	tileSpawnLocation += gridLocation;
	return tileSpawnLocation;
}

void AGrid::respawnTiles(TArray<int32> respawnIndex)
{
	for (int32 emptyId : respawnIndex)
	{
		if (TilesInGrid[emptyId] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ggg respawn id = %d"), emptyId);
			SpawnTile(GetRandomTile(), GridIndexToLocation(emptyId), emptyId);
		}

	}

	if(respawnIds.Num()>0)
	respawnIds.Empty();
}

void AGrid::DrawLineBetweenTiles(const FVector startPoint,const FVector endPoint)
{	
	Line_PS->SetVisibility(true);
	Line_PS->Template = lineParticleTemplate;
	//if (Line_PS->IsValidLowLevel())
	//{
		Line_PS->ActivateSystem();
		Line_PS->SetBeamSourcePoint(0, startPoint, 0);
		Line_PS->SetBeamTargetPoint(0, endPoint, 0);
	//}
		UE_LOG(LogTemp,Warning,TEXT("gggg start point = %s , end point = %s "),*startPoint.ToString(),*endPoint.ToString());
}

void AGrid::HideLine()
{
	Line_PS->SetVisibility(false);
		Line_PS->DeactivateSystem();
		Line_PS->SetBeamSourcePoint(0, FVector::ZeroVector, 0);
		Line_PS->SetBeamTargetPoint(0, FVector::ZeroVector, 0);
}