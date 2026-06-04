#include "InviteListenerActor.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Delegates/DelegateCombinations.h"

AInviteListenerActor::AInviteListenerActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AInviteListenerActor::BeginPlay()
{
    Super::BeginPlay();
}

void AInviteListenerActor::InitializeListener()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (!Subsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("InviteListenerActor: No OnlineSubsystem found"));
        return;
    }

    IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
    if (!Sessions.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("InviteListenerActor: No SessionInterface found"));
        return;
    }

    // Bind delegates
    InviteReceivedHandle = Sessions->AddOnSessionInviteReceivedDelegate_Handle(
        FOnSessionInviteReceivedDelegate::CreateUObject(this, &AInviteListenerActor::HandleInviteReceived)
    );

    InviteAcceptedHandle = Sessions->AddOnSessionUserInviteAcceptedDelegate_Handle(
        FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &AInviteListenerActor::HandleInviteAccepted)
    );

    JoinSessionCompleteHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(
        FOnJoinSessionCompleteDelegate::CreateUObject(this, &AInviteListenerActor::HandleJoinSessionComplete)
    );

    UE_LOG(LogTemp, Log, TEXT("InviteListenerActor: Delegates bound successfully"));
}

void AInviteListenerActor::HandleInviteReceived(
    const FUniqueNetId& UserId,
    const FUniqueNetId& FromId,
    const FString& AppId,
    const FOnlineSessionSearchResult& InviteResult)
{
    LastInviteResult = FBlueprintSessionResult(InviteResult);
    OnInviteReceived.Broadcast(LastInviteResult);
}

void AInviteListenerActor::HandleInviteAccepted(
    bool bWasSuccessful,
    int32 LocalUserNum,
    TSharedPtr<const FUniqueNetId> UserId,
    const FOnlineSessionSearchResult& InviteResult)
{
    OnInviteAccepted.Broadcast();
}

void AInviteListenerActor::HandleJoinSessionComplete(
    FName SessionName,
    EOnJoinSessionCompleteResult::Type Result)
{
    OnJoinSessionComplete.Broadcast();
}

FBlueprintSessionResult AInviteListenerActor::GetLastInviteSession() const
{
    return LastInviteResult;
}
