#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BlueprintDataDefinitions.h"
#include "OnlineSessionSettings.h"

#include "InviteListenerActor.generated.h"

// Renamed delegates to avoid collisions with old UObject version
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInviteActor_OnInviteReceived, FBlueprintSessionResult, SessionResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInviteActor_OnInviteAccepted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInviteActor_OnJoinSessionComplete);

UCLASS(BlueprintType, Blueprintable)
class BITWINDY_API AInviteListenerActor : public AActor
{
    GENERATED_BODY()

public:
    AInviteListenerActor();

    UFUNCTION(BlueprintCallable)
    void InitializeListener();

    // Blueprint events
    UPROPERTY(BlueprintAssignable)
    FInviteActor_OnInviteReceived OnInviteReceived;

    UPROPERTY(BlueprintAssignable)
    FInviteActor_OnInviteAccepted OnInviteAccepted;

    UPROPERTY(BlueprintAssignable)
    FInviteActor_OnJoinSessionComplete OnJoinSessionComplete;

    UFUNCTION(BlueprintCallable)
    FBlueprintSessionResult GetLastInviteSession() const;

protected:
    virtual void BeginPlay() override;

private:
    // Stored session result
    FBlueprintSessionResult LastInviteResult;

    // Delegate handles
    FDelegateHandle InviteReceivedHandle;
    FDelegateHandle InviteAcceptedHandle;
    FDelegateHandle JoinSessionCompleteHandle;

    // Internal handlers
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
