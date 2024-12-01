// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterProjectile.h"

#include "MaterialHLSLTree.h"
#include "ToolContextInterfaces.h"
#include "Characters/SmashCharacter.h"
#include "Components/BoxComponent.h"
#include "Engine\Engine.h"


// Sets default values
ASmashCharacterProjectile::ASmashCharacterProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox"));
	CollisionBox->SetBoxExtent(GetActorScale3D());
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASmashCharacterProjectile::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASmashCharacterProjectile::OnOverlapEnd);
}


// Called when the game starts or when spawned
void ASmashCharacterProjectile::BeginPlay()
{
	Super::BeginPlay();
	CurrentProjectileLifeTime = 0.f;
	ProjectileOrient = Cast<ASmashCharacter>(GetOwner())->GetOrientX();
	
}

// Called every frame
void ASmashCharacterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsMoving)
	{
		CurrentProjectileLifeTime += DeltaTime;
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += FVector::ForwardVector * ProjectileSpeed * DeltaTime * ProjectileOrient;
		SetActorLocation(CurrentLocation);
		
	}
	if(ProjectileLifeTime < CurrentProjectileLifeTime)
		Destroy();
}

void ASmashCharacterProjectile::ShootProjectile()
{
	bIsMoving = true;
}
float ASmashCharacterProjectile::GetProjectileSpeed() const
{
	return ProjectileSpeed;
}

void ASmashCharacterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr) return;
	if(!bIsMoving || OtherActor == GetOwner() || OtherActor == this) return;
	
	UE_LOG(LogTemp, Warning, TEXT("OtherActor=%s"), *OtherActor->GetName());
	Destroy();
	
}

void ASmashCharacterProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


