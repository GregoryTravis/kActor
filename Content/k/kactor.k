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
    current_location.z = current_location.z + deltaHeight * 20.0;

    var current_rotation = super.getActorRotation();
    current_rotation.yaw = current_rotation.yaw + (deltaTime * 20.0);

    super.setActorLocationAndRotation(current_location, current_rotation);

    return 55.0;
  }

  public fun setLocation(x, y, z) {
    return super.setLocation(x, y, z);
  }
}

return kactor;
