﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"

/** Handles reporting progress from streamable handles */
DECLARE_DELEGATE_OneParam(FCIS_AssetManagerStartupJobSubstepProgress, float /*NewProgress*/);


struct FCIS_AssetManagerStartupJob
{
	FCIS_AssetManagerStartupJobSubstepProgress SubstepProgressDelegate;
	TFunction<void(const FCIS_AssetManagerStartupJob&, TSharedPtr<FStreamableHandle>&)> JobFunc;
	FString JobName;
	float JobWeight;
	mutable double LastUpdate = 0;

	/** Simple job that is all synchronous */
	FCIS_AssetManagerStartupJob(const FString& InJobName, const TFunction<void(const FCIS_AssetManagerStartupJob&, TSharedPtr<FStreamableHandle>&)>& InJobFunc, float InJobWeight)
		: JobFunc(InJobFunc)
		, JobName(InJobName)
		, JobWeight(InJobWeight)
	{}

	/** Perform actual loading, will return a handle if it created one */
	TSharedPtr<FStreamableHandle> DoJob() const;

	void UpdateSubstepProgress(float NewProgress) const
	{
		SubstepProgressDelegate.ExecuteIfBound(NewProgress);
	}

	void UpdateSubstepProgressFromStreamable(TSharedRef<FStreamableHandle> StreamableHandle) const
	{
		if (SubstepProgressDelegate.IsBound())
		{
			// StreamableHandle::GetProgress traverses() a large graph and is quite expensive
			double Now = FPlatformTime::Seconds();
			if (LastUpdate - Now > 1.0 / 60)
			{
				SubstepProgressDelegate.Execute(StreamableHandle->GetProgress());
				LastUpdate = Now;
			}
		}
	}
};
