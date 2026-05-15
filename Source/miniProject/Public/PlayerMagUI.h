// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMagUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UPlayerMagUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdataAmmo(int32 currentAmmo, int32 maxAmmo);
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* ammoRingImage;
	
	UPROPERTY()
	UMaterialInstanceDynamic* ammoRingMID;	

	
};
