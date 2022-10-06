class kactor
{
  public var super = nill;

  public fun kactor(_super) {
    super = _super;
  }

  public fun tick(deltaTime) {
    var current_location = super.getActorLocation();
    show "CURRENT LOCATION\n";
    show current_location;
    show current_location.x;
    show current_location.y;
    show current_location.z;

    var elapsed = super.getGameTimeSinceCreation();
    show "ELAPSED\n";
    show elapsed;
    var deltaHeight = sin(elapsed + deltaTime) - sin(elapsed);
    current_location.z = current_location.z + deltaHeight * 20.0;

    show current_location.z;
    show "CURRENT LOCATION done\n";

    var current_rotation = super.getActorRotation();
    show "CURRENT rotation\n";
    show current_rotation;
    show current_rotation.roll;
    show current_rotation.pitch;
    show current_rotation.yaw;
    current_rotation.yaw = current_rotation.yaw + (deltaTime * 20.0);
    show current_rotation.yaw;
    show "CURRENT rotation done\n";

    super.setActorLocationAndRotation(current_location, current_rotation);

    return 55.0;
  }

  public fun setLocation(x, y, z) {
    return super.setLocation(x, y, z);
  }
}

return kactor;
