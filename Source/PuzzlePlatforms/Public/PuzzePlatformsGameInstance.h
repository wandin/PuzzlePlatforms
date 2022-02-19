// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzePlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPuzzePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	
	
	virtual void Init();
	
	/* Adds a function that can be executable from the command line*/
	UFUNCTION(Exec)
	void Host();

	/* Adds a function that can be executable from the command line*/
	UFUNCTION(Exec)
	void Join(const FString& Address);
};
