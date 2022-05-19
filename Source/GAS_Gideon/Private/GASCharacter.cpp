

#include "GASCharacter.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"

AGASCharacter::AGASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AttributeSetVar = AbilitySystemComponent->GetSet<UGASAttributeSet>();

		const_cast<UGASAttributeSet*>(AttributeSetVar)->HealthChangeDelegate.AddDynamic(this, &AGASCharacter::OnHealthChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->ManaChangeDelegate.AddDynamic(this, &AGASCharacter::OnManaChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->BaseAttackDamagedChangeDelegate.AddDynamic(this, &AGASCharacter::OnBaseAttackDamagedChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->SpeedMultiplierChangeDelegate.AddDynamic(this, &AGASCharacter::OnSpeedMultiplierChangeNative);
		
		//Alternate Method for Attribute Change Delegate
		//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVar->GetHealthAttribute()).AddUObject(this, &AGASCharacter::OnHealthChangedNative);
		//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVar->GetManaAttribute()).AddUObject(this, &AGASCharacter::OnManaChangedNative);
	}
}

void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASCharacter::OnHealthChangedNative(float Health, int32 StackCount)
{
	OnHealthChanged(Health, StackCount);
}

void AGASCharacter::OnManaChangedNative(float Mana, int32 StackCount)
{
	OnManaChanged(Mana, StackCount);
}

void AGASCharacter::OnBaseAttackDamagedChangedNative(float BaseAttackDamage, int32 StackCount)
{
	OnBaseAttackDamagedChanged(BaseAttackDamage, StackCount);
}

void AGASCharacter::OnSpeedMultiplierChangeNative(float SpeedMultiplier, int32 StackCount)
{
	OnSpeedMultierChange(SpeedMultiplier, StackCount);
}

//Function Definition for Alternate Method for Attribute Change Delegates
//void AGASCharacter::OnHealthChangedNative(const FOnAttributeChangeData& Data)
//{
//	OnHealthChanged(Data.OldValue, Data.NewValue);
//}
//
//void AGASCharacter::OnManaChangedNative(const FOnAttributeChangeData& Data)
//{
//	OnManaChanged(Data.OldValue, Data.NewValue);
//}


void AGASCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
	}
}

void AGASCharacter::InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel)
{
	for (const auto AbilityItem : AbilitiesToAcquire)
	{
		InitializeAbility(AbilityItem, AbilityLevel);
	}
}

void AGASCharacter::InitializeAbilityUsingDictionary(TMap<TSubclassOf<UGameplayAbility>, int32> Abilities)
{
	for (const auto Item : Abilities)
	{
		InitializeAbility(Item.Key, Item.Value);
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	
	//InitializeAbilityMulti(InitialAbilitiesList, 0);
	InitializeAbilityUsingDictionary(InitialAbilitiesDict);
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AGASCharacter::GetHealthValues(float& Health, float& MaxHealth) const
{
	if (AttributeSetVar)
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = AttributeSetVar->GetMaxHealth();
	}
}

void AGASCharacter::GetManaValues(float& Mana, float& MaxMana) const
{
	if (AttributeSetVar)
	{
		Mana = AttributeSetVar->GetMana();
		MaxMana = AttributeSetVar->GetMaxMana();
	}
}

void AGASCharacter::GetBaseAttackDamageValue(float& BaseAttackDamage)
{
	if (AttributeSetVar)
	{
		BaseAttackDamage = AttributeSetVar->GetBaseAttackDamage();
	}
}

void AGASCharacter::GetSpeedMultiplierValue(float& SpeedMultiplier)
{
	SpeedMultiplier = AttributeSetVar->GetSpeedMultiplier();
}

void AGASCharacter::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;

	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);

	for (const auto Spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
	}
}

void AGASCharacter::CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithOutTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTags, &WithOutTags, nullptr);
}

void AGASCharacter::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);

	for (const auto Spec : MatchingAbilities)
	{
		Spec->Level = NewLevel;
	}
}

void AGASCharacter::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void AGASCharacter::RemoveLooseGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

