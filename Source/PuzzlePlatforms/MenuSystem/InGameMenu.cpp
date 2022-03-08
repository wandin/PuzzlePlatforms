// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelMenu);

	if (!ensure(QuitToMenuButton != nullptr)) return false;
	QuitToMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitToMenu);

	return true;
}

void UInGameMenu::CancelMenu()
{
	Teardown();
}

void UInGameMenu::QuitToMenu()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
