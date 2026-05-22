// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEnemyBossSpawnIndicator> bossSpawnIndicatorClass;
	UEnemyBossSpawnIndicator* BossSpawnIndicatorInstance;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class URestartUI> restartUIClass;
	UPROPERTY()
	class URestartUI* restartUIInstance;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyBossHpUI> bossHpUIClass;
	UPROPERTY()
	class UEnemyBossHpUI* bossHpUIInstance;

	void ShowBossSpawnWarning();
	void HideBossSpawnWarning();
	
	// 보스 스폰 경고 사운드 변수입니다.
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* bossSpawnWarningSound;

	// 사운드 중지를 위해 오디오 컴포넌트를 저장할 변수입니다.
	UPROPERTY()
	class UAudioComponent* bossSpawnWarningAudioComp;
	
	void ShowRestartUI();

	// 마우스 커서와 입력 모드를 메인 메뉴용으로 설정하는 함수입니다.
	void SetMenuInputMode(class UUserWidget* MenuWidget);

	// 보스 체력바 제어 함수
	void ShowBossHPBar(FString BossName);
	void UpdateBossHP(float CurrentHP, float MaxHP);
	void HideBossHPBar();
};
