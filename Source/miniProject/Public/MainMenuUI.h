// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 생성 시 호출되는 초기화 함수입니다.
	virtual void NativeConstruct() override;

	// 배경 이미지를 담을 변수입니다. 에디터 위젯 블루프린트의 이름과 일치해야 합니다.
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* imgBackground;

	// 게임 시작 버튼입니다.
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnStart;

	// 게임 종료 버튼입니다.
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnQuit;

private:
	// 시작 버튼 클릭 시 실행될 함수입니다.
	UFUNCTION()
	void OnStartClicked();

	// 종료 버튼 클릭 시 실행될 함수입니다.
	UFUNCTION()
	void OnQuitClicked();
};
