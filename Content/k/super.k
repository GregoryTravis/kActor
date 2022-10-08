class super
{
  public var kactor = nill;
  public var getActorLocation_delegate = nill;
  public var getActorRotation_delegate = nill;
  public var setActorLocationAndRotation_delegate = nill;
  public var getGameTimeSinceCreation_delegate = nill;
  public var getHeightScale_delegate = nill;
  public var getRotationSpeed_delgate = nill;

  public fun super(_kactor,
                   _getActorLocation_delegate,
                   _getActorRotation_delegate,
                   _setActorLocationAndRotation_delegate,
                   _getGameTimeSinceCreation_delegate,
                   _getHeightScale_delegate,
                   _getRotationSpeed_delgate) {
    kactor = _kactor;
    getActorLocation_delegate = _getActorLocation_delegate;
    getActorRotation_delegate = _getActorRotation_delegate;
    setActorLocationAndRotation_delegate = _setActorLocationAndRotation_delegate;
    getGameTimeSinceCreation_delegate = _getGameTimeSinceCreation_delegate;
    getHeightScale_delegate = _getHeightScale_delegate;
    getRotationSpeed_delgate = _getRotationSpeed_delgate;
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

  public fun getHeightScale() {
    return getHeightScale_delegate(kactor);
  }

  public fun getRotationSpeed() {
    return getRotationSpeed_delgate(kactor);
  }
}

return super;
