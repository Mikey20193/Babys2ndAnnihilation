#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/OnlineSessionInterface.h"      // EOnJoinSessionCompleteResult, FUniqueNetId
#include "BlueprintDataDefinitions.h"              // FBlueprintSessionResult
#include "OnlineSessionSettings.h"                 // FOnlineSessionSearchResult

#include "UInviteListenerComponent.generated.h"    // MUST match filename exactly

// Renamed delegates to avoid redefinition with old UObject version
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInviteListenerComp_OnInviteReceived, FBlueprintSessionResult, SessionResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInviteListenerComp_OnInviteAccepted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInviteListenerComp_OnJoinSessionComplete);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BITWINDY_API UInviteListenerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInviteListenerComponent();

    UFUNCTION(BlueprintCallable)
    void InitializeListener();

    // Blueprint events
    UPROPERTY(BlueprintAssignable)
    FInviteListenerComp_OnInviteReceived OnInviteReceived;

    UPROPERTY(BlueprintAssignable)
    FInviteListenerComp_OnInviteAccepted OnInviteAccepted;

    UPROPERTY(BlueprintAssignable)
    FInviteListenerComp_OnJoinSessionComplete OnJoinSessionComplete;

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
