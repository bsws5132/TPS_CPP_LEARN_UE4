// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_CPP_LEARN_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	// 전부 순서대로 대기, 이동, 공격, 피격, 죽음

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	//대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2;
	//경과시간
	float currentTime = 0;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ATPSPlayer* target;

	//소유 액터
	UPROPERTY()
	class AEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;

};
