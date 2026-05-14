// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyIndicatorWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"


void UEnemyIndicatorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
    
	if (!IsValid(targetActor) || !arrowImage)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	// 2. 모든 계산을 '픽셀(Pixel) 좌표'로 통일합니다! (DPI 스케일 직접 계산 X)
	FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	FVector2D screenCenter = viewportSize / 2.0f;
    
	FVector2D screenPos;
	bool bProjected = PC->ProjectWorldLocationToScreen(targetActor->GetActorLocation(), screenPos);

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
	// 5. 방향 계산 (픽셀 - 픽셀 이므로 완벽하게 작동!)
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
    
	
	
	// 엔진에게 "내가 픽셀 좌표 줄 테니까 알아서 화면에 배치해!" 라고 요청
	SetPositionInViewport(finalPos, true);
    
	// ★ 보너스 핵심: 위젯의 중심점(Pivot)을 정중앙(0.5, 0.5)으로 맞춰야 테두리에 예쁘게 걸칩니다.
	SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
}


void UEnemyIndicatorWidget::SetTarget(AActor* newTarget)
{
	targetActor = newTarget;
}


