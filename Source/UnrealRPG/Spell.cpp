// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 10.0f;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(mesh);
	particleTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleTrail"));
	particleFinal = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleFinal"));
	particleFinal->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	spawnLoc = GetActorLocation();
	Super::BeginPlay();
	
}

// Called every frame
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
	if (FVector::Dist(v1, targetLoc) < 0.2f)
	{
		BetterDestroy();
		UE_LOG(LogTemp, Warning, TEXT("Location arrived"));
		//No funciona continuar aqui
	}
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

