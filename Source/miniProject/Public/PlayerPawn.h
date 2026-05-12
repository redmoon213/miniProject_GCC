// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class MINIPROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//박스콜리전과 스태틱메시 컴포넌트
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	
	// Category -> 에디터에 찾기 쉽게 "Camera"탭이라는 이름을 붙여줌.
	UPROPERTY(EditAnywhere, Category="Camera")
	class USpringArmComponent* springArmComp;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	class UCameraComponent* cameraComp;
	
	//입력관련 변수들
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imcPlayerInput;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaHorizontal;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaVertical;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaFire;
	
	
	UPROPERTY(EditAnywhere)
	float movementSpeed = 500.0f;
	
	// 플레이어 커서 위젯을 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerCursor> playerCursorClass;
	
	//총구 컴포넌트 
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletPlayerBasic> bulletFactory;
	
	//발사효과음
	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;
	
	
	
	
private:
	float h;
	float v;
	
	class UPlayerCursor* playerCursorInstance;
	
	void OnInputVertical(const struct FInputActionValue& value);
	void OnInputHorizontal(const struct FInputActionValue& value);
	
	void Fire(const struct FInputActionValue& value);
};
