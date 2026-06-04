#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/OnlineSessionInterface.h"      // For EOnJoinSessionCompleteResult, FUniqueNetId, etc.
#include "BlueprintDataDefinitions.h"              // From Advanced Sessions (FBlueprintSessionResult)
#include "OnlineSessionSettings.h"
#include "InviteListener.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInviteListener_OnInviteReceived, FBlueprintSessionResult, SessionResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInviteListener_OnInviteAccepted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInviteListener_OnJoinSessionComplete);

UCLASS(BlueprintType, Blueprintable)

class BITWINDY_API UInviteListener : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void Initialize();

    // Events exposed to Blueprint
    UPROPERTY(BlueprintAssignable)
    FInviteListener_OnInviteReceived OnInviteReceived;

    UPROPERTY(BlueprintAssignable)
    FInviteListener_OnInviteAccepted OnInviteAccepted;

    UPROPERTY(BlueprintAssignable)
    FInviteListener_OnJoinSessionComplete OnJoinSessionComplete;

    // Blueprint getter for the stored session result
    UFUNCTION(BlueprintCallable)
    FBlueprintSessionResult GetLastInviteSession() const;

private:
    // Store the Advanced Sessions type, not the raw OnlineSubsystem type
    FBlueprintSessionResult LastInviteResult;

    // Delegate handles
    FDelegateHandle InviteReceivedHandle;
    FDelegateHandle InviteAcceptedHandle;
    FDelegateHandle JoinSessionCompleteHandle;

    // Internal handlers (must match OnlineSubsystem delegate signatures)
    void HandleInviteReceived(const FUniqueNetId& UserId,
        const FUniqueNetId& FromId,
        const FString& AppId,
        const FOnlineSessionSearchResult& InviteResult);

    void HandleInviteAccepted(bool bWasSuccessful,
        int32 LocalUserNum,
        TSharedPtr<const FUniqueNetId> UserId,
        const FOnlineSessionSearchResult& InviteResult);

    void HandleJoinSessionComplete(FName SessionName,
        EOnJoinSessionCompleteResult::Type Result);
};
