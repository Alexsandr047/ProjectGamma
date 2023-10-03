#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "CIS_ItemFragment_DescriptionItem.generated.h"


UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemFragment_DescriptionItem : public UCIS_ItemFragment
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	FString DescriptionItemID;
};
