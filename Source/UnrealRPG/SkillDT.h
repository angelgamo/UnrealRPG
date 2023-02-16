// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDT.generated.h"

UENUM(BlueprintType)
enum ESkillCastType {
	onLocation,
	onActor,
	onDirection
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct UNREALRPG_API FSkillDT : public FTableRowBase{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString description;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<UTexture2D> image;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ASpell> skillBP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int manaCost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int baseDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ESkillCastType> castType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float maxDistance;

	FSkillDT() : name("") , description(""), image(nullptr), skillBP(nullptr),
		manaCost(0) , baseDamage(0), speed(0.0f), 
		castType(onActor), maxDistance(1000.0f) {}
};

USTRUCT(BlueprintType)
struct UNREALRPG_API FClassDT : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int hp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int mana;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int damage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float speed;
	
	FClassDT() : name(""), hp(0), mana(0) , damage(0), speed(0.0f) {}
};