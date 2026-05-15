// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UPlayerHealthUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateHealthIcon(int32 currentHealth);

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* healthBox;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHealthIcon> healthIconClass;
	
	UPROPERTY()
	TArray<class UPlayerHealthIcon*> healthIcons;
	
	UPROPERTY(EditAnywhere)
	int32 maxHealth = 6;
	

};
