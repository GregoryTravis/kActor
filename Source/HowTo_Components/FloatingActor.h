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
    
    // Need to be public for C++/K bridge
    
    float GetHeightScale();
    float GetRotationSpeed();

    sexp fvector_class;
    sexp frotator_class;

protected:
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    FString FindSource(FString filename);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // k garbage collection
    void GC();

    // Unpins k objects
    virtual void FinishDestroy() override;

private:

    UPROPERTY(EditAnywhere, Category = Movement)
    float heightScale;

    UPROPERTY(EditAnywhere, Category = Movement)
    float rotationSpeed;

    sexp kdelegate;
    sexp super_class;
    sexp kthis;
    sexp super;
    sexp clas;

    int tick_index;
};
