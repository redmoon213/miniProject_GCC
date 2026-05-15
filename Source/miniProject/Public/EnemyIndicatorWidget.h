// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyIndicatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UEnemyIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTarget(AActor* Target);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* arrowImage;
	
	UPROPERTY(EditAnywhere, Category="Indicator")
	float screenMargin = 50.0f;
	
private:
	
	UPROPERTY()
	AActor* targetActor;

	UPROPERTY()
	class APlayerController* cachedPC;
	
};
