// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PG_UtilityFunctionLibrary.h"

#include "Core/PG_ProjectGamma_Settings.h"

bool UPG_UtilityFunctionLibrary::GetUseBackEnd()
{
	return GetDefault<UPG_ProjectGamma_Settings>()->UseBackEnd;
}
