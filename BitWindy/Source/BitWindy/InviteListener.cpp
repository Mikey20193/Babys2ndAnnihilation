#include "InviteListener.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Delegates/DelegateCombinations.h"
#include "BlueprintDataDefinitions.h" // Needed for FBlueprintSessionResult

void UInviteListener::Initialize()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (!Subsystem) return;

    IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
    if (!Sessions.IsValid()) return;

    // Bind delegates for invite and join events
    InviteReceivedHandle = Sessions->AddOnSessionInviteReceivedDelegate_Handle(
        FOnSessionInviteReceivedDelegate::CreateUObject(this, &UInviteListener::HandleInviteReceived)
    );

    InviteAcceptedHandle = Sessions->AddOnSessionUserInviteAcceptedDelegate_Handle(
        FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UInviteListener::HandleInviteAccepted)
    );

    JoinSessionCompleteHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(
        FOnJoinSessionCompleteDelegate::CreateUObject(this, &UInviteListener::HandleJoinSessionComplete)
    );
}

void UInviteListener::HandleInviteReceived(
    const FUniqueNetId& UserId,
    const FUniqueNetId& FromId,
    const FString& AppId,
    const FOnlineSessionSearchResult& InviteResult)
{
    // Convert raw OnlineSubsystem result into Blueprint-friendly type
    LastInviteResult = FBlueprintSessionResult(InviteResult);

    // Broadcast to Blueprint
    OnInviteReceived.Broadcast(LastInviteResult);
}

void UInviteListener::HandleInviteAccepted(
    bool bWasSuccessful,
    int32 LocalUserNum,
    TSharedPtr<const FUniqueNetId> UserId,
    const FOnlineSessionSearchResult& InviteResult)
{
    OnInviteAccepted.Broadcast();
}

void UInviteListener::HandleJoinSessionComplete(
    FName SessionName,
    EOnJoinSessionCompleteResult::Type Result)
{
    OnJoinSessionComplete.Broadcast();
}

FBlueprintSessionResult UInviteListener::GetLastInviteSession() const
{
    return LastInviteResult;
}
