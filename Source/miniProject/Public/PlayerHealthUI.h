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

	
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* healthBox;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHealthIcon> healthIconClass;
	
	UPROPERTY()
	TArray<class UPlayerHealthIcon*> healthIcons;
	
	UPROPERTY(EditAnywhere)
	int32 maxHealth = 6;
	
	
	//플레이어 피격시 애니메이션
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* hitVignetteImage;
	
	class UMaterialInstanceDynamic* hitVigetteMaterial;
	void StartPlayerHitAnimation();
	FTimerHandle playerHitAnimationtTimerHandle;
	void EndPlayerHitAnimation();
protected:
	virtual void NativeConstruct() override;
};
