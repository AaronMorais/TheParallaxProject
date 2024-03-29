# TheParallaxProject

# Alloy
Generates either voxel-based or brick-based solutions based on one of the "lego.als" files and a provided alloy model. Example models are available in the "models" folder under either "voxels" or "bricks".

Results are outputted to "lego.xml" and "lego.json". To view the result, start a simple web server in the alloy directory and open the "index.html" file.

Available Tests:
```
  ant cube
  ant pyramid
  ant lblock
  ant line
  ant teapot
```

To run a particular model:
```
  ant model -Dmodel="models/pyramid.model"
```

Note: All paths are relative to the alloy directory.

# Leogizer
Performs voxelization on .obj files. It can output a obj file with voxels or an alloy voxel model.

Usage:
```
  cd legoizer
  make run
```
