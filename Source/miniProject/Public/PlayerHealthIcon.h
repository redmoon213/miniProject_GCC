// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthIcon.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UPlayerHealthIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* healthIcon;
	
	UPROPERTY(EditAnywhere)
	class UTexture2D* fullTexture;
	UPROPERTY(EditAnywhere)
	class UTexture2D* emptyTexture;
	
	void SetIsFull(bool bIsFull);
	
};
