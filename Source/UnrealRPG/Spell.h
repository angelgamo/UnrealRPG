// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillDT.h"
#include "Spell.generated.h"

UCLASS()
class UNREALRPG_API ASpell : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpell();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector spawnLoc;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	float duration;
	
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	int damage;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	AActor* target;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	FVector targetLoc;
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	FRotator targetDir;
	
	UPROPERTY (BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ESkillCastType> castType;

	
	void SetDamage(int dmg) { damage = dmg;}
	
	UFUNCTION (BlueprintCallable)
	void Initialize(int _dmg, float _durat, AActor* _target, FVector _targetLoc,
	FRotator _targetDir, TEnumAsByte<ESkillCastType> _castType )
	{
		damage = _dmg;
		duration = _durat;
		target = _target;
		targetLoc = _targetLoc;
		targetDir = _targetDir;
		castType = _castType;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};