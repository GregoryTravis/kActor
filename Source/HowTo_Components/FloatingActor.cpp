// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingActor.h"
#include "UObject/UObjectGlobals.h"
#include "Misc/Paths.h"

#include <string>
#include "kembed.h"

float AFloatingActor::NuTick(float DeltaTime)
{
  sexp result = ke_call_method(kdelegate, "tick", cons(SEXP_MKFLOAT(DeltaTime), nill));
  float f = SEXP_GET_FLOAT(result);
  KESD(result);
    UE_LOG(LogTemp, Warning, TEXT("AAA NuTick result %f %d %lx %f %lx\n"), DeltaTime, (int)DeltaTime, result, f, SEXP_MKFLOAT(DeltaTime));
  return f;
}

sexp FVector_to_fvector(AFloatingActor *kactor, FVector fv)
{
  sexp r = ke_call_constructor(kactor->fvector_class,
                               L3(SEXP_MKFLOAT(fv.X),
                                  SEXP_MKFLOAT(fv.Y),
                                  SEXP_MKFLOAT(fv.Z)));
  return r;
}

sexp FRotator_to_frotator(AFloatingActor *kactor, FRotator fr)
{
  return ke_call_constructor(kactor->frotator_class,
                             L3(SEXP_MKFLOAT(fr.Pitch),
                                SEXP_MKFLOAT(fr.Roll),
                                SEXP_MKFLOAT(fr.Yaw)));
}

FVector fvector_to_FVector(sexp fvector)
{
  sexp x = ke_get_field(fvector, "x");
  sexp y = ke_get_field(fvector, "y");
  sexp z = ke_get_field(fvector, "z");
  FVector fv;
    fv.X = SEXP_GET_FLOAT(x);
    fv.Y = SEXP_GET_FLOAT(y);
    fv.Z = SEXP_GET_FLOAT(z);
  return fv;
}

FRotator frotator_to_FRotator(sexp frotator)
{
  sexp pitch = ke_get_field(frotator, "pitch");
  sexp roll = ke_get_field(frotator, "roll");
  sexp yaw = ke_get_field(frotator, "yaw");
    FRotator fr;
    fr.Pitch = SEXP_GET_FLOAT(pitch);
    fr.Roll = SEXP_GET_FLOAT(roll);
    fr.Yaw = SEXP_GET_FLOAT(yaw);
  return fr;
}

sexp GetGameTimeSinceCreation_delegate_sexp_native(sexp arglist)
{
    A(length(arglist) == 1);
    sexp kactor_sexp = car(arglist);
    AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactor_sexp);
    float t = kactor->GetGameTimeSinceCreation();
    return SEXP_MKFLOAT(t);
}

sexp GetActorRotation_delegate_sexp_native(sexp arglist)
{
  A(length(arglist) == 1);

  sexp kactor_sexp = car(arglist);
  AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactor_sexp);
  FRotator rotation = kactor->GetActorRotation();
  return FRotator_to_frotator(kactor, rotation);
}

sexp GetActorLocation_delegate_sexp_native(sexp arglist)
{
  A(length(arglist) == 1);

  sexp kactor_sexp = car(arglist);
  AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactor_sexp);
  FVector location = kactor->GetActorLocation();
  return FVector_to_fvector(kactor, location);
}

sexp SetActorLocationAndRotation_delegate_sexp_native(sexp arglist)
{
  A(length(arglist) == 3);

  sexp kactor_sexp = car(arglist);
  sexp location = cadr(arglist);
  sexp rotation = caddr(arglist);
  AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactor_sexp);
  FVector fv = fvector_to_FVector(location);
  FRotator fr = frotator_to_FRotator(rotation);
    UE_LOG(LogTemp, Warning, TEXT("AAA SALAR %f %f %f %f %f %f"), fv.X, fv.Y, fv.Z, fr.Pitch, fr.Roll, fr.Yaw);
  return kactor->SetActorLocationAndRotation(fv, fr);
}

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    this->heightScale = 20.0;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    VisualMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

    UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), ( CubeVisualAsset.Succeeded() ? TEXT("true") : TEXT("false") ));

    if (CubeVisualAsset.Succeeded())
    {
        VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
        VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }

    ke_init();

    sexp super_class = ke_exec_file(TCHAR_TO_ANSI(*FindSource("super.k")));
    KESD(super_class);
    sexp kthis = SEXP_MKOBJ(this);

    sexp GetActorLocation_delegate_sexp =
      mknative(&GetActorLocation_delegate_sexp_native, strdup("GetActorLocation_delegate_sexp_native"));
    sexp GetActorRotation_delegate_sexp =
      mknative(&GetActorRotation_delegate_sexp_native, strdup("GetActorRotation_delegate_sexp_native"));
    sexp SetActorLocationAndRotation_delegate_sexp =
      mknative(&SetActorLocationAndRotation_delegate_sexp_native,
          strdup("SetActorLocationAndRotation_delegate_sexp_native"));
    sexp GetGameTimeSinceCreation_delegate_sexp =
      mknative(&GetGameTimeSinceCreation_delegate_sexp_native,
          strdup("GetGameTimeSinceCreation_delegate_sexp_native"));
    sexp super = ke_call_constructor(super_class,
        L5(kthis,
           GetActorLocation_delegate_sexp,
           GetActorRotation_delegate_sexp,
           SetActorLocationAndRotation_delegate_sexp,
           GetGameTimeSinceCreation_delegate_sexp));

    sexp clas = ke_exec_file(TCHAR_TO_ANSI(*FindSource("kactor.k")));
    kdelegate = ke_call_constructor(clas, L1(super));
    fvector_class = ke_exec_file(TCHAR_TO_ANSI(*FindSource("fvector.k")));
    frotator_class = ke_exec_file(TCHAR_TO_ANSI(*FindSource("frotator.k")));
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
    
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    NuTick(DeltaTime);

}

FString AFloatingActor::FindSource(FString filename)
{
    UE_LOG(LogTemp, Warning, TEXT("FIND SOURCE ecd %s"), *FPaths::EngineContentDir());
    UE_LOG(LogTemp, Warning, TEXT("FIND SOURCE ecd %s"), *FPaths::ProjectContentDir());
    FString p = FPaths::Combine(FPaths::ProjectContentDir(), "k", filename);
    UE_LOG(LogTemp, Warning, TEXT("FIND SOURCE %s %s"), *filename, *p);
    return p;
}
