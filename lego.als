abstract sig Brick {
	voxels : set Voxel,
	colour : one Colour
}
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
sig OneOne, OneTwo, OneThree, OneFour, TwoTwo, LBlock, TwoThree, TwoFour,
	OneOnePlate, OneTwoPlate, OneThreePlate, OneFourPlate, LPlate,
	TwoTwoPlate, TwoThreePlate, TwoFourPlate, ThreeThreePlate extends Brick {}
fun length[] : Brick -> Int {
	OneOne->1 + OneTwo->2 + OneThree->3 + OneFour->4 +
		TwoTwo->2 + LBlock->2 + TwoThree->2 + TwoFour->2 +
		OneOnePlate->1 + OneTwoPlate->2 + OneThreePlate->3 + OneFourPlate->4 +
		LPlate->2 + TwoTwoPlate->2 + TwoThreePlate->2 + TwoFourPlate->2 +
		ThreeThreePlate -> 3}
fun width[] : Brick -> Int {
	OneOne->1 + OneTwo->1 + OneThree->1 + OneFour->1 +
		TwoTwo->2 + LBlock->2 + TwoThree->3 + TwoFour->4 +
		OneOnePlate->1 + OneTwoPlate->1 + OneThreePlate->1 + OneFourPlate->1 +
		LPlate->2 + TwoTwoPlate->2 + TwoThreePlate->3 + TwoFourPlate->4 +
		ThreeThreePlate -> 3}
fun height[] : Brick -> Int {
	OneOne->3 + OneTwo->3 + OneThree->3 + OneFour->3 +
		TwoTwo->3 + LBlock->3 + TwoThree->3 + TwoFour->3 +
		OneOnePlate->1 + OneTwoPlate->1 + OneThreePlate->1 + OneFourPlate->1 +
		LPlate->1 + TwoTwoPlate->1 + TwoThreePlate->1 + TwoFourPlate->1 +
		ThreeThreePlate -> 1}
fun volume[] : Brick -> Int {
	OneOne->3 + OneTwo->6 + OneThree->9 + OneFour->12 +
		TwoTwo->12 + LBlock->9 + TwoThree->18 + TwoFour->24 +
		OneOnePlate->1 + OneTwoPlate->2 + OneThreePlate->3 + OneFourPlate->4 +
		LPlate->3 + TwoTwoPlate->4 + TwoThreePlate->6 + TwoFourPlate->8 +
		ThreeThreePlate -> 9}
fun inventory[] : Brick -> Int {
	OneOne->1 + OneTwo->1 + OneThree->1 + OneFour->1 +
		TwoTwo->1 + LBlock->1 + TwoThree->1 + TwoFour->1 +
		OneOnePlate->5 + OneTwoPlate->4 + OneThreePlate->0 + OneFourPlate->4 +
		LPlate->0 + TwoTwoPlate->0 + TwoThreePlate->1 + TwoFourPlate->8 +
		ThreeThreePlate -> 1}


abstract sig Voxel {}
--1x1x3
--one sig A, B, C extends Voxel {}
--fun x[] : Voxel -> Int {
--	A -> 0 + B -> 0 + C -> 0}
--fun y[] : Voxel -> Int {
--	A -> -1 + B->0 + C-> 1}
--fun z[] : Voxel -> Int {
--	A -> 0 + B->0 + C-> 0}
--fun colour[] : Voxel -> Colour {
--	A -> Red + B->Red + C-> Red}

--2x2x2
--one sig A, B, C, D, E, F, G, H extends Voxel {}
--fun x[] : Voxel -> Int {
--	A -> 0 + B -> 0 + C -> 0+ D->0 + E->1 + F->1 + G->1 + H->1}
--fun y[] : Voxel -> Int {
--	A -> 0 + B->1 + C-> 0 + D->1 + E->0 + F->1 + G->0 + H->1}
--fun z[] : Voxel -> Int {
--	A -> 0 + B->0 + C-> 1 + D->1 + E->0 + F->0 + G->1 + H->1}
--fun colour[] : Voxel -> Colour {
--	A->Red + B->Blue + C->Red + D->Blue + E->Blue + F->Blue + G->Blue + H->Blue}


--L block
--one sig A, B, C, D, E, F, G, H, I extends Voxel {}
--fun x[] : Voxel -> Int {
--	A -> 0 + B -> 0 + C -> 0+ D->0 + E->0 + F->0 + G->1 + H->1 + I->1}
--fun y[] : Voxel -> Int {
--	A -> -1 + B->0 + C-> 1 + D->-1 + E->0 + F->1 + G->-1 + H->0 + I->1}
--fun z[] : Voxel -> Int {
--	A -> 0 + B->0 + C-> 0 + D->1 + E->1 + F->1 + G->0 + H->0 + I->0}
--fun colour[] : Voxel -> Colour {
--	A->Red + B->Red + C->Red + D->Red + E->Red + F->Red + G->Red + H->Red + I->Red}

--Upside down pyramid (top layer 3x3, middle layer cross, bottom layer single brick)
one sig A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P extends Voxel {}
fun x[] : Voxel -> Int {
	A->-1 + B->0 + C->1 + D->-1 + E->0 + F->1 + G->-1 + H->0 + I->1 + J->0 + K->-1 + L->0 + N->1 + M->0 + O->0 + P->0}
fun y[] : Voxel -> Int {
	A->1 + B->1 + C->1 + D->1 + E->1 + F->1 + G->1 + H->1 + I->1 + J->0 + K->0 + L->0 + N->0 + M->0 + O->-1 + P->-2}
fun z[] : Voxel -> Int {
	A->-1 + B->-1 + C->-1 + D->0 + E->0 + F->0 + G->1 + H->1 + I->1 + J->-1 + K->0 + L->0 + N->0 + M->1 + O->0 + P->0}
fun colour[] : Voxel -> Colour {
	A->Red + B->Red + C->Red + D->Red + E->Red + F->Red + G->Red + H->Red + I->Red + J->Red + K->Red + L->Blue + N->Red + M->Red + O->Blue + P->Blue}

abstract sig Colour {}
one sig Red, Blue, Yellow extends Colour {}

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
		 (#(v.*(b.voxels <: xAdjacent :> b.voxels)) <= b.length and
			 #(v.*(b.voxels <: zAdjacent :> b.voxels)) <= b.width) or
 		(#(v.*(b.voxels <: xAdjacent :> b.voxels)) <= b.width and
			 #(v.*(b.voxels <: zAdjacent :> b.voxels)) <= b.length)
 	-- Each voxel in the brick must be the same colour as the brick
	all b : Brick | all v : b.voxels | v.colour = b.colour
	--	Each brick is only as tall as brick allows
	all b : Brick | all v : b.voxels |
		 #(v.*(b.voxels <: yAdjacent :> b.voxels)) <= b.height
	-- There must be at least one TwoTwo brick used
	#(OneOne <: Brick) <= OneOne.inventory
	#(OneTwo <: Brick) <= OneTwo.inventory
	#(OneThree <: Brick) <= OneThree.inventory
	#(OneFour <: Brick) <= OneFour.inventory
	#(TwoTwo <: Brick) <= TwoTwo.inventory
	#(TwoThree <: Brick) <= TwoThree.inventory
	#(LBlock <: Brick) <= LBlock.inventory
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
