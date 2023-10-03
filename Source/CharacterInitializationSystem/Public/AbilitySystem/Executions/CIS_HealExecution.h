#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CIS_HealExecution.generated.h"


/**
 * UCIS_HealExecution
 *
 *	Execution used by gameplay effects to apply healing to the health attributes.
 */
UCLASS()
class UCIS_HealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCIS_HealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
