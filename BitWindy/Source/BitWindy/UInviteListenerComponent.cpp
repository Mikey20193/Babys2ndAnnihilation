#include "UInviteListenerComponent.h"   // MUST be first include

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Delegates/DelegateCombinations.h"

UInviteListenerComponent::UInviteListenerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInviteListenerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInviteListenerComponent::InitializeListener()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (!Subsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("InviteListenerComponent: No OnlineSubsystem found"));
        return;
    }

    IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
    if (!Sessions.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("InviteListenerComponent: No SessionInterface found"));
        return;
    }

    // Bind delegates
    InviteReceivedHandle = Sessions->AddOnSessionInviteReceivedDelegate_Handle(
        FOnSessionInviteReceivedDelegate::CreateUObject(this, &UInviteListenerComponent::HandleInviteReceived)
    );

    InviteAcceptedHandle = Sessions->AddOnSessionUserInviteAcceptedDelegate_Handle(
        FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UInviteListenerComponent::HandleInviteAccepted)
    );

    JoinSessionCompleteHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(
        FOnJoinSessionCompleteDelegate::CreateUObject(this, &UInviteListenerComponent::HandleJoinSessionComplete)
    );

    UE_LOG(LogTemp, Log, TEXT("InviteListenerComponent: Delegates bound successfully"));
}

void UInviteListenerComponent::HandleInviteReceived(
    const FUniqueNetId& UserId,
    const FUniqueNetId& FromId,
    const FString& AppId,
    const FOnlineSessionSearchResult& InviteResult)
{
    LastInviteResult = FBlueprintSessionResult(InviteResult);
    OnInviteReceived.Broadcast(LastInviteResult);
}

void UInviteListenerComponent::HandleInviteAccepted(
    bool bWasSuccessful,
    int32 LocalUserNum,
    TSharedPtr<const FUniqueNetId> UserId,
    const FOnlineSessionSearchResult& InviteResult)
{
    OnInviteAccepted.Broadcast();
}

void UInviteListenerComponent::HandleJoinSessionComplete(
    FName SessionName,
    EOnJoinSessionCompleteResult::Type Result)
{
    OnJoinSessionComplete.Broadcast();
}

FBlueprintSessionResult UInviteListenerComponent::GetLastInviteSession() const
{
    return LastInviteResult;
}
