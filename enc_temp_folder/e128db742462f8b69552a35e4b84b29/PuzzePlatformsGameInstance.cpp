// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "PuzzlePlatforms/MenuSystem/MainMenu.h"
#include "PuzzlePlatforms/MenuSystem/MenuWidget.h"


const static FName SESSION_NAME = TEXT("GameSession"); // declares the Session name, pretty obvious
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName"); // declaring a const server name

UPuzzePlatformsGameInstance::UPuzzePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;

}

void UPuzzePlatformsGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) //boolean check for shared pointer IOnlineSessionPtr
		{
			// creates session
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzePlatformsGameInstance::OnCreateSessionComplete); 
			// destroys session
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzePlatformsGameInstance::OnDestroySessionComplete);
			// Find Session
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzePlatformsGameInstance::OnFindSessionComplete);
			// Joining Session using server button Index from menu
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem"));
	}
}

void UPuzzePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	_Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(_Menu != nullptr)) return;

	_Menu->Setup();
	_Menu->SetMenuInterface(this);
}

void UPuzzePlatformsGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* _Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(_Menu != nullptr)) return;

	_Menu->Setup();
	_Menu->SetMenuInterface(this);
}

// When pressing HostButton, creates a session(SESSION_NAME) "My Session", if a session already exists destroys the session and creates one
void UPuzzePlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid()) //boolean
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME); // Destroy Session, if a session is already created.
		}
		else
		{
			CreateSession();
		}
	}
}

// CREATE SESSION
void UPuzzePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

// if CreatesSession works, then opens the level
void UPuzzePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeded)
{
	if (!Succeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create My Session"));
		return;
	}
	if (_Menu != nullptr)
	{
		_Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
}

// After Destroy Session, creates a session.
void UPuzzePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeded)
{
	if(Succeded)
	{
		CreateSession();
	}
}

void UPuzzePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;

		SessionSearch->MaxSearchResults = 100;
		// Search player presences in session
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		
	}
}

void UPuzzePlatformsGameInstance::OnFindSessionComplete(bool Succeded)
{
		if (Succeded && SessionSearch.IsValid() && _Menu != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Finished finding Session"));

			TArray<FServerData> ServerNames;

			for (auto& SearchResult : SessionSearch->SearchResults) // iterates through the Sessions found
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Session Names: %s"), *SearchResult.GetSessionIdStr());

				FServerData Data;
				Data.Name = SearchResult.GetSessionIdStr();
				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections + 1;
				Data.HostUserName = SearchResult.Session.OwningUserName;

				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				{
					Data.Name = ServerName;
				}
				else
				{
					Data.Name = "Could not find a server name input";
				}
				ServerNames.Add(Data);
			}

			_Menu->SetServerList(ServerNames);
		}
}

void UPuzzePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (_Menu != nullptr)
	{
		_Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}

void UPuzzePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC != nullptr)) return;

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC != nullptr)) return;

	PC->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}


