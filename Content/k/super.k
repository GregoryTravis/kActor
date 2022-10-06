class super
{
  public var kactor = nill;
  public var getActorLocation_delegate = nill;
  public var getActorRotation_delegate = nill;
  public var setActorLocationAndRotation_delegate = nill;
  public var getGameTimeSinceCreation_delegate = nill;

  public fun super(_kactor,
                   _getActorLocation_delegate,
                   _getActorRotation_delegate,
                   _setActorLocationAndRotation_delegate,
                   _getGameTimeSinceCreation_delegate) {
    kactor = _kactor;
    getActorLocation_delegate = _getActorLocation_delegate;
    getActorRotation_delegate = _getActorRotation_delegate;
    setActorLocationAndRotation_delegate = _setActorLocationAndRotation_delegate;
    getGameTimeSinceCreation_delegate = _getGameTimeSinceCreation_delegate;
  }

  public fun getActorLocation() {
    return getActorLocation_delegate(kactor);
  }

  public fun getActorRotation() {
    return getActorRotation_delegate(kactor);
  }

  public fun setActorLocationAndRotation(location, rotation) {
    return setActorLocationAndRotation_delegate(kactor, location, rotation);
  }

  public fun getGameTimeSinceCreation() {
    return getGameTimeSinceCreation_delegate(kactor);
  }
}

return super;
