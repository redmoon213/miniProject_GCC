// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyIndicatorWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"


void UEnemyIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// PlayerController를 미리 찾아서 변수에 저장(캐싱)합니다.
	cachedPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}


void UEnemyIndicatorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
    
	if (!IsValid(targetActor) || !arrowImage)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 캐싱된 컨트롤러가 유효한지 확인합니다.
	if (!cachedPC)
	{
		return;
	}

	// 2. 모든 계산을 '픽셀(Pixel) 좌표'로 통일합니다! (DPI 스케일 직접 계산 X)
	FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	FVector2D screenCenter = viewportSize / 2.0f;
    
	FVector2D screenPos;
	
	// 수정 전 코드
	// bool bProjected = PC->ProjectWorldLocationToScreen(targetActor->GetActorLocation(), screenPos);
	
	// 캐싱된 컨트롤러를 사용하여 월드 좌표를 화면 좌표로 변환합니다.
	bool bProjected = cachedPC->ProjectWorldLocationToScreen(targetActor->GetActorLocation(), screenPos);

	// 3. 화면 안에 있는지 체크 (픽셀 기준)
	if (bProjected && 
	   screenPos.X >= screenMargin && screenPos.X <= viewportSize.X - screenMargin && 
	   screenPos.Y >= screenMargin && screenPos.Y <= viewportSize.Y - screenMargin)
	{
		
		SetRenderOpacity(0.0f);
		return;
	}
	
	// 4. 화면 밖일 때 표시
	
	SetRenderOpacity(1.0f);
	// 5. 방향 계산, 투영된 위치에서 화면의 중앙좌표를 빼서 방향을 구함.
	FVector2D dir = screenPos - screenCenter;
	if (!bProjected) 
	{
		dir *= -1.0f; 
	}

	// 6. 회전 업데이트
	float angle = FMath::RadiansToDegrees(FMath::Atan2(dir.Y, dir.X)) + 90.0f;
	arrowImage->SetRenderTransformAngle(angle);

	// 7. 가장자리 고정 계산
	float boundsX = screenCenter.X - screenMargin;
	float boundsY = screenCenter.Y - screenMargin;
	float absDirX = FMath::Abs(dir.X);
	float absDirY = FMath::Abs(dir.Y);

	if (absDirX * boundsY > absDirY * boundsX) 
	{
		dir *= (boundsX / absDirX);
	}
	else 
	{
		dir *= (boundsY / absDirY);
	}	
    
	// 8. 최종 위치 설정 (픽셀 좌표)
	FVector2D finalPos = screenCenter + dir;
    
	
	
	// 최종 위치를 뷰포트에 배치시킴.
	SetPositionInViewport(finalPos, true);
    
	// 위젯의 피벗을 중앙(0.5, 0.5)에 맞춰 화면 가장자리에 제대로 걸쳐 보일 수 있게 함.
	SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
}


void UEnemyIndicatorWidget::SetTarget(AActor* newTarget)
{
	targetActor = newTarget;
}


