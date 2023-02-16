// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASpell::ASpell()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 10.0f;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mesh->SetCollisionProfileName(TEXT("Hitler2"));
	
	meshExplosionTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshTrailExplosion"));
	meshExplosionTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	meshExplosionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASpell::OnProjectileExplosionImpact);
	meshExplosionTrigger->SetVisibility(false);

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
	meshExplosionTrigger->AttachToComponent(mesh, rules, TEXT("socket"));
	meshExplosionTrigger->SetRelativeScale3D(FVector(6.0f, 6.0f, 6.0f));
	
	mesh->OnComponentBeginOverlap.AddDynamic(this, &ASpell::OnProjectileImpact);

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

void ASpell::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int BodyIndex, bool FromSweep, const FHitResult& Hit)
{
	auto enemy = GetInstigator()->ActorHasTag("Ally") ? "Enemy" : "Ally";
	if (OtherActor->ActorHasTag("Hitable") && OtherActor->ActorHasTag(enemy))
	{
		BetterDestroy();
		UE_LOG(LogTemp, Warning, TEXT("Projectil impacto con %s"), *OtherActor->GetName());
	}
}

void ASpell::OnProjectileExplosionImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int BodyIndex, bool FromSweep,
                                         const FHitResult& Hit)
{
	if (castType == onActor && OtherActor == target)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, damage, Hit.ImpactNormal, Hit, GetInstigator()->Controller, this,
		                                   damageType);
		UE_LOG(LogTemp, Warning, TEXT("Projectil exploto con %s"), *OtherActor->GetName());
	}
	else
	{
		auto enemy = GetInstigator()->ActorHasTag("Ally") ? "Enemy" : "Ally";
		if (OtherActor->ActorHasTag("Hitable") && OtherActor->ActorHasTag(enemy))
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, damage, Hit.ImpactNormal, Hit, GetInstigator()->Controller,
			                                   this, damageType);
			UE_LOG(LogTemp, Warning, TEXT("Projectil exploto con %s"), *OtherActor->GetName());
		}
	}
}
