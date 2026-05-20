// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartUI.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void URestartUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (btnRestart)
	{
		btnRestart->OnClicked.AddDynamic(this, &URestartUI::OnRestartClicked);
		
	}
	if (btnQuit)
	{
		btnQuit->OnClicked.AddDynamic(this, &URestartUI::OnQuitClicked);
	}
}

void URestartUI::OnRestartClicked()
{
	UGameplayStatics::OpenLevel(this, "Main");
}

void URestartUI::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit,false);
}
