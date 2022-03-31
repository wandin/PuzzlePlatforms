// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "PuzzePlatformsGameInstance.h"
#include "TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartCounter, this, &ALobbyGameMode::StartGame, 10);
		
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	// using PuzzlePlatformGameInstance.h specifically
	auto GameInstance = Cast<UPuzzePlatformsGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	GameInstance->StartSession();
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}