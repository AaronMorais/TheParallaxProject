abstract sig Brick {
	voxels : set Voxel,
	colour : one Colour
}

sig OneOne, OneTwo, OneThree, OneFour, TwoTwo, LBrick, TwoThree, TwoFour,
	OneOnePlate, OneTwoPlate, OneThreePlate, OneFourPlate, LPlate,
	TwoTwoPlate, TwoThreePlate, TwoFourPlate, ThreeThreePlate extends Brick {}
fun length[] : Brick -> Int {
		OneOne->1 + OneTwo->2 + OneThree->3 + OneFour->4 + TwoTwo->2 + LBrick->2 + TwoThree->2 + TwoFour->2 +
		OneOnePlate->1 + OneTwoPlate->2 + OneThreePlate->3 + OneFourPlate->4 + LPlate->2 +
		TwoTwoPlate->2 + TwoThreePlate->2 + TwoFourPlate->2 + ThreeThreePlate -> 3}
fun width[] : Brick -> Int {
		OneOne->1 + OneTwo->1 + OneThree->1 + OneFour->1 + TwoTwo->2 + LBrick->2 + TwoThree->3 + TwoFour->4 +
		OneOnePlate->1 + OneTwoPlate->1 + OneThreePlate->1 + OneFourPlate->1 + LPlate->2 +
		TwoTwoPlate->2 + TwoThreePlate->3 + TwoFourPlate->4 + ThreeThreePlate -> 3}
fun height[] : Brick -> Int {
		OneOne->3 + OneTwo->3 + OneThree->3 + OneFour->3 + TwoTwo->3 + LBrick->3 + TwoThree->3 + TwoFour->3 +
		OneOnePlate->1 + OneTwoPlate->1 + OneThreePlate->1 + OneFourPlate->1 + LPlate->1 +
		TwoTwoPlate->1 + TwoThreePlate->1 + TwoFourPlate->1 + ThreeThreePlate -> 1}
fun volume[] : Brick -> Int {
		OneOne->3 + OneTwo->6 + OneThree->9 + OneFour->12 + TwoTwo->12 + LBrick->9 + TwoThree->18 + TwoFour->24 +
		OneOnePlate->1 + OneTwoPlate->2 + OneThreePlate->3 + OneFourPlate->4 + LPlate->3 +
		TwoTwoPlate->4 + TwoThreePlate->6 + TwoFourPlate->8 + ThreeThreePlate -> 9}
fun inventory[] : Brick -> Int {
		OneOne->10 + OneTwo->10 + OneThree->10 + OneFour->10 + TwoTwo->10 + LBrick->10 + TwoThree->10 + TwoFour->10 +
		OneOnePlate->10 + OneTwoPlate->10 + OneThreePlate->10 + OneFourPlate->10 + LPlate->10 +
		TwoTwoPlate->10 + TwoThreePlate->10 + TwoFourPlate->10 + ThreeThreePlate -> 10}

-- a set comprehension to tell us which bricks are connected to which others
fun connected[] : Brick->Brick {
	{b1, b2 : Brick | connected[b1,b2] && b1 != b2 }
}
-- the definition of what it means for two bricks to be connected
-- functions and predicates are in different name spaces,
-- so it is ok to have a pred and a fun with the same name
pred connected[b1, b2 : one Brick] {
	some v1 : b1.voxels |
			some v2 : b2.voxels | aboveOrBelow[v1, v2]
}

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
	{v1, v2 : Voxel | xAdjacent[v1,v2] or zAdjacent[v1,v2] or aboveOrBelow[v1,v2]}
}
fun xAdjacent[] : Voxel -> Voxel {
	{v1, v2 : Voxel | xAdjacent[v1,v2]}
}
fun zAdjacent[] : Voxel -> Voxel {
	{v1, v2 : Voxel | zAdjacent[v1,v2] }
}
fun yAdjacent[] : Voxel -> Voxel {
	{v1, v2 : Voxel | aboveOrBelow[v1,v2] }
}

abstract sig Colour {}
one sig Red, Blue, Yellow extends Colour {}

fact WellPlaced {
	-- all Voxels are assigned to some brick
	Brick.voxels = Voxel
	-- Bricks have the proper volume
	all b : Brick | b.volume = #(b.voxels)
	-- Each Voxel is occupied by exactly one Brick
	all v : Voxel | one voxels.v
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
 	-- Each voxel in the brick must be the same colour as the brick
	all b : Brick | all v : b.voxels | v.colour = b.colour
	--	Each brick is only as tall as brick allows
	all b : Brick | all v : b.voxels |
		 #(v.*(b.voxels <: yAdjacent :> b.voxels)) = b.height
	-- There must be at least one TwoTwo brick used
	#(OneOne <: Brick) <= OneOne.inventory
	#(OneTwo <: Brick) <= OneTwo.inventory
	#(OneThree <: Brick) <= OneThree.inventory
	#(OneFour <: Brick) <= OneFour.inventory
	#(TwoTwo <: Brick) <= TwoTwo.inventory
	#(TwoThree <: Brick) <= TwoThree.inventory
	#(LBrick <: Brick) <= LBrick.inventory
	#(TwoFour <: Brick) <= TwoFour.inventory
	#(OneTwoPlate <: Brick) <= OneTwoPlate.inventory
	#(OneOnePlate <: Brick) <= OneOnePlate.inventory
	#(OneThreePlate <: Brick) <= OneThreePlate.inventory
	#(OneFourPlate <: Brick) <= OneFourPlate.inventory
	#(TwoTwoPlate <: Brick) <= TwoTwoPlate.inventory
	#(TwoThreePlate <: Brick) <= TwoThreePlate.inventory
	#(TwoFourPlate <: Brick) <= TwoFourPlate.inventory
	#(LPlate <: Brick) <= LPlate.inventory
	#(ThreeThreePlate <: Brick) <= ThreeThreePlate.inventory
}
pred build {}
run build for 8 but 8 Int