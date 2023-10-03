// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CIS_InteractionWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_InteractionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UCIS_InteractionWidget(const FObjectInitializer& ObjectInitializer);

		
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTarget(UObject* NewTarget);
	virtual void SetTarget_Implementation(UObject* NewTarget);

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UObject* Target;

protected:
	
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NewObjectReceived();
	
	UFUNCTION(BlueprintCallable)
	void GetObjectToSreenPostion(FVector2D& Position) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void GetObjectLocation(FVector& Output) const;
	virtual void GetObjectLocation_Implementation(FVector& Output) const;
};
