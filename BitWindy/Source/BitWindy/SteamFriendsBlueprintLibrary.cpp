// Fill out your copyright notice in the Description page of Project Settings.
#include "SteamFriendsBlueprintLibrary.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Interfaces/OnlineSessionInterface.h"


TArray<FSteamFriendData> USteamFriendsBlueprintLibrary::GetSteamFriends()
{
    TArray<FSteamFriendData> Result;

    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GWorld, "Steam");
    if (!Subsystem) return Result;

    IOnlineFriendsPtr Friends = Subsystem->GetFriendsInterface();
    if (!Friends.IsValid()) return Result;

    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (!Identity.IsValid()) return Result;

    TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
    if (!UserId.IsValid()) return Result;

    TArray<TSharedRef<FOnlineFriend>> FriendList;
    Friends->GetFriendsList(0, TEXT("default"), FriendList);

    for (auto& Friend : FriendList)
    {
        FSteamFriendData Data;
        Data.DisplayName = Friend->GetDisplayName();
        Data.RealName = Friend->GetRealName();
        Data.SteamId = Friend->GetUserId()->ToString();

        const FOnlineUserPresence& Presence = Friend->GetPresence();
        Data.PresenceStatus = Presence.Status.StatusStr;

        Result.Add(Data);
    }

    return Result;
}

