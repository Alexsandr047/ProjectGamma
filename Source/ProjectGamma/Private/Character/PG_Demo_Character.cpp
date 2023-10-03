// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PG_Demo_Character.h"

#include "Character/PG_CharacterPresets_Component.h"

APG_Demo_Character::APG_Demo_Character(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CharacterPresets = CreateDefaultSubobject<UPG_CharacterPresets_Component>(TEXT("CharacterPresets"));
}
