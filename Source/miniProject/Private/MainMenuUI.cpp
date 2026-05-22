// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 시작 버튼에 클릭 이벤트 함수를 연결합니다.
	if (btnStart)
	{
		btnStart->OnClicked.AddDynamic(this, &UMainMenuUI::OnStartClicked);
	}

	// 종료 버튼에 클릭 이벤트 함수를 연결합니다.
	if (btnQuit)
	{
		btnQuit->OnClicked.AddDynamic(this, &UMainMenuUI::OnQuitClicked);
	}
}

void UMainMenuUI::OnStartClicked()
{
	// "Main"이라는 이름의 레벨을 엽니다. 
	// UGameplayStatics::OpenLevel은 지정된 이름의 맵으로 전환하는 기능을 합니다.
	UE_LOG(LogTemp, Warning, TEXT("MainMenu: Start Button Clicked!"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
}

void UMainMenuUI::OnQuitClicked()
{
	// 게임을 종료합니다.
	// UKismetSystemLibrary::QuitGame은 실행 중인 게임 프로세스를 안전하게 닫습니다.
	UE_LOG(LogTemp, Warning, TEXT("MainMenu: Quit Button Clicked!"));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
