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





	//1. 스켈레탈메시 데이터를 불러오고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/TPS_CONTENTPACK/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));


	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//2. Mesh 컴포넌트의 위치와 회전 값을 설정하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	//3. TPS 카메라를 붙이고 싶다.
	// 
	//3-1. SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = false;



	//3-2. Camera 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;




	/*--------------------------------------------------------------------------------------------*/





	//4. 총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	//4-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh());
	//4-2. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Weapon/shiroko_gun.shiroko_gun'"));

	//4-3 데이터로드 성공 후?
	if (TempGunMesh.Succeeded())
	{
		//4-4 스켈레탈 메시 데이터 할당
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		//4-5. 위치 조정하기
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}



	/*--------------------------------------------------------------------------------------------*/



	// 5. 스나이퍼건 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));



	// 5-1 부모 컴포넌트를 Mesh 컴포넌트로 설정.
	sniperGunComp->SetupAttachment(GetMesh());



	// 5-2 스태틱메시 데이터 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));




	// 5-3 데이터로드가 성공했다면
	if (TempGunMesh.Succeeded())
	{
		// 5-4 스테틱메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);

		// 5-5 위치 조정하기
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));

		// 5-6 크기 조정하기
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}



	/*--------------------------------------------------------------------------------------------*/




} // 기본적인 생성자.... 이  // 생성자. 클래스가 생성되는 동시에 이 코드를 실행합니다.

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 스나이퍼 UI 위젯 인스턴스를 생성합니다.

	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	// 2. 일반 조준 UI 크로스헤어 인스턴스를 생성합니다.
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	// 3. 일반 조준 UI 등록
	_crosshairUI->AddToViewport();

	// 기본으로 스나이퍼건을 사용하도록 설정
	ChangeToSniperGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
	
}



/*캐릭터 움직임 및 입력 관련*/




// 무언가를 입력했을 때 이 입력이 해당 함수를 실행시키도록 바인딩해줍니다.
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//캐릭터 및 카메라 회전 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	//상하좌우 캐릭터 움직임 바인딩

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);

	//점프 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);


	// 총알 발사 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	// 총 교체 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);

	// 스나이퍼 조준 모드 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);
	
}

void ATPSPlayer::InputFire()
{
	if (bUsingGrenadeGun)
	{
		//총알 발사 처리
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
		UE_LOG(LogTemp, Display, TEXT("InputFire function is executed."));
	}
	else // 스나이퍼건을 사용중일때
	{
		// LineTrace 시작위치
		FVector startPos = tpsCamComp->GetComponentLocation();

		// 종료 위치 (총알이 부딪히는 지점)
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000; //여기서 * 5000은 5000cm을 뜻함. 즉 사거리임

		//LineTrace의 충돌 정보를 담은 변수 선언
		FHitResult hitinfo;

		//충돌 옵션 설정 변수
		FCollisionQueryParams params;

		//플레이어 자기 자신은 충돌에서 제외됨!
		params.AddIgnoredActor(this);

		// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작위치 , 종료위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitinfo, startPos, endPos, ECC_Visibility, params);

		

		if (bHit)
		{
			//디버그용, 라인트레이스를 녹색 선으로 그려줌
			//DrawDebugLine(GetWorld(), startPos, hitinfo.ImpactPoint, FColor::Green, false, 2.0f, 0, 2.0f);

			// 충돌 처리 -> 총알 파편 효과 재생
			FTransform bulletTrans;

			//부딪힌 위치 할당
			bulletTrans.SetLocation(hitinfo.ImpactPoint);

			//총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			//물리가 적용된 물체를 날려버리는 코드
			auto hitComp = hitinfo.GetComponent();
			//1. 만약 컴포넌트에 물리가 적용되어 있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				//2. 날려버릴 힘과 방향이 필요함
				FVector force = -hitinfo.ImpactNormal * hitComp->GetMass() * 50000;

				//3. 그 방향으로 날려버릴 것이다.
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
	// 플레이어 이동 처리
	// 등속운동 공식
	// P(결과 위치) = P0(현재위치) + v(속도) + t(시간)

	direction = FTransform(GetControlRotation()).TransformVector(direction); // 이동하는 방향을 플레이어가 보고있는 방향으로 수정함.

	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}


// 유탄총으로 변경하는 함수
void  ATPSPlayer::ChangeToGrenadeGun()
{
	//사용중으로 체크
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}


void  ATPSPlayer::ChangeToSniperGun()
{
	//사용중으로 체크
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim()
{


	//스나이퍼건 모드가 아니라면 처리하지 않음.
	if (bUsingGrenadeGun)
	{
		return;
	}


	/*만약 입력처리가 안되었을 경우 = 마우스를 뗐다*/
	if (bSniperAim == false)
	{
		// 조준 모드 활성화
		bSniperAim = true;

		//UI 등록
		_sniperUI->AddToViewport();

		//카메라의 시야각을 조정
		tpsCamComp->SetFieldOfView(20.0f);

		// 일반 조준 UI를 제거함
		_crosshairUI->RemoveFromParent();
	}


	/*만약 입력처리가 되었을 경우 = 마우스를 눌렀다*/
	else
	{
		//조준모드 비활성화
		bSniperAim = false;

		//UI 제거
		_sniperUI->RemoveFromParent();

		//3. 카메라 시야각 원래대로 복원
		tpsCamComp->SetFieldOfView(90.0f);

		//4. 일반 조준 UI 등록
		_crosshairUI->AddToViewport();
	}
	
	
	/*
	if (!(bSniperAim))
	{
		//조준모드 비활성화
		bSniperAim = false;

		//UI 제거
		_sniperUI->RemoveFromParent();

		//3. 카메라 시야각 원래대로 복원
		tpsCamComp->SetFieldOfView(90.0f);

		return;
	}
	//만약 입력처리가 되었을 경우 = 마우스를 눌렀다

	// 조준 모드 활성화
	bSniperAim = true;

	//UI 등록
	_sniperUI->AddToViewport();

	//카메라의 시야각을 조정
	tpsCamComp->SetFieldOfView(20.0f);

	*/
}