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

	/*ĳ���� �����Ӱ� �Է� ó���� �����ϴ� ��*/

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;


	// �¿� ȸ�� �Է� ó��
	void Turn(float value);

	// ���� ȸ�� �Է� ó��
	void LookUp(float value);

	//�ӵ�
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;

	//����
	FVector direction;

	//�¿� �̵� �Է� �̺�Ʈ ó���Լ�
	void InputHorizontal(float value);
	
	//���� �̵� �Է� �̺�Ʈ ó���Լ�
	void InputVertical(float value);

	// ���� �Է� �̺�Ʈ ó�� �Լ�
	void InputJump();

	//�÷��̾��� �̵� ó��
	void Move();




public:

	/*�ѿ� ���õ� �͵��� ��� �����ϴ� ��*/

	// �� ���̷�Ż�޽�

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//������ ����ƽ�޽� ������Ʈ ����
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperGunComp;


	//�Ѿ� ����
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;
	
    //�Ѿ� �ǰݽ� ����Ʈ ����
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;


	// �Ѿ� �߻� ó�� �Լ�
	void InputFire();




	//��ź������ ����
	void ChangeToGrenadeGun();

	//���������� ����
	void ChangeToSniperGun();




	// �������� ����
	void SniperAim();





	// �������� ���������� ����
	bool bSniperAim = false; 



	// ��ź�� ��� ����
	bool bUsingGrenadeGun = true;


	//������ ���� UI
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	//�Ϲ� ���� ũ�ν����UI ����
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// �������� UI ���� �ν��Ͻ�
	class UUserWidget* _sniperUI;

	//ũ�ν���� �ν��Ͻ�
	class UUserWidget* _crosshairUI;

};
