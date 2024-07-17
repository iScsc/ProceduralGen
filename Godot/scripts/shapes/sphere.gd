extends MeshInstance3D

@export var rings: int = 100
@export var radial_segments: int = 100
@export var radius: float = .5



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
	
	
	## Sphere
	# Vertex indices.
	var thisrow = 0
	var prevrow = 0
	var point = 0
	
	# Loop over rings.
	for i in range(rings + 1):
		var v = float(i) / rings
		var w = sin(PI * v)
		var y = cos(PI * v)
		
		# Loop over segments in ring.
		for j in range(radial_segments + 1):
			var u = float(j) / radial_segments
			var x = sin(u * PI * 2.0)
			var z = cos(u * PI * 2.0)
			var vert = Vector3(x * radius * w, y * radius, z * radius * w)
			verts.append(vert)
			normals.append(vert.normalized())
			uvs.append(Vector2(u, v))
			point += 1
			
			# Create triangles in ring using indices.
			if i > 0 and j > 0:
				indices.append(prevrow + j - 1)
				indices.append(prevrow + j)
				indices.append(thisrow + j - 1)
				
				indices.append(prevrow + j)
				indices.append(thisrow + j)
				indices.append(thisrow + j - 1)
		
		prevrow = thisrow
		thisrow = point
	
	
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
	
