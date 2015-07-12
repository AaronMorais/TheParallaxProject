abstract sig Brick {
	voxels : set Voxel
}
-- a set comprehension to tell us which bricks are connected to which others
fun connected[] : Brick->Brick {
	{b1, b2 : Brick | connected[b1,b2] }
}
-- the definition of what it means for two bricks to be connected
-- functions and predicates are in different name spaces,
-- so it is ok to have a pred and a fun with the same name
pred connected[b1, b2 : one Brick] {
	some v1 : b1.voxels |
			some v2 : b2.voxels | aboveOrBelow[v1, v2]
}
sig OneOne, OneTwo, OneThree, OneFour, TwoTwo extends Brick {}
fun length[] : Brick -> Int {
	OneOne->1 + OneTwo->2 + OneThree->3 + OneFour->4 + TwoTwo->2}
fun width[] : Brick -> Int {
	OneOne->1 + OneTwo->1 + OneThree->1 + OneFour->1 + TwoTwo->2}
fun volume[] : Brick -> Int {
	OneOne->1 + OneTwo->2 + OneThree->3 + OneFour->4 + TwoTwo->4}

abstract sig Voxel {}
one sig A, B, C, D, E, F, G, H extends Voxel {}
fun x[] : Voxel -> Int {
	A -> 0 + B -> 0 + C -> 0+ D->0 + E->1 + F->1 + G->1 + H->1}
fun y[] : Voxel -> Int {
	A -> 0 + B->1 + C-> 0 + D->1 + E->0 + F->1 + G->0 + H->1}
fun z[] : Voxel -> Int {
	A -> 0 + B->0 + C-> 1 + D->1 + E->0 + F->0 + G->1 + H->1}
pred above[v1, v2 : one Voxel] {
	v1.x = v2.x
	v1.z = v2.z
	v1.y = next[v2.y]
}
pred aboveOrBelow[v1, v2 : one Voxel] {
	above[v1,v2] or above[v2,v1]
}
pred xAdjacent[v1,v2 : one Voxel] {
	v1.y = v2.y
	v1.z = v2.z
	v1.x = next[v2.x] or next[v1.x] = v2.x
}
pred zAdjacent[v1,v2 : one Voxel] {
	v1.y = v2.y
	v1.x = v2.x
	v1.z = next[v2.z] or next[v1.z] = v2.z
}
fun adjacent[] : Voxel -> Voxel {
	{v1, v2 : Voxel | xAdjacent[v1,v2] or zAdjacent[v1,v2] }
}
fun xAdjacent[] : Voxel -> Voxel {
	{v1, v2 : Voxel | xAdjacent[v1,v2]}
}
fun zAdjacent[] : Voxel -> Voxel {
	{v1, v2 : Voxel | zAdjacent[v1,v2] }
}
fact WellPlaced {
	-- all Voxels are assigned to some brick
	Brick.voxels = Voxel
	-- Bricks have the proper volume
	all b : Brick | b.volume = #(b.voxels)
	-- Each Voxel is occupied by exactly one Brick
	all v : Voxel | one voxels.v
	-- Each brick exists on one level
	all b : Brick | all v1, v2 : b.voxels | v1.y = v2.y
	-- Each brick occupies contiguous voxels
	all b : Brick | all v: b.voxels | b.voxels = v.*(b.voxels <: adjacent :> b.voxels)
	-- Every brick is connected to some other brick
	(#Brick > 1) => Brick in Brick.connected
	-- Every brick is (transitively) connected to every other brick
	all b : Brick | b.*connected = Brick
	-- Bricks are only as long and wide as the brick type allows
	all b : Brick | all v : b.voxels |
		 (#(v.*(b.voxels <: xAdjacent :> b.voxels)) = b.length and
			 #(v.*(b.voxels <: zAdjacent :> b.voxels)) = b.width) or
 		(#(v.*(b.voxels <: xAdjacent :> b.voxels)) = b.width and
			 #(v.*(b.voxels <: zAdjacent :> b.voxels)) = b.length)
}
pred build {}
run build for 6 but 6 Int
