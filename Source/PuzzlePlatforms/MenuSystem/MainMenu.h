// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 Index);

protected:

	virtual bool Initialize() override;

private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

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
	class UPanelWidget* ServerList;

	
	TOptional<uint32> SelectedIndex;


	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitGame();

};
