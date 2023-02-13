// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillDT.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Spell.generated.h"

UCLASS()
class UNREALRPG_API ASpell : public AActor
{
	GENERATED_BODY()

public:
	ASpell();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* meshExplosionTrigger;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UNiagaraComponent* particleTrail;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UNiagaraComponent* particleFinal;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector spawnLoc;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	float duration;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	float speed;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	float maxDistance;
	
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	int damage;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	AActor* target;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	FVector targetLoc;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	FVector targetDir;
	
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ESkillCastType> castType;

	bool canMove = false;
	float distTraveled = 0.0f;

	TSubclassOf<UDamageType> damageType;

	
	void SetDamage(int dmg) { damage = dmg;}
	
	UFUNCTION (BlueprintCallable)
	void Initialize(int _dmg, float _durat, float _speed, float _maxDistance, AActor* _target, FVector _targetLoc,
	FRotator _targetDir, TEnumAsByte<ESkillCastType> _castType )
	{
		damage = _dmg;
		SetLifeSpan(_durat);	//duration = _durat;
		speed = _speed;
		maxDistance = _maxDistance;
		target = _target;
		targetLoc = _targetLoc;
		targetLoc.Z = 0.0f;
		SetActorRotation(_targetDir);
		targetDir = GetActorForwardVector();
		castType = _castType;
		spawnLoc = GetActorLocation();
		canMove = true;
	}

	UFUNCTION (BlueprintCallable)
	void BetterDestroy()
	{
		canMove = false;
		
		mesh->SetHiddenInGame(true);
		
		particleTrail->Deactivate();
		particleFinal->Activate();
		
		SetLifeSpan(1.0f);
	}

	void OnLocation(float delta);
	void OnActor(float delta);
	void OnDirection(float delta);

	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit);
	void OnProjectileExplosionImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};