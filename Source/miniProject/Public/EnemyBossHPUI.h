// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBossHPUI.generated.h"

/**
 * 보스 체력 바를 관리하는 UI 클래스입니다.
 */
UCLASS()
class MINIPROJECT_API UEnemyBossHPUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 블루프린트 위젯과 바인딩할 프로그레스 바 (이름이 일치해야 함)
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BossHPBar;

	// 블루프린트 위젯과 바인딩할 보스 이름 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossNameText;

public:
	/**
	 * 보스의 현재 체력 비율을 UI에 반영합니다.
	 * @param CurrentHP 현재 체력
	 * @param MaxHP 최대 체력
	 */
	void UpdateHPBar(float CurrentHP, float MaxHP);

	/**
	 * 보스의 이름을 설정합니다.
	 * @param BossName 표시할 보스 이름
	 */
	void SetBossName(FString BossName);
};
