// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CIS_PawnComponent.h"

UCIS_PawnComponent::UCIS_PawnComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}
