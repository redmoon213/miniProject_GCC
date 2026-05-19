// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBossSpawnIndicator.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UEnemyBossSpawnIndicator : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* warningIcon;
};
