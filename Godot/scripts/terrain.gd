@tool

class_name Terrain extends StaticBody3D


@export var coll_shape: CollisionShape3D
@export var mesh_instance: MeshInstance3D



## Map dimensions in points in (width, length)
@export var map_dimensions: Vector2 = Vector2(1, 1):
	set(value):
		#print("trying to set dimensions to ", value)
		if value != map_dimensions:
			map_dimensions = Vector2(max(1, value.x), max(1, value.y))
			length = map_dimensions.y
			width = map_dimensions.x
			reset_map()

var length: float = 1:
	set(value):
		#print("trying to set length to ", value)
		if value != length:
			length = max(1, value)
			map_dimensions = Vector2(map_dimensions.x, length)
var width: float = 1:
	set(value):
		#print("trying to set width to ", value)
		if value != width:
			width = max(1, value)
			map_dimensions = Vector2(width, map_dimensions.y)

## Real distance between adjacent points in (width, length) distances
@export var distance_between_points: Vector2 = Vector2(1., 1.)



## Button to generate the mesh in the editor when it goes to True
@export var gen_terrain_bool: bool = false:
	set(value):
		if Engine.is_editor_hint():
			gen_terrain_bool = value
			if gen_terrain_bool:
				generate_dummy_terrain()
				generate_all()
				
				gen_terrain_bool = false

## Button to remove the current mesh in the editor when it goes to True
@export var reset_mesh_bool: bool = false:
	set(value):
		if Engine.is_editor_hint():
			reset_mesh_bool = value
			if reset_mesh_bool:
				reset_all()
				
				reset_mesh_bool = false



## map with dimensions length x width
var map: Array[Array] = [[0]]


func _ready() -> void:
	
	if not mesh_instance is MeshInstance3D:
		var new_mesh := MeshInstance3D.new()
		self.add_child(new_mesh)
		new_mesh.owner = self
		mesh_instance = new_mesh
	
	if not coll_shape is CollisionShape3D:
		var new_coll := CollisionShape3D.new()
		self.add_child(new_coll)
		new_coll.owner = self
		coll_shape = new_coll
	#
	#
	#if not Engine.is_editor_hint():
		#
		#if not mesh_instance.mesh is Mesh:
			#print("Generating a new mesh and collision shape")
			#generate_dummy_terrain()
			##print_map()
			#
			#generate_map_mesh()
			#generate_collision_shape()
	





func reset_map() -> void:
	map = []
	
	for i in range(length):
		var line: Array[float] = []
		
		for j in range(width):
			line.append(0.)
		
		map.append(line)



func generate_dummy_terrain() -> void:
	
	var sinc = \
		(func sinc(x: float) -> float:
			if x == 0:
				return 1.
			else:
				return sin(x)/x)
	
	for i in range(length):
		for j in range(width):
			# centered variables
			var ci := i - length/2
			var cj := j - width/2
			
			map[i][j] = sin((ci*cj/100)**2)
			#sinc.call((25*ci/length*cj/width)**2)
			#sin(ci*cj/100) + 2*(randf() - 1/2)
			#3 * randf() - 1



func print_map() -> void:
	print("Map of size ", map_dimensions, " :")
	for i in range(length):
		print(map[i])




func generate_map_mesh() -> void:
	
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
	
	
	
	## Terrain
	var wd := distance_between_points.x
	var ld := distance_between_points.y
	
	for i in range(length):
		for j in range(width):
			var alt = map[i][j]
			
			# centered variables
			var ci := i - length/2
			var cj := j - width/2
			
			var vert := Vector3(wd*cj, alt, ld*ci)
			var uv := Vector2(i, j)
			
			## Normals are processed once every vert has been correctly created.
			
			verts.append_array([vert, vert])
			uvs.append_array([uv, uv])
			
			var own_id := 2*(i*width + j)
			var left_id := 2*(i*width + (j-1))
			var right_id := 2*(i*width + (j+1))
			var up_id := 2*((i-1)*width + j)
			var down_id := 2*((i+1)*width + j)
			
			## avoid up left issue
			if i > 0 and j > 0:
				#up-wards
				indices.append_array([own_id, left_id, up_id])
				
				#down-wards
				indices.append_array([own_id + 1, up_id + 1, left_id + 1])
			
			## avoid down right issue
			if i < length - 1 and j < width - 1:
				#up-wards
				indices.append_array([own_id, right_id, down_id])
				
				#down-wards
				indices.append_array([own_id + 1, down_id + 1, right_id + 1])
			
			### avoid down left issue
			#if i < length - 1 and j > 0:
				#indices.append_array([own_id, down_id, left_id])
			#
			### avoid up right issue
			#if i > 0 and j < width - 1:
				#indices.append_array([own_id, up_id, right_id])
	
	
	
	
	
	var nb_points_per_triangle := 3 ## IS A CONSTANT, BUT TO EXPLAIN WHY : 3 points per triangle
	var nb_faces_per_triangle := 2  ## IS A CONSTANT, BUT TO EXPLAIN WHY : face up and down for each
	
	normals.resize(len(verts))
	
	# Setting up normals
	for i in range(0, len(indices), nb_points_per_triangle * nb_faces_per_triangle):
		# ids are in the order : point 1 up, point 2 up, point 3 up, point 1 down, point 2 down, point 3 down
		var id1: int = indices[i]
		var id2: int = indices[i+1]
		var id3: int = indices[i+2]
		
		var id4: int = indices[i+3]
		var id5: int = indices[i+4]
		var id6: int = indices[i+5]
		
		var vert1: Vector3 = verts[id1]
		var vert2: Vector3 = verts[id2]
		var vert3: Vector3 = verts[id3]
		
		# Get two vectors from the plane
		var vec1 := vert2 - vert1
		var vec2 := vert3 - vert1
		
		
		var norm_up := vec2.cross(vec1).normalized()
																	#Vector3(randf(), randf(), randf()).normalized()
																	#Vector3(0, 1, 0)
		var norm_down := -norm_up
		
		# To be added for each vert
		normals[id1] = norm_up
		normals[id2] = norm_up
		normals[id3] = norm_up
		
		normals[id4] = norm_down
		normals[id5] = norm_down
		normals[id6] = norm_down
	
	
	## verifications
	#print("Number of vert : ", len(verts), " - Number of uvs : ", len(uvs), " - Number of normals : ", len(normals))
	#print("Number of indices : ", len(indices), " ---> divided by 3 : ", len(indices)/3, " (2 for each face)")
	
	##########  ACTUAL SURFACE GENERATION HERE - END  ###########
	
	
	
	## Adding the arrays to the final surface array
	surface_array[Mesh.ARRAY_VERTEX] = verts
	surface_array[Mesh.ARRAY_TEX_UV] = uvs
	surface_array[Mesh.ARRAY_NORMAL] = normals
	surface_array[Mesh.ARRAY_INDEX] = indices
	
	## Resetting the current mesh
	reset_mesh()
	
	## Finally creating the surface using triangles
	# No blendshapes, lods, or compression used.
	mesh_instance.mesh = ArrayMesh.new()
	mesh_instance.mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_array)
	



func reset_mesh() -> void:
	mesh_instance.mesh = null




func generate_collision_shape() -> void:
	reset_collision_shape()
	
	var new_shape : ConcavePolygonShape3D = mesh_instance.mesh.create_trimesh_shape()
	coll_shape.shape = new_shape
	



func reset_collision_shape() -> void:
	coll_shape.shape = null
	



func generate_all() -> void:
	reset_all()
	
	generate_map_mesh()
	generate_collision_shape()
	



func reset_all() -> void:
	reset_collision_shape()
	reset_mesh()




static func save_mesh(terrain: Terrain) -> void:
	ResourceSaver.save(terrain.mesh_instance.mesh, "res://data/custom_mesh.tres", ResourceSaver.FLAG_COMPRESS)
	




static func load_terrain(path: String) -> Terrain:
	
	var file = FileAccess.open(path, FileAccess.READ)
	
	var lines: Array[String] = []
	
	while file.get_position() < file.get_length():
		lines.append(file.get_line())
	
	file.close()
	
	var width_parts := lines[0].split(" ", false)
	var length_parts :=  lines[1].split(" ", false)
	
	var nb_chunks_width: int = width_parts[0].to_int()
	var nb_length_width: int = length_parts[0].to_int()
	
	var width: int = width_parts[1].to_int()
	var length: int = length_parts[1].to_int()
	
	
	var alt_mult: float = width / nb_chunks_width
	
	
	var new_terrain: Terrain = Terrain.new()
	
	new_terrain.map_dimensions = Vector2(width, length)
	
	for i in range(length):
		var line_parts := lines[2 + i].split(" ", false)
		
		for j in range(width):
			new_terrain.map[i][j] = alt_mult * line_parts[j].to_float()
	
	return new_terrain
	
