extends MeshInstance3D


@export var dimension: float = 1


func _ready() -> void:
	## Creating the final surface array
	var surface_array: Array = []
	
	## MANDATORY since Godot except this size
	surface_array.resize(Mesh.ARRAY_MAX)
	
	## Creating intermediate arrays
	var verts = PackedVector3Array()
	var uvs = PackedVector2Array()
	var normals = PackedVector3Array()
	var indices = PackedInt32Array()
	
	
	
	########## ACTUAL SURFACE GENERATION HERE - BEGIN ###########
	
	
	
	## Square
	var d := dimension/2
	
	var vert0 := d*Vector3(-1, -1, 0)
	var uv0 := Vector2(0, 0)
	var norm0 := Vector3(0, 0, 1)
	
	var vert1 := d*Vector3(-1, 1, 0)
	var uv1 := Vector2(0, 1)
	var norm1 := Vector3(0, 0, 1)
	
	var vert2 := d*Vector3(1, -1, 0)
	var uv2 := Vector2(1, 0)
	var norm2 := Vector3(0, 0, 1)
	
	var vert3 := d*Vector3(1, 1, 0)
	var uv3 := Vector2(1, 1)
	var norm3 := Vector3(0, 0, 1)
	
	verts.append_array([vert0, vert1, vert2, vert3])
	verts.append_array([vert0, vert1, vert2, vert3])
	normals.append_array([norm0, norm1, norm2, norm2])
	normals.append_array([-norm0, -norm1, -norm2, -norm2])
	uvs.append_array([uv0, uv1, uv2, uv3])
	uvs.append_array([uv0, uv1, uv2, uv3])
	
	indices.append_array([0, 1, 2])
	
	indices.append_array([2, 1, 3])
	
	indices.append_array([4, 6, 5])
	
	indices.append_array([6, 7, 5])
	
	
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
	
