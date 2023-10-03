// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CIS_EquipmentItem.generated.h"

class ACIS_Character;
class UCIS_AttachmentLogic;
class ACIS_AttachmentItem;


UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_EquipmentItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACIS_EquipmentItem();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	USkeletalMeshComponent* SkeletalMeshComponent;*/
	
	UFUNCTION(BlueprintCallable)
	void SetCharacterParam(ACIS_Character* Character);	
	void SetSocketOnCharacter(FName Socket);
	void SetAttachmentItems(TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentItems);
	
	virtual void InitAttachableElement();

	UFUNCTION(BlueprintCallable)
	ACIS_Character* GetOwningCharacter() { return OwningCharacter; };

	UFUNCTION()
	TArray<AActor*> GetAttachmentActors() {return AttachmentActors; }
private:
	
	UFUNCTION()
	virtual void OnRep_OwningCharacter();

	UFUNCTION()
	virtual void OnRep_AttachableElements();
	
protected:

	
	UPROPERTY(ReplicatedUsing = OnRep_OwningCharacter)
	ACIS_Character* OwningCharacter;

	UPROPERTY(Replicated)
	FName SocketOnCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AttachableElements)
	TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentElements;

	UPROPERTY()
	TArray<AActor*> AttachmentActors;
};
