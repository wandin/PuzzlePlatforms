// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "Components/WidgetSwitcher.h"
#include "MainMenu.generated.h"


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetMenuInterface(IMenuInterface* _MenuInterface);

	void Setup();
	void Teardown();

protected:

	virtual bool Initialize() override;

private:

	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	/* declaring the Widget Switcher for switching widgets when Join button is clicked*/
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	/* declaring the Main Menu, which will open using the WidgetSwitcher when the CancelJoinMenuButton is clicked*/
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	/* declaring the Join Menu, which will open using the WidgetSwitcher when the Joinbutton is clicked*/
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressField;


	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void JoinServer();

	IMenuInterface* MenuInterface;
};
