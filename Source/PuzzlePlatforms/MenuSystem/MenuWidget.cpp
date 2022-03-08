// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::Setup()
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

void UMenuWidget::Teardown()
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

void UMenuWidget::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	// Injecting dependencies from the GameInstance's functions Host and Join, to our MainMenu file.
	this->MenuInterface = _MenuInterface;
}