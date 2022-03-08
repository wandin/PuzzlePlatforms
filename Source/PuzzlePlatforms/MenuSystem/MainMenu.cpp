// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	
	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinGameButton != nullptr)) return false;
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	// Injecting dependencies from the GameInstance's functions Host and Join, to our MainMenu file.
	this->MenuInterface = _MenuInterface;
}

/* Setup Menu Widget*/
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

/* TearDown the Menu Widget */
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

/* Host a Server once the Host Button from the MainMenu is clicked*/
void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

/* Open the JoinMenu once the Join Button from the MainMenu is clicked*/
void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Open Join Menu!!"));
	MenuSwitcher->SetActiveWidget(JoinMenu);
	

}

/* Open the MainMenu once the Cancel Button from the JoinMenu is clicked*/
void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(CancelJoinMenuButton != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

/* Join Server with a IP address */
void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAddressField != nullptr)) return;

		const FString& Address = IPAddressField->GetText().ToString();
		MenuInterface->Join(Address);
	}
}
