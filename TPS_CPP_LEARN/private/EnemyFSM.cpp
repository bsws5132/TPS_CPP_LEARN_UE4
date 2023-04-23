// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//월드에서 ATPSPlayer 타깃 찾기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	//ATPSPlayer 타입으로 캐스팅
	target = Cast<ATPSPlayer>(actor);
	// 소유 객체 가져오기
	me = Cast<AEnemy>(GetOwner());
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
	// ...
}

//대기
void UEnemyFSM::IdleState()
{
	// 1. 시간이 흘렀으니
	currentTime += GetWorld()->DeltaTimeSeconds; // 2. 만약 경과 시간이 대기시간을 초과했다 = 설정한 시간이 되었다

	if (currentTime > idleDelayTime)
	{
		//3. 이동상태로 전환 마렵다 ㄱㄱ
		mState = EEnemyState::Move;

		//이후 초기화함.

		currentTime = 0;
	}
}


//이동
void UEnemyFSM::MoveState()
{
	
	// 목표
	FVector destination = target->GetActorLocation();
	// 목표로 향하는 방향
	FVector dir = destination - me->GetActorLocation();
	//그 방향으로 이동하기
	me->AddMovementInput(dir.GetSafeNormal());
	
}


//공격
void UEnemyFSM::AttackState()
{

}

//피격
void UEnemyFSM::DamageState()
{

}

//죽음
void UEnemyFSM::DieState()
{

}