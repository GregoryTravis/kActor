// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "kembed.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class HOWTO_COMPONENTS_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* VisualMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    float SetLocation(float x, float y, float z);
    
    //UFUNCTION(BlueprintCallable)
    //virtual void SALAR();
    
// private: ?
    UPROPERTY()
    float heightScale;
    
    sexp kdelegate;
};
