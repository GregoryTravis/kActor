// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingActor.h"
#include "UObject/UObjectGlobals.h"
#include "Misc/Paths.h"

#include <string>
#include "kembed.h"

// Forward decls
sexp GetGameTimeSinceCreation_delegate_sexp_native(sexp arglist);
sexp GetActorRotation_delegate_sexp_native(sexp arglist);
sexp GetActorLocation_delegate_sexp_native(sexp arglist);
sexp SetActorLocationAndRotation_delegate_sexp_native(sexp arglist);
sexp GetHeightScale_delegate_sexp_native(sexp arglist);
sexp FVector_to_fvector(AFloatingActor *kactor, FVector fv);
sexp FRotator_to_frotator(AFloatingActor *kactor, FRotator fr);
FVector fvector_to_FVector(sexp fvector);
FRotator frotator_to_FRotator(sexp frotator);

// Sets up Mesh
// Initialize bridge between C++ and K.
AFloatingActor::AFloatingActor()
{
    // Standard actor setup
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

    // Set up bridge between C++ and K.
    ke_init();

    // Create K superclass
    sexp super_class = ke_exec_file(TCHAR_TO_ANSI(*FindSource("super.k")));
    KESD(super_class);
    sexp kthis = SEXP_MKOBJ(this);

    // Create superclass delegates that allow K to call back to C++
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
    sexp GetHeightScale_delegate_sexp =
    mknative(&GetHeightScale_delegate_sexp_native,
             strdup("GetHeightScale_delegate_sexp_native"));
    sexp super = ke_call_constructor(super_class,
                                     L6(kthis,
                                        GetActorLocation_delegate_sexp,
                                        GetActorRotation_delegate_sexp,
                                        SetActorLocationAndRotation_delegate_sexp,
                                        GetGameTimeSinceCreation_delegate_sexp,
                                        GetHeightScale_delegate_sexp));

    // Set up 'kactor', the K implementation of this class
    sexp clas = ke_exec_file(TCHAR_TO_ANSI(*FindSource("kactor.k")));
    kdelegate = ke_call_constructor(clas, L1(super));
    
    // Set up utility classes
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
    ke_call_method(kdelegate, "tick", cons(SEXP_MKFLOAT(DeltaTime), nill));
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
    bool b = kactor->SetActorLocationAndRotation(fv, fr);
    sexp b_sexp = SEXP_MKINT((int)b);
    return b_sexp;
}

float AFloatingActor::GetHeightScale()
{
    return heightScale;
}

sexp GetHeightScale_delegate_sexp_native(sexp arglist)
{
    A(length(arglist) == 1);

    sexp kactor_sexp = car(arglist);
    AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactor_sexp);
    return SEXP_MKFLOAT(kactor->GetHeightScale());
}

// Utility to convert FVectors between C++ and K representations
sexp FVector_to_fvector(AFloatingActor *kactor, FVector fv)
{
    sexp r = ke_call_constructor(kactor->fvector_class,
                                 L3(SEXP_MKFLOAT(fv.X),
                                    SEXP_MKFLOAT(fv.Y),
                                    SEXP_MKFLOAT(fv.Z)));
    return r;
}

// Utility to convert FVectors between C++ and K representations
sexp FRotator_to_frotator(AFloatingActor *kactor, FRotator fr)
{
    return ke_call_constructor(kactor->frotator_class,
                               L3(SEXP_MKFLOAT(fr.Pitch),
                                  SEXP_MKFLOAT(fr.Roll),
                                  SEXP_MKFLOAT(fr.Yaw)));
}

// Utility to convert FVectors between C++ and K representations
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

// Utility to convert FVectors between C++ and K representations
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

// Get path to K source file
FString AFloatingActor::FindSource(FString filename)
{
    FString p = FPaths::Combine(FPaths::ProjectContentDir(), "k", filename);
    return p;
}
