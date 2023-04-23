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

	//���忡�� ATPSPlayer Ÿ�� ã��
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	//ATPSPlayer Ÿ������ ĳ����
	target = Cast<ATPSPlayer>(actor);
	// ���� ��ü ��������
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

//���
void UEnemyFSM::IdleState()
{
	// 1. �ð��� �귶����
	currentTime += GetWorld()->DeltaTimeSeconds; // 2. ���� ��� �ð��� ���ð��� �ʰ��ߴ� = ������ �ð��� �Ǿ���

	if (currentTime > idleDelayTime)
	{
		//3. �̵����·� ��ȯ ���ƴ� ����
		mState = EEnemyState::Move;

		//���� �ʱ�ȭ��.

		currentTime = 0;
	}
}


//�̵�
void UEnemyFSM::MoveState()
{
	
	// ��ǥ
	FVector destination = target->GetActorLocation();
	// ��ǥ�� ���ϴ� ����
	FVector dir = destination - me->GetActorLocation();
	//�� �������� �̵��ϱ�
	me->AddMovementInput(dir.GetSafeNormal());
	
}


//����
void UEnemyFSM::AttackState()
{

}

//�ǰ�
void UEnemyFSM::DamageState()
{

}

//����
void UEnemyFSM::DieState()
{

}