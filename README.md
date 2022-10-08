kActor: Interpreted scripting language integration for Unreal Engine
======

## Overview

kActor is a proof-of-concept integration between Unreal Engine and a [dynamically typed scripting langauge called k](https://github.com/GregoryTravis/k). It includes:

* Boilerplate bridge code for specific Actor methods (Tick(), etc.)
* Two-way bridge code (C++ -> k, k -> C++)
* Integration with the Unreal garbage bollector **\[in progress\]**
* C++ <-> k data structure conversion
* Superclass bindings

## Demo

<p align="center">
  <img src="https://raw.githubusercontent.com/GregoryTravis/kActor/master/screenshots/kActor.gif">
</p>

The demo project is based on a simple Unreal tutorial. The object movement was rewritten in k:

Movement logic in C++:

```
void AFloatingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector NewLocation = GetActorLocation();
    FRotator NewRotation = GetActorRotation();
    float RunningTime = GetGameTimeSinceCreation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * this->heightScale;
    float DeltaRotation = DeltaTime * 20.0f;
    NewRotation.Yaw += DeltaRotation;
    SetActorLocationAndRotation(NewLocation, NewRotation);
}
```

Movement logic in k:

```
public fun tick(deltaTime)
{
  var current_location = super.getActorLocation();

  var elapsed = super.getGameTimeSinceCreation();
  var deltaHeight = sin(elapsed + deltaTime) - sin(elapsed);
  var heightScale = super.getHeightScale();
  current_location.z = current_location.z + deltaHeight * heightScale;

  var current_rotation = super.getActorRotation();
  var rotationSpeed = super.getRotationSpeed();
  current_rotation.yaw = current_rotation.yaw + (deltaTime * rotationSpeed);

  super.setActorLocationAndRotation(current_location, current_rotation);
}
```

## C++ -> k bridge

Calling k from C++ is relatively simple. The following boilerplate was written by hand but should be generated for every overrideable method:

```
void AFloatingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ke_call_method(kdelegate, "tick", cons(SEXP_MKFLOAT(DeltaTime), nill));
}
```

## k -> C++ bridge

Calling back to the engine from k is a little more complicated. An Actor written in K has a superclass explicitly passed to it in its constructor:

```
public fun kactor(_super) {
  super = _super;
}
```

This superclass, in turn, contains wrapped delegate native C++ functions:

```
public var kactor = nill;
public var getActorLocation_delegate = nill;
public var getActorRotation_delegate = nill;

public fun super(_kactor,
                 _getActorLocation_delegate,
                 _getActorRotation_delegate) {
  kactor = _kactor;
  getActorLocation_delegate = _getActorLocation_delegate;
  getActorRotation_delegate = _getActorRotation_delegate;
}
```

These can be accessed from the k Actor code:

```
var current_rotation = super.getActorRotation();
var rotationSpeed = super.getRotationSpeed();
current_rotation.yaw = current_rotation.yaw + (deltaTime * rotationSpeed);

super.setActorLocationAndRotation(current_location, current_rotation);
```

On the C++ side, the delegates are top-level functions which extract the wrapped Actor object and call its method:

```
sexp GetGameTimeSinceCreation_delegate_sexp_native(sexp arglist)
{
    A(length(arglist) == 1);
    sexp kactor_sexp = car(arglist);
    // Extract wrapped Actor object
    AFloatingActor *kactor = (AFloatingActor*)SEXP_GET_OBJ(kactor_sexp);
    // Call its method
    float t = kactor->GetGameTimeSinceCreation();
    return SEXP_MKFLOAT(t);
}
```

This is in turn wrapped in a k native function wrapper to pass to the superclass:

```
sexp GetGameTimeSinceCreation_delegate_sexp =
  mknative(&GetGameTimeSinceCreation_delegate_sexp_native,
           strdup("GetGameTimeSinceCreation_delegate_sexp_native"));

sexp super = ke_call_constructor(super_class,
                                 L2(kthis,
                                    GetGameTimeSinceCreation_delegate_sexp))
```

## Properties

Properties are accessed via the same k->C++ bridge, allowing editable values to be used from k:

<p align="center">
  <img src="https://raw.githubusercontent.com/GregoryTravis/kActor/master/screenshots/properties.png">
</p>

```
var current_rotation = super.getActorRotation();
```

## About k

k is a simple imperative language with a little bit of OO; it's like a minimal Javascript, except it has first-class continuations and call/cc. You can read more about it [here](https://github.com/GregoryTravis/k).

It includes a small [Actor-like test harness](https://github.com/GregoryTravis/k/blob/main/src/KActor.cpp) for developing outside the Unreal engine.
