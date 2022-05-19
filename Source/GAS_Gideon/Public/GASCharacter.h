#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GASCharacter.generated.h"

class UGASAbilitySystemComponent;
class UGASAttributeSet;

UCLASS()
class GAS_GIDEON_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GASGameplayAbility")
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GASGameplayAbility")
	const class UGASAttributeSet* AttributeSetVar;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GASGameplayAbility")
	//TArray<TSubclassOf<class UGameplayAbility>> InitialAbilitiesList;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GASGameplayAbility")
	TMap<TSubclassOf<class UGameplayAbility>, int32> InitialAbilitiesDict;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Native
	
	UFUNCTION()
	virtual void OnHealthChangedNative(float	Health, int32 StackCount);

	UFUNCTION()
	virtual void OnManaChangedNative(float Mana, int32 StackCount);

	UFUNCTION()
	virtual void OnBaseAttackDamagedChangedNative(float	BaseAttackDamage, int32 StackCount);

	UFUNCTION()
	virtual void OnSpeedMultiplierChangeNative(float SpeedMultiplier, int32 StackCount);

	// BP

	UFUNCTION(BlueprintImplementableEvent, Category="GASGameplayAbility")
	void OnHealthChanged(float	Health, int32 StackCount);

	UFUNCTION(BlueprintImplementableEvent, Category="GASGameplayAbility")
	void OnManaChanged(float Mana, int32 StackCount);

	UFUNCTION(BlueprintImplementableEvent, Category="GASGameplayAbility")
	void OnBaseAttackDamagedChanged(float	BaseAttackDamage, int32 StackCount);

	UFUNCTION(BlueprintImplementableEvent, Category="GASGameplayAbility")
	void OnSpeedMultierChange(float SpeedMultiplier, int32 StackCount);

	//Alternate Method for Attribute Change Delagates
	////Native functions for Health Change and Mana Change
	//virtual void OnHealthChangedNative(const FOnAttributeChangeData& Data);
	//virtual void OnManaChangedNative(const FOnAttributeChangeData& Data);

	////Called when Health is Changed in BP
	// UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	//void OnHealthChanged(float OldValue, float NewValue);
	//
	//////Called when Mana is Changed in BP
	// UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	//void OnManaChanged(float OldValue, float NewValue);

	// Init Ability

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel);

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void InitializeAbilityUsingDictionary(TMap<TSubclassOf<UGameplayAbility>, int32> Abilities);

	// 
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// ======================================== Getters ========================================

	UFUNCTION(BlueprintPure, Category="GASGameplayAbility")
	void GetHealthValues(float& Health, float& MaxHealth) const;

	UFUNCTION(BlueprintPure, Category="GASGameplayAbility")
	void GetManaValues(float& Mana, float& MaxMana) const;

	UFUNCTION(BlueprintPure, Category="GASGameplayAbility")
	void GetBaseAttackDamageValue(float& BaseAttackDamage);

	UFUNCTION(BlueprintPure, Category="GASGameplayAbility")
	void GetSpeedMultiplierValue(float& SpeedMultiplier);

	// ======================================== Ability Modifier Functions ========================================

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithOutTags);

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel);
	
	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void AddLooseGameplayTag(FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category="GASGameplayAbility")
	void RemoveLooseGameplayTag(FGameplayTag TagToRemove);
	
};
