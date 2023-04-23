// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;





	//1. ���̷�Ż�޽� �����͸� �ҷ����� �ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/TPS_CONTENTPACK/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));


	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//2. Mesh ������Ʈ�� ��ġ�� ȸ�� ���� �����ϰ� �ʹ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	//3. TPS ī�޶� ���̰� �ʹ�.
	// 
	//3-1. SpringArm ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = false;



	//3-2. Camera ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;




	/*--------------------------------------------------------------------------------------------*/





	//4. �� ���̷�Ż�޽� ������Ʈ ���
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	//4-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	gunMeshComp->SetupAttachment(GetMesh());
	//4-2. ���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Weapon/shiroko_gun.shiroko_gun'"));

	//4-3 �����ͷε� ���� ��?
	if (TempGunMesh.Succeeded())
	{
		//4-4 ���̷�Ż �޽� ������ �Ҵ�
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		//4-5. ��ġ �����ϱ�
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}



	/*--------------------------------------------------------------------------------------------*/



	// 5. �������۰� ������Ʈ ���
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));



	// 5-1 �θ� ������Ʈ�� Mesh ������Ʈ�� ����.
	sniperGunComp->SetupAttachment(GetMesh());



	// 5-2 ����ƽ�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));




	// 5-3 �����ͷε尡 �����ߴٸ�
	if (TempGunMesh.Succeeded())
	{
		// 5-4 ����ƽ�޽� ������ �Ҵ�
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);

		// 5-5 ��ġ �����ϱ�
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));

		// 5-6 ũ�� �����ϱ�
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}



	/*--------------------------------------------------------------------------------------------*/




} // �⺻���� ������.... ��  // ������. Ŭ������ �����Ǵ� ���ÿ� �� �ڵ带 �����մϴ�.

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. �������� UI ���� �ν��Ͻ��� �����մϴ�.

	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	// 2. �Ϲ� ���� UI ũ�ν���� �ν��Ͻ��� �����մϴ�.
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	// 3. �Ϲ� ���� UI ���
	_crosshairUI->AddToViewport();

	// �⺻���� �������۰��� ����ϵ��� ����
	ChangeToSniperGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
	
}



/*ĳ���� ������ �� �Է� ����*/




// ���𰡸� �Է����� �� �� �Է��� �ش� �Լ��� �����Ű���� ���ε����ݴϴ�.
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//ĳ���� �� ī�޶� ȸ�� ���ε�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	//�����¿� ĳ���� ������ ���ε�

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);

	//���� �Է� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);


	// �Ѿ� �߻� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	// �� ��ü �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);

	// �������� ���� ��� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);
	
}

void ATPSPlayer::InputFire()
{
	if (bUsingGrenadeGun)
	{
		//�Ѿ� �߻� ó��
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
		UE_LOG(LogTemp, Display, TEXT("InputFire function is executed."));
	}
	else // �������۰��� ������϶�
	{
		// LineTrace ������ġ
		FVector startPos = tpsCamComp->GetComponentLocation();

		// ���� ��ġ (�Ѿ��� �ε����� ����)
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000; //���⼭ * 5000�� 5000cm�� ����. �� ��Ÿ���

		//LineTrace�� �浹 ������ ���� ���� ����
		FHitResult hitinfo;

		//�浹 �ɼ� ���� ����
		FCollisionQueryParams params;

		//�÷��̾� �ڱ� �ڽ��� �浹���� ���ܵ�!
		params.AddIgnoredActor(this);

		// Channel ���͸� �̿��� LineTrace �浹 ����(�浹 ����, ������ġ , ������ġ, ���� ä��, �浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitinfo, startPos, endPos, ECC_Visibility, params);

		

		if (bHit)
		{
			//����׿�, ����Ʈ���̽��� ��� ������ �׷���
			//DrawDebugLine(GetWorld(), startPos, hitinfo.ImpactPoint, FColor::Green, false, 2.0f, 0, 2.0f);

			// �浹 ó�� -> �Ѿ� ���� ȿ�� ���
			FTransform bulletTrans;

			//�ε��� ��ġ �Ҵ�
			bulletTrans.SetLocation(hitinfo.ImpactPoint);

			//�Ѿ� ���� ȿ�� �ν��Ͻ� ����
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			//������ ����� ��ü�� ���������� �ڵ�
			auto hitComp = hitinfo.GetComponent();
			//1. ���� ������Ʈ�� ������ ����Ǿ� �ִٸ�
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				//2. �������� ���� ������ �ʿ���
				FVector force = -hitinfo.ImpactNormal * hitComp->GetMass() * 50000;

				//3. �� �������� �������� ���̴�.
				hitComp->AddForce(force);
			}
		}
	}

}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputHorizontal(float value)
{
	direction.Y = value;
	
}

void ATPSPlayer::InputVertical(float value)
{	
	direction.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::Move()
{	
	// �÷��̾� �̵� ó��
	// ��ӿ ����
	// P(��� ��ġ) = P0(������ġ) + v(�ӵ�) + t(�ð�)

	direction = FTransform(GetControlRotation()).TransformVector(direction); // �̵��ϴ� ������ �÷��̾ �����ִ� �������� ������.

	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}


// ��ź������ �����ϴ� �Լ�
void  ATPSPlayer::ChangeToGrenadeGun()
{
	//��������� üũ
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}


void  ATPSPlayer::ChangeToSniperGun()
{
	//��������� üũ
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim()
{


	//�������۰� ��尡 �ƴ϶�� ó������ ����.
	if (bUsingGrenadeGun)
	{
		return;
	}


	/*���� �Է�ó���� �ȵǾ��� ��� = ���콺�� �ô�*/
	if (bSniperAim == false)
	{
		// ���� ��� Ȱ��ȭ
		bSniperAim = true;

		//UI ���
		_sniperUI->AddToViewport();

		//ī�޶��� �þ߰��� ����
		tpsCamComp->SetFieldOfView(20.0f);

		// �Ϲ� ���� UI�� ������
		_crosshairUI->RemoveFromParent();
	}


	/*���� �Է�ó���� �Ǿ��� ��� = ���콺�� ������*/
	else
	{
		//���ظ�� ��Ȱ��ȭ
		bSniperAim = false;

		//UI ����
		_sniperUI->RemoveFromParent();

		//3. ī�޶� �þ߰� ������� ����
		tpsCamComp->SetFieldOfView(90.0f);

		//4. �Ϲ� ���� UI ���
		_crosshairUI->AddToViewport();
	}
	
	
	/*
	if (!(bSniperAim))
	{
		//���ظ�� ��Ȱ��ȭ
		bSniperAim = false;

		//UI ����
		_sniperUI->RemoveFromParent();

		//3. ī�޶� �þ߰� ������� ����
		tpsCamComp->SetFieldOfView(90.0f);

		return;
	}
	//���� �Է�ó���� �Ǿ��� ��� = ���콺�� ������

	// ���� ��� Ȱ��ȭ
	bSniperAim = true;

	//UI ���
	_sniperUI->AddToViewport();

	//ī�޶��� �þ߰��� ����
	tpsCamComp->SetFieldOfView(20.0f);

	*/
}