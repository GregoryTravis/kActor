kActor: Interpreted scripting language integration for Unreal Engine
======

kActor is a proof-of-concept integration between Unreal Engine and a [dynamically typed scripting langauge called k](https://github.com/GregoryTravis/k). It includes:

* Boilerplate bridge code for specific Actor methods (Tick(), etc.)
* Two-way bridge code (C++ -> k, k -> C++)
* Integration with the Unreal garbage bollector **\[in progress\]**
* C++ <-> k data structure conversion
* Superclass bindings


<p align="center">
  <img src="https://raw.githubusercontent.com/GregoryTravis/kActor/master/screenshots/kActor.mp4">
</p>

The demo project is based on a simple Unreal tutorial. The object movement was rewritten in k:

Original C++ movement logic:

```
FVector NewLocation = GetActorLocation();
FRotator NewRotation = GetActorRotation();
float RunningTime = GetGameTimeSinceCreation();
float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
NewLocation.Z += DeltaHeight * this->heightScale;
float DeltaRotation = DeltaTime * 20.0f;
NewRotation.Yaw += DeltaRotation;
SetActorLocationAndRotation(NewLocation, NewRotation);
```

“converted a tutorial demo game to”

screenshot
c++/k code
hand-written boilerplate to be generated
class
superclass

compare c++ and k for the rotation logic
the KActor test framework
“classic blah blah rep for int/float”
point to unreal branch in k
