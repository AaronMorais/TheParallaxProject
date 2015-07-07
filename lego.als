abstract sig Brick {}
sig OneOne, OneTwo, OneThree extends Brick {}
fun length[] : Brick -> Int {
	OneOne->1 + OneTwo->2 + OneThree->3 }
fun width[] : Brick -> Int {
	OneOne->1 + OneTwo->1 + OneThree->1 }
fun volume[] : Brick -> Int {
	OneOne -> 1 + OneTwo -> 2 + OneThree -> 3 }

abstract sig Voxel {}
one sig A, B, C, D extends Voxel {}
fun x[] : Voxel -> Int {
	A -> -1 + B -> 0 + C -> 1 + D->0 }
fun y[] : Voxel -> Int {
	A -> 0 + B->0 + C-> 0 + D->1 }

one sig Model {
	bricks : set Brick -> set Voxel
}

fact WellPlaced {
	-- all bricks in the Alloy universe are in our Model
	Model.(bricks.Voxel) = Brick
	-- all Voxels are assigned to some brick
	Brick.(Model.bricks) = Voxel
	#Brick.(Model.bricks) = #Voxel
	-- Bricks are contiguous
	-- Bricks have the proper volume
	--#((Model.bricks).Voxel).volume = 3
	all brick : (Model.bricks).Voxel | (brick.volume = #(brick.(Model.bricks)))
--  Bricks are connected
}



pred build {
	
}

run build 
