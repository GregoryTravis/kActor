class kactor
{
  public var super = nill;

  public fun kactor(_super) {
    super = _super;
  }

  public fun tick(deltaTime) {
    var current_location = super.getActorLocation();

    var elapsed = super.getGameTimeSinceCreation();
    var deltaHeight = sin(elapsed + deltaTime) - sin(elapsed);
    var heightScale = super.getHeightScale();
    current_location.z = current_location.z + deltaHeight * heightScale;

    var current_rotation = super.getActorRotation();
    var rotationSpeed = super.getRotationSpeed();
    current_rotation.yaw = current_rotation.yaw + (deltaTime * rotationSpeed);

    super.setActorLocationAndRotation(current_location, current_rotation);

    return 55.0;
  }

  public fun setLocation(x, y, z) {
    return super.setLocation(x, y, z);
  }
}

return kactor;
