// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCursor.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UPlayerCursor : public UUserWidget
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* cursorImage;
	
};
