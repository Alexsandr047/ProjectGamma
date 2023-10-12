// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PG_UtilityFunctionLibrary.h"

#include "Core/PG_ProjectGamma_Settings.h"

bool UPG_UtilityFunctionLibrary::GetUseBackEnd()
{
	return GetDefault<UPG_ProjectGamma_Settings>()->UseBackEnd;
}

FString UPG_UtilityFunctionLibrary::GenerateID(int32 Length)
{
	FString Id = "";
	TArray<FString> Symbols {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
		"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
		"0","1","2","3","4","5","6","7","8","9" };

	for(int32 i = 0; i < Length; ++i )
	{
		Id.Append(Symbols[FMath::RandRange(1,Symbols.Num())-1]);
	}
	return Id;
}
