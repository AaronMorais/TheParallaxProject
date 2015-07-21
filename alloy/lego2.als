
--2x2x2
abstract sig Voxel {}
one sig A, B, C, D, E, F, G, H extends Voxel {}
fun x[] : Voxel -> Int {
  A -> 0 + B -> 0 + C -> 0+ D->0 + E->1 + F->1 + G->1 + H->1}
fun y[] : Voxel -> Int {
  A -> 0 + B->1 + C-> 0 + D->1 + E->0 + F->1 + G->0 + H->1}
fun z[] : Voxel -> Int {
  A -> 0 + B->0 + C-> 1 + D->1 + E->0 + F->0 + G->1 + H->1}

abstract sig Brick {
  location : one BrickLocation
}

abstract sig BrickLocation {}
one sig OneOnePlateL0, OneOnePlateL1, OneOnePlateL2, OneOnePlateL3, OneOnePlateL4,
  OneOnePlateL5, OneOnePlateL6, OneOnePlateL7, OneTwoPlateL0, OneTwoPlateL1,
  OneTwoPlateL2, OneTwoPlateL3, OneTwoPlateL4, OneTwoPlateL5, OneTwoPlateL6,
  OneTwoPlateL7, TwoTwoPlateL0,TwoTwoPlateL1 extends BrickLocation {}
fun voxels[] : BrickLocation -> set Voxel {
  OneOnePlateL0 -> A +
  OneOnePlateL1 -> B +
  OneOnePlateL2 -> C +
  OneOnePlateL3 -> D +
  OneOnePlateL4 -> E +
  OneOnePlateL5 -> F +
  OneOnePlateL6 -> G +
  OneOnePlateL7 -> H +
  OneTwoPlateL0 -> (A + C) +
  OneTwoPlateL1 -> (E + G) +
  OneTwoPlateL2 -> (A + E) +
  OneTwoPlateL3 -> (C + G) +
  OneTwoPlateL4 -> (B + D) +
  OneTwoPlateL5 -> (F + H) +
  OneTwoPlateL6 -> (B + F) +
  OneTwoPlateL7 -> (D + H) +
  TwoTwoPlateL0 -> (A + C + E + G) +
  TwoTwoPlateL1 -> (B + D + F + H)
}

fun connectedLocation[] : BrickLocation -> BrickLocation {
  OneOnePlateL0 -> (OneOnePlateL1 + OneTwoPlateL4 + OneTwoPlateL6 + TwoTwoPlateL1) +
  OneOnePlateL1 -> (OneOnePlateL0 + OneTwoPlateL0 + OneTwoPlateL2 + TwoTwoPlateL0) +
  OneOnePlateL2 -> (OneOnePlateL3 + OneTwoPlateL4 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneOnePlateL3 -> (OneOnePlateL2 + OneTwoPlateL0 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneOnePlateL4 -> (OneOnePlateL5 + OneTwoPlateL5 + OneTwoPlateL6 + TwoTwoPlateL1) +
  OneOnePlateL5 -> (OneOnePlateL4 + OneTwoPlateL1 + OneTwoPlateL2 + TwoTwoPlateL0) +
  OneOnePlateL6 -> (OneOnePlateL7 + OneTwoPlateL5 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneOnePlateL7 -> (OneOnePlateL6 + OneTwoPlateL1 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneTwoPlateL0 -> (OneOnePlateL1 + OneOnePlateL3 + OneTwoPlateL4 + OneTwoPlateL6 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneTwoPlateL1 -> (OneOnePlateL5 + OneOnePlateL7 + OneTwoPlateL5 + OneTwoPlateL6 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneTwoPlateL2 -> (OneOnePlateL1 + OneOnePlateL5 + OneTwoPlateL6 + OneTwoPlateL4 + OneTwoPlateL5 + TwoTwoPlateL1) +
  OneTwoPlateL3 -> (OneOnePlateL3 + OneOnePlateL7 + OneTwoPlateL7 + OneTwoPlateL4 + OneTwoPlateL5 + TwoTwoPlateL1) +
  OneTwoPlateL4 -> (OneOnePlateL0 + OneOnePlateL2 + OneTwoPlateL0 + OneTwoPlateL2 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneTwoPlateL5 -> (OneOnePlateL4 + OneOnePlateL6 + OneTwoPlateL1 + OneTwoPlateL2 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneTwoPlateL6 -> (OneOnePlateL0 + OneOnePlateL4 + OneTwoPlateL2 + OneTwoPlateL0 + OneTwoPlateL1 + TwoTwoPlateL0) +
  OneTwoPlateL7 -> (OneOnePlateL2 + OneOnePlateL6 + OneTwoPlateL3 + OneTwoPlateL0 + OneTwoPlateL1 + TwoTwoPlateL0) +
  TwoTwoPlateL0 -> (OneOnePlateL1 + OneOnePlateL3 + OneOnePlateL5 + OneOnePlateL7 + OneTwoPlateL4 + OneTwoPlateL5 + OneTwoPlateL6 + OneTwoPlateL7) +
  TwoTwoPlateL1 -> (OneOnePlateL0 + OneOnePlateL2 + OneOnePlateL4 + OneOnePlateL6 + OneTwoPlateL0 + OneTwoPlateL1 + OneTwoPlateL2 + OneTwoPlateL3)
}
fun conflict[] : BrickLocation -> set BrickLocation {
  OneOnePlateL0 -> (OneTwoPlateL0 + OneTwoPlateL2 + TwoTwoPlateL0) +
  OneOnePlateL1 -> (OneTwoPlateL4 + OneTwoPlateL6 + TwoTwoPlateL1) +
  OneOnePlateL2 -> (OneTwoPlateL0 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneOnePlateL3 -> (OneTwoPlateL4 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneOnePlateL4 -> (OneTwoPlateL1 + OneTwoPlateL2 + TwoTwoPlateL0) +
  OneOnePlateL5 -> (OneTwoPlateL5 + OneTwoPlateL6 + TwoTwoPlateL1) +
  OneOnePlateL6 -> (OneTwoPlateL1 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneOnePlateL7 -> (OneTwoPlateL5 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneTwoPlateL0 -> (OneOnePlateL0 + OneOnePlateL2 + OneTwoPlateL2 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneTwoPlateL1 -> (OneOnePlateL4 + OneOnePlateL6 + OneTwoPlateL2 + OneTwoPlateL3 + TwoTwoPlateL0) +
  OneTwoPlateL2 -> (OneOnePlateL0 + OneOnePlateL4 + OneTwoPlateL0 + OneTwoPlateL1 + TwoTwoPlateL0) +
  OneTwoPlateL3 -> (OneOnePlateL2 + OneOnePlateL6 + OneTwoPlateL0 + OneTwoPlateL1 + TwoTwoPlateL0) +
  OneTwoPlateL4 -> (OneOnePlateL1 + OneOnePlateL3 + OneTwoPlateL6 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneTwoPlateL5 -> (OneOnePlateL5 + OneOnePlateL7 + OneTwoPlateL6 + OneTwoPlateL7 + TwoTwoPlateL1) +
  OneTwoPlateL6 -> (OneOnePlateL1 + OneOnePlateL5 + OneTwoPlateL4 + OneTwoPlateL5 + TwoTwoPlateL1) +
  OneTwoPlateL7 -> (OneOnePlateL3 + OneOnePlateL7 + OneTwoPlateL4 + OneTwoPlateL5 + TwoTwoPlateL1) +
  TwoTwoPlateL0 -> (OneOnePlateL0 + OneOnePlateL2 + OneOnePlateL4 + OneOnePlateL6 + OneTwoPlateL0 + OneTwoPlateL1 + OneTwoPlateL2 + OneTwoPlateL3) +
  TwoTwoPlateL1 -> (OneOnePlateL1 + OneOnePlateL3 + OneOnePlateL5 + OneOnePlateL7 + OneTwoPlateL4 + OneTwoPlateL5 + OneTwoPlateL6 + OneTwoPlateL7)
}

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

-- the definition of what it means for two bricks to be connected
-- functions and predicates are in different name spaces,
-- so it is ok to have a pred and a fun with the same name
--pred connected[b1, b2 : one Brick] {
--  some v1 : b1.voxels |
--      some v2 : b2.voxels | aboveOrBelow[v1, v2]
--}
--
--pred above[v1, v2 : one Voxel] {
--  v1.x = v2.x
--  v1.z = v2.z
--  v1.y = next[v2.y]
--}
--pred aboveOrBelow[v1, v2 : one Voxel] {
--  above[v1,v2] or above[v2,v1]
--}


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