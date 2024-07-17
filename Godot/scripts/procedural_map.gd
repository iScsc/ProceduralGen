class_name ProceduralMap extends MeshInstance3D

## --- Signals ---



## --- Static Variables and Constants ---



## --- Default values ---



## --- Static methods ---



## --- Instances ---





## --- Instances Methods ---

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
	
