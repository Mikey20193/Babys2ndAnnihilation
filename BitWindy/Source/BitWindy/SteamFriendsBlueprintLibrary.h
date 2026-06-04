// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "SteamFriendsBlueprintLibrary.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSteamFriendData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(BlueprintReadOnly)
    FString RealName;

    UPROPERTY(BlueprintReadOnly)
    FString SteamId;

    UPROPERTY(BlueprintReadOnly)
    FString PresenceStatus;
};

UCLASS()
class BITWINDY_API USteamFriendsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable, Category = "Steam|Friends")
    static TArray<FSteamFriendData> GetSteamFriends();

	
};
