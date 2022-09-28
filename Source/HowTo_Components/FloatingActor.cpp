// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor.h"
#include "UObject/UObjectGlobals.h"

#include "kinit.h"

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

    // k_init();
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

