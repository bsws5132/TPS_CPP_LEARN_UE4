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

	/*캐릭터 움직임과 입력 처리를 정의하는 곳*/

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
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

	/*총에 관련된 것들을 모두 정의하는 곳*/

	// 총 스켈레탈메시

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//저격총 스테틱메쉬 컴포넌트 정의
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperGunComp;


	//총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;
	
    //총알 피격시 이펙트 공장
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;


	// 총알 발사 처리 함수
	void InputFire();




	//유탄총으로 변경
	void ChangeToGrenadeGun();

	//저격총으로 변경
	void ChangeToSniperGun();




	// 스나이퍼 조준
	void SniperAim();





	// 스나이퍼 조준중인지 여부
	bool bSniperAim = false; 



	// 유탄총 사용 여부
	bool bUsingGrenadeGun = true;


	//저격총 조준 UI
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	//일반 조준 크로스헤어UI 위젯
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// 스나이퍼 UI 위젯 인스턴스
	class UUserWidget* _sniperUI;

	//크로스헤어 인스턴스
	class UUserWidget* _crosshairUI;

};
