// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:

	virtual bool Initialize() override;

private:

	TSubclassOf<class UUserWidget> ServerRowClass;


	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* HostServerButton;

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

	/* declaring the Main Menu, which will open in different circumstances*/
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	/* declaring the Host Menu, which will open using the WidgetSwitcher when the Hostbutton is clicked*/
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameBox;

	/* declaring Buttons on Widget created in UE*/
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostMenuButton;
	
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
	void OpenHostMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void UpdateChildren();
};
