// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_CPP_LEARN_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;


	// 좌우 회전 입력 처리
	void Turn(float value);

	// 상하 회전 입력 처리
	void LookUp(float value);

	//속도
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;

	//방향
	FVector direction;

	//좌우 이동 입력 이벤트 처리함수
	void InputHorizontal(float value);
	
	//상하 이동 입력 이벤트 처리함수
	void InputVertical(float value);

	// 점프 입력 이벤트 처리 함수
	void InputJump();

	//플레이어의 이동 처리
	void Move();




public:

	// 총 스켈레탈메시

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	// 총알 발사 처리 함수
	void InputFire();


};
