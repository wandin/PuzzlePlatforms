// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	if (!ensure(Host != nullptr))
	{
		return false;
	}

	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

		return true;
}


void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	/* Enables controller to be able to click on the menu*/
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	/* set input mode and make the cursor visible */
	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	/* Enables controller to be used in game mode again*/
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	FInputModeGameOnly InputModeData;
	/* set input mode back to game mode */
	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = false;
}

void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	// Injecting dependencies from the GameInstance's functions Host and Join, to our MainMenu file.
	this->MenuInterface = _MenuInterface;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}
