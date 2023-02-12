// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASpell::ASpell()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 10.0f;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(mesh);
	particleTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleTrail"));
	particleFinal = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleFinal"));
	particleFinal->SetAutoActivate(false);
}

void ASpell::BeginPlay()
{
	spawnLoc = GetActorLocation();
	Super::BeginPlay();
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	particleTrail->AttachToComponent(mesh, rules, TEXT("socket"));
	particleFinal->AttachToComponent(mesh, rules, TEXT("socket"));
}

 void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canMove)
	{
		distTraveled += speed * DeltaTime;
		if (distTraveled >= maxDistance)
			BetterDestroy();
		
		if (castType == onLocation)
			OnLocation(DeltaTime);
		else if (castType == onActor)
			OnActor(DeltaTime);
		else
			OnDirection(DeltaTime);
	}
}

void ASpell::OnLocation(float delta)
{
	OnDirection(delta);
	FVector v1 = GetActorLocation();
	v1.Z = 0.0f;
	if (FVector::Dist(v1, targetLoc) < 10.0f)
		BetterDestroy();
}

void ASpell::OnActor(float delta)
{
	targetDir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	SetActorRotation(targetDir.Rotation());
	OnLocation(delta);
}

void ASpell::OnDirection(float delta)
{
	SetActorLocation(GetActorLocation() + targetDir * speed * delta, true);
}

void ASpell::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& hit)
{
	if(OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, damage, NormalImpulse, hit, GetInstigator()->Controller, this, damageType);
	}
	UE_LOG(LogTemp, Warning, TEXT("Projectil impacto con %s"), *OtherActor->GetName());
}

