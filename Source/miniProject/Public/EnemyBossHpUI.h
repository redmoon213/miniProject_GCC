// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBossHpUI.generated.h"

/**
 * 보스 체력 바를 관리하는 UI 클래스입니다.
 */
UCLASS()
class MINIPROJECT_API UEnemyBossHpUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 블루프린트 위젯과 바인딩할 프로그레스 바 (이름이 일치해야 함)
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BossHPBar;

	// 블루프린트 위젯과 바인딩할 보스 이름 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossNameText;

	// 애니메이션 관련 변수
	bool bIsFillingUp = false;
	float CurrentAnimationTime = 0.0f;
	float TargetFillDuration = 2.5f;

	// 위젯의 Tick 함수 (매 프레임 호출됨)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/**
	 * 보스 체력 바가 서서히 차오르는 애니메이션을 시작합니다.
	 * @param Duration 차오르는 데 걸리는 시간 (초)
	 */
	void StartFillingUp(float Duration = 2.5f);

	/**
	 * 애니메이션이 차오르는 총 시간을 반환합니다.
	 */
	float GetFillDuration() const { return TargetFillDuration; }

	/**
	 * 보스의 현재 체력 비율을 UI에 반영합니다.
	 * (애니메이션 중에는 호출되어도 무시되거나 애니메이션 종료 후 적용되도록 설계 가능)
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
