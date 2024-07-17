extends MeshInstance3D


@export var dimension: float = 1


func _ready() -> void:
	## Creating the final surface array
	var surface_array: Array = []
	
	## MANDATORY since Godot except this size
	surface_array.resize(Mesh.ARRAY_MAX)
	
	## Creating intermediate arrays
	var verts = PackedVector3Array()
	var uvs = PackedVector3Array()
	var normals = PackedVector3Array()
	var indices = PackedInt32Array()
	
	
	
	########## ACTUAL SURFACE GENERATION HERE - BEGIN ###########
	
	
	
	## Cube
	# For uv, here is the plane cube :
	#    *---*
	#    |   |
	#*---*---*---*---*
	#|   |   |   |   |
	#*---*---*---*---*
	#    |   |
	#    *---*
	var d := dimension
	
	## Points are added 3 by 3 with identical vertices and uv but
	## normals x, then y and then z
	## So the indexes of each point (i, j, k) in the list is :
	## list n° is :   k + j * 2 + i * 2*2
	## vector id is :  3*(k + j * 2 + i * 2*2) + (normy == 1) + 2*(normz == 1)
	
	for i in range(2):
		for j in range(2):
			for k in range(2):
				var vert := d*Vector3(i-.5, j-.5, k-.5)
				var normal1 := 2*Vector3(i-.5, 0, 0)
				var normal2 := 2*Vector3(0, j-.5, 0)
				var normal3 := 2*Vector3(0, 0, k-.5)
				var uv := Vector3(i, j, k)
				
				verts.append_array([vert, vert, vert])
				normals.append_array([normal1, normal2, normal3])
				uvs.append_array([uv, uv, uv])
	
	
	
	
	var id1 := 0
	var id2 := 0
	var id3 := 0
	var id4 := 0
	
	var i := 0
	var j := 0
	var k := 0
	
	## First point in (0, 0, 0)
	# z normal -> plane (x, y)
	id1 = 3*(k + j*2 + i*4) + 2
	id2 = 3*(k + (j+1)*2 + i*4) + 2
	id3 = 3*(k + j*2 + (i+1)*4) + 2
	id4 = 3*(k + (j+1)*2 + (i+1)*4) + 2
	
	indices.append_array([id1, id3, id4])
	indices.append_array([id1, id4, id2])
	
	# y normal -> plane (x, z)
	id1 = 3*(k + j*2 + i*4) + 1
	id2 = 3*((k+1) + j*2 + i*4) + 1
	id3 = 3*(k + j*2 + (i+1)*4) + 1
	id4 = 3*((k+1) + j*2 + (i+1)*4) + 1
	
	indices.append_array([id1, id4, id3])
	indices.append_array([id1, id2, id4])
	
	# x normal -> plane (y, z)
	id1 = 3*(k + j*2 + i*4) + 0
	id2 = 3*((k+1) + j*2 + i*4) + 0
	id3 = 3*(k + (j+1)*2 + i*4) + 0
	id4 = 3*((k+1) + (j+1)*2 + i*4) + 0
	
	indices.append_array([id1, id3, id4])
	indices.append_array([id1, id4, id2])
	
	
	
	
	## Second point in (1, 1, 1)
	i = 1
	j = 1
	k = 1
	
	# z normal -> plane (x, y)
	id1 = 3*(k + j*2 + i*4) + 2
	id2 = 3*(k + (j-1)*2 + i*4) + 2
	id3 = 3*(k + j*2 + (i-1)*4) + 2
	id4 = 3*(k + (j-1)*2 + (i-1)*4) + 2
	
	indices.append_array([id1, id4, id3])
	indices.append_array([id1, id2, id4])
	
	# y normal -> plane (x, z)
	id1 = 3*(k + j*2 + i*4) + 1
	id2 = 3*((k-1) + j*2 + i*4) + 1
	id3 = 3*(k + j*2 + (i-1)*4) + 1
	id4 = 3*((k-1) + j*2 + (i-1)*4) + 1
	
	indices.append_array([id1, id3, id4])
	indices.append_array([id1, id4, id2])
	
	# x normal -> plane (y, z)
	id1 = 3*(k + j*2 + i*4) + 0
	id2 = 3*((k-1) + j*2 + i*4) + 0
	id3 = 3*(k + (j-1)*2 + i*4) + 0
	id4 = 3*((k-1) + (j-1)*2 + i*4) + 0
	
	indices.append_array([id1, id4, id3])
	indices.append_array([id1, id2, id4])
	
	
	##########  ACTUAL SURFACE GENERATION HERE - END  ###########
	
	
	
	## Adding the arrays to the final surface array
	surface_array[Mesh.ARRAY_VERTEX] = verts
	surface_array[Mesh.ARRAY_TEX_UV] = uvs
	surface_array[Mesh.ARRAY_NORMAL] = normals
	surface_array[Mesh.ARRAY_INDEX] = indices
	
	## Finally creating the surface using triangles
	# No blendshapes, lods, or compression used.
	mesh = ArrayMesh.new()
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_array)
	
	
	var saving: bool = false
	
	if saving:
		# Saves mesh to a .tres file with compression enabled.
		ResourceSaver.save(mesh, "res://custom_mesh.tres", ResourceSaver.FLAG_COMPRESS)
	
