// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingActor.h"
#include "UObject/UObjectGlobals.h"

#include <string>
//#include "sexp.h"
#include "kembed.h"

float SetLocation_delegate(AFloatingActor *kactor, float x, float y, float z)
{
  return kactor->SetLocation(x, y, z);
}

sexp SetLocation_delegate_sexp_native(sexp arglist)
{
  sexp kactors, xs, ys, zs;
  printf("HEYHEY\n");
  KESD(arglist);
  kactors = car(arglist);
  xs = cadr(arglist);
  ys = caddr(arglist);
  zs = caddr(cdr(arglist));

  AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactors);
  sexp x = SEXP_GET_INTEGER(xs);
  sexp y = SEXP_GET_INTEGER(ys);
  sexp z = SEXP_GET_INTEGER(zs);
  float n = SetLocation_delegate(kactor, (float)x, (float)y, (float)z);
  sexp ns = SEXP_MKINT((int)n);
  return ns;
}

float AFloatingActor::SetLocation(float x, float y, float z)
{
    UE_LOG(LogTemp, Warning, TEXT("AAA SetLocation %f %f %f\n"), x, y, z);
  return x + y + z;
}

float AFloatingActor::NuTick(float DeltaTime)
{
  sexp result = ke_call_method(kdelegate, "tick", cons(SEXP_MKINT((int)DeltaTime), nill));
  float f = (float)SEXP_GET_INTEGER(result);
  KESD(result);
    UE_LOG(LogTemp, Warning, TEXT("AAA NuTick result %f %d %lx %f\n"), DeltaTime, (int)DeltaTime, result, f);
  return f;
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

    sexp super_class = ke_exec_file("/Users/gmt/proj/kinterp/super.k");
    UE_LOG(LogTemp, Warning, TEXT("AAA SUPER %lx\n"), super_class);
    KESD(super_class);
    sexp kthis = SEXP_MKOBJ(this);
    sexp SetLocation_delegate_sexp =
      mknative(&SetLocation_delegate_sexp_native, strdup("SetLocation_delegate_sexp_native"));
    sexp super = ke_call_constructor(super_class,
        cons(kthis, cons(SetLocation_delegate_sexp, nill)));

    sexp clas = ke_exec_file("/Users/gmt/proj/kinterp/kactor.k");
    UE_LOG(LogTemp, Warning, TEXT("AAA CLAS %lx\n"), clas);
    kdelegate = ke_call_constructor(clas, cons(super, nill));
    //UE_LOG(LogTemp, Warning, TEXT("DELEGATE %lp"), kdelegate);
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
    
}

/*
void AFloatingActor::SALAR()
{
}
 */

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    NuTick(DeltaTime);
    
    FVector NewLocation = GetActorLocation();
    FRotator NewRotation = GetActorRotation();
    float RunningTime = GetGameTimeSinceCreation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * this->heightScale;       //Scale our height by a factor of 20
    float DeltaRotation = DeltaTime * 20.0f;    //Rotate by 20 degrees per second
    NewRotation.Yaw += DeltaRotation;
    //SetActorLocationAndRotation(NewLocation, NewRotation);

    UClass *uc = this->StaticClass();
    
    /*
    // Create a FVector
    {
        UObject* ClassPackage = ANY_PACKAGE;
        UClass* Result = FindObject<UClass>(ClassPackage, TEXT("FVector"));
        if (Result != NULL) {
            UE_LOG(LogTemp, Warning, TEXT("FindObject %s"), *Result->GetName());
        } else {
            UE_LOG(LogTemp, Warning, TEXT("FindObject nope"));
        }
    }
     */

//#if 0
    for (TFieldIterator<UFunction> It(uc); It; ++It) {
         //SomeArray->Add(*It)
        UFunction* Property = *It;
        FString name = Property->GetName();
        if (name == FString(TEXT("K2_SetActorLocationAndRotation"))) {
            UE_LOG(LogTemp, Warning, TEXT("HEYO UF %s"), *name);
            uint8* Buffer = (uint8*)FMemory_Alloca(Property->ParmsSize);
            FMemory::Memzero(Buffer, Property->ParmsSize);
            for ( TFieldIterator<FProperty> It2(Property); It2 && (It2->PropertyFlags&(CPF_Parm|CPF_ReturnParm)) == CPF_Parm; ++It2 )
            {
                FProperty* ParamProperty = *It2;
                FString paramName = ParamProperty->GetName();
                FString paramType = ParamProperty->GetCPPType();
                UE_LOG(LogTemp, Warning, TEXT("HEYO UF ARG %s %s"), *paramName, *paramType);
                if (paramName == FString(TEXT("NewLocation"))) {
                    UE_LOG(LogTemp, Warning, TEXT("param0 %p"), ParamProperty->ContainerPtrToValuePtr<FVector>(Buffer));
                    *ParamProperty->ContainerPtrToValuePtr<FVector>(Buffer) = NewLocation;
                } else if (paramName == FString(TEXT("NewRotation"))) {
                    UE_LOG(LogTemp, Warning, TEXT("param1 %p"), ParamProperty->ContainerPtrToValuePtr<FRotator>(Buffer));
                    *ParamProperty->ContainerPtrToValuePtr<FRotator>(Buffer) = NewRotation;
                } else {
                    UE_LOG(LogTemp, Warning, TEXT("HEYO UF ARG HUH?"));
                }
            }
            this->ProcessEvent(Property, Buffer);
        }
    }
//#endif
    
    for (TFieldIterator<FProperty> It(uc); It; ++It) {
         //SomeArray->Add(*It)
        FProperty* Property = *It;
        FString name = Property->GetName();
        // UE_LOG(LogTemp, Warning, TEXT("HEYO FP %s"), *name);
        if (name == FString(TEXT("heightScale"))) {
            UE_LOG(LogTemp, Warning, TEXT("HEYO2 %s"), *name);
            //Property->SetPropertyValue(200.0f);
            if (const FFloatProperty* FloatProperty = CastField<const FFloatProperty>(Property))
            {
                UE_LOG(LogTemp, Warning, TEXT("Cast ok"));
                FFloatProperty *__FloatProperty__ = CastField<FFloatProperty>(Property);
                if (__FloatProperty__) {
                    float* Value = __FloatProperty__->ContainerPtrToValuePtr<float>(this);
                    UE_LOG(LogTemp, Warning, TEXT("was %f %f"), *Value, heightScale);
                    *Value = 200.0;
                    
                    /*
                        float f = (float)__FloatProperty__->GetFloatingPointPropertyValue(this);
                    UE_LOG(LogTemp, Warning, TEXT("was %f %f"), f, heightScale);
                     */
                     
                }
                /*
                float f = FloatProperty->GetPropertyValue(Property);
                UE_LOG(LogTemp, Warning, TEXT("was %f %f"), f, heightScale);
                FloatProperty->SetPropertyValue(Property, 200.0f);
                float f2 = FloatProperty->GetPropertyValue(Property);
                UE_LOG(LogTemp, Warning, TEXT("is %f %f"), f2, heightScale);
                */
                /*
                 if (float* ValuePtr = FloatProperty->ContainerPtrToValuePtr<float>(Item))
                {
                    *ValuePtr = 200.0;
                }
                 */
            }
        }
    }
    //UE_LOG(LogTemp, Warning, TEXT("Some warning message") );
}

