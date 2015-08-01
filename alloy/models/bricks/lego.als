-- a set comprehension to tell us which bricks are connected to which others
fun connected[] : Brick->Brick {
  {b1, b2 : Brick | connected[b1,b2] && b1 != b2 }
}
-- the definition of what it means for two bricks to be connected
-- functions and predicates are in different name spaces,
-- so it is ok to have a pred and a fun with the same name
pred connected[b1, b2 : one Brick] {
  b2.location in b1.location.connectedLocation
    or b1.location in b2.location.connectedLocation
}

fact WellPlaced {
  -- all Voxels are assigned to some brick
  Brick.location.voxels = Voxel
  -- Every brick is connected to some other brick
  (#Brick > 1) => Brick.location in Brick.location.connectedLocation
  -- Every brick is (transitively) connected to every other brick
  all b : Brick | b.*connected = Brick
  -- Ensure no collisions
  all b : Brick | all c : b.location.conflict | c not in Brick.location
  -- Each location is used at most once
  all l : BrickLocation | lone location.l
}
pred build {}
run build for 5 but 3 Int
