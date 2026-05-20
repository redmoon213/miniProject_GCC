// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class MINIPROJECT_API APortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	class UDecalComponent* decalComp;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPortalIndicator> indicatorClass;
	
	class UPortalIndicator* indicatorInstance;
	
	UPROPERTY(EditAnywhere)
	FName NextLevelName;
	
	UFUNCTION()
	void OnPortalOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool bIsPortalOpen = false;
	void ShowPortal();
};
