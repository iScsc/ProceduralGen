@tool

class_name Terrain extends Node3D


## Constants

const TERRAIN_SCENE = preload("res://scenes/terrain.tscn")

const BASE_LAND_COLOR := Color.FOREST_GREEN

## Variables

# Ground
@export var ground: StaticBody3D
@export var ground_coll_shape: CollisionShape3D
@export var ground_mesh_instance: MeshInstance3D

# Sea
@export var sea: StaticBody3D
@export var sea_coll_shape: CollisionShape3D
@export var sea_mesh_instance: MeshInstance3D

## Map dimensions in points in (width, length)
@export var map_dimensions: Vector2 = Vector2(2, 2):
	set(value):
		#print("trying to set dimensions to ", value)
		if value != map_dimensions:
			map_dimensions = Vector2(max(1, value.x), max(1, value.y))
			length = map_dimensions.y
			width = map_dimensions.x
			
			set_shader_dimensions()
			
			reset_map()
			reset_color_map()

var length: float = 2:
	set(value):
		#print("trying to set length to ", value)
		if value != length:
			length = max(1, value)
			map_dimensions = Vector2(map_dimensions.x, length)
var width: float = 2:
	set(value):
		#print("trying to set width to ", value)
		if value != width:
			width = max(1, value)
			map_dimensions = Vector2(width, map_dimensions.y)

## Real distance between adjacent points in (width, altitude, length) distances
@export var distance_between_points: Vector3 = Vector3(1., .5, 1.):
	set(value):
		if value != distance_between_points:
			distance_between_points = value
			
			set_shader_distance()



@export var sea_level: float = 0.0



## Button to generate the mesh in the editor when it goes to True
@export var gen_terrain_bool: bool = false:
	set(value):
		if Engine.is_editor_hint():
			gen_terrain_bool = value
			if gen_terrain_bool:
				#generate_dummy_terrain()
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
var map: Array[Array] = [[0, 0], [0, 0]]

var color_map: Image = Image.create(2, 2, false, Image.FORMAT_RGBA8)





@export var terrain_shader: Shader = preload("res://shaders/terrain.gdshader")

@export var sea_shader: Shader = preload("res://shaders/sea.gdshader")


func _init() -> void:
	color_map.fill(Color.WHITE)


func _ready() -> void:
	
	if not ground is StaticBody3D:
		var new_ground := StaticBody3D.new()
		self.add_child(new_ground)
		new_ground.owner = self
		ground = new_ground
	
	if not ground_mesh_instance is MeshInstance3D:
		var new_mesh := MeshInstance3D.new()
		ground_mesh_instance.add_child(new_mesh)
		new_mesh.owner = ground_mesh_instance
		ground_mesh_instance = new_mesh
	
	if not ground_coll_shape is CollisionShape3D:
		var new_coll := CollisionShape3D.new()
		ground_coll_shape.add_child(new_coll)
		new_coll.owner = ground_coll_shape
		ground_coll_shape = new_coll
	
	
	
	if not sea is StaticBody3D:
		var new_sea := StaticBody3D.new()
		self.add_child(new_sea)
		new_sea.owner = self
		sea = new_sea
	
	if not sea_mesh_instance is MeshInstance3D:
		var new_mesh := MeshInstance3D.new()
		sea_mesh_instance.add_child(new_mesh)
		new_mesh.owner = sea_mesh_instance
		sea_mesh_instance = new_mesh
	
	if not sea_coll_shape is CollisionShape3D:
		var new_coll := CollisionShape3D.new()
		sea_coll_shape.add_child(new_coll)
		new_coll.owner = sea_coll_shape
		sea_coll_shape = new_coll
	





func reset_map() -> void:
	map = []
	
	for i in range(length):
		var line: Array[float] = []
		
		for j in range(width):
			line.append(0.)
		
		map.append(line)



func reset_color_map() -> void:
	color_map = Image.create(width, length, false, Image.FORMAT_RGBA8)
	color_map.fill(Color.WHITE)
	
	set_shader_cmap()
	



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
	if ground_mesh_instance is MeshInstance3D:
		var new_mesh := ArrayMesh.new()
		generate_ground_surface(new_mesh)
		ground_mesh_instance.mesh = new_mesh
	
	if sea_mesh_instance is MeshInstance3D:
		var new_mesh := ArrayMesh.new()
		generate_sea_surface(new_mesh)
		sea_mesh_instance.mesh = new_mesh




func generate_ground_surface(mesh: ArrayMesh) -> void:
	## Creating the final surface array
	var surface_array: Array = []
	
	## MANDATORY since Godot except this size
	surface_array.resize(Mesh.ARRAY_MAX)
	
	## Creating intermediate arrays
	var verts = PackedVector3Array()
	var uvs = PackedVector2Array()
	var normals = PackedVector3Array()
	var indices = PackedInt32Array()
	
	## Terrain
	var wd := distance_between_points.x
	var ad := distance_between_points.y
	var ld := distance_between_points.z
	
	
	
	for i in range(length):
		for j in range(width):
			var alt = map[i][j]
			# centered variables
			var ci: float = i - (length - 1)/2.
			var cj: float = j - (width - 1)/2.
			
			var vert := Vector3(wd*cj, ad*alt, ld*ci)
			var uv := Vector2(i, j)
			
			# Normals are processed once every vert has been correctly created.
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
	
	
	## Normals Processing
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
	
	## Adding the arrays to the final surface array
	surface_array[Mesh.ARRAY_VERTEX] = verts
	surface_array[Mesh.ARRAY_TEX_UV] = uvs
	surface_array[Mesh.ARRAY_NORMAL] = normals
	surface_array[Mesh.ARRAY_INDEX] = indices
	
	## Finally creating the surface using triangles
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_array)
	mesh.surface_set_name(mesh.get_surface_count() - 1, "Ground")
	
	## Setting up the material
	set_ground_material(mesh)
	



func generate_sea_surface(mesh: ArrayMesh) -> void:
	## BOOLEAN TO DECIDE WHETHER THE SURFACE SHALL BE OPTIMIZED BY TRUNCATURE OR NOT
	var optimize: bool = false ## NEED TO BE ENHANCED -> Should be optimized to generate meshes untill its not necessary anymore.
							   ## something like not generating when every vertex is like : abs(ground_alt - sea_alt) > x
							   ## where x is something like : x = 1.1 * sea_amplitude
	
	
	## Creating the final surface array
	var surface_array: Array = []
	
	## MANDATORY since Godot except this size
	surface_array.resize(Mesh.ARRAY_MAX)
	
	## Creating intermediate arrays
	var verts = PackedVector3Array()
	var uvs = PackedVector2Array()
	var normals = PackedVector3Array()
	var indices = PackedInt32Array()
	
	## Terrain
	var wd := distance_between_points.x
	var ad := distance_between_points.y
	var ld := distance_between_points.z
	
	var alt = sea_level
	
	for i in range(length):
		for j in range(width):
			var alt_cur: float = map[i][j]
			
			var up_left_good: bool = false
			var down_right_good: bool = false
			
			if alt_cur <= alt:
				if i > 0 and j > 0:
					up_left_good = true
				if i < length - 1 and j < width - 1:
					down_right_good = true
			
			## avoid up left issue
			if (not up_left_good) and i > 0 and j > 0:
				## Checking if at least one of the points is under water
				var alt_left: float = map[i][j-1]
				var alt_up: float = map[i-1][j]
				
				if not optimize or alt_left <= alt or alt_up <= alt:
					up_left_good = true
			
			## avoid down right issue
			if (not down_right_good) and i < length - 1 and j < width - 1:
				## Checking if at least one of the points is under water
				var alt_right: float = map[i][j+1]
				var alt_down: float = map[i+1][j]
				
				if not optimize or alt_right <= alt or alt_down <= alt:
					down_right_good = true
			
			
			
			
			
			#if not (up_left_good or down_right_good):
				#continue
			
			
			# centered variables
			var ci: float = i - (length - 1)/2.
			var cj: float = j - (width - 1)/2.
			
			var vert := Vector3(wd*cj, ad*alt, ld*ci)
			var uv := Vector2(i, j)
			
			# Normals are processed once every vert has been correctly created.
			verts.append_array([vert, vert])
			uvs.append_array([uv, uv])
			
			var own_id := 2*(i*width + j)
			var left_id := 2*(i*width + (j-1))
			var right_id := 2*(i*width + (j+1))
			var up_id := 2*((i-1)*width + j)
			var down_id := 2*((i+1)*width + j)
			
			## avoid up left issue
			if up_left_good:                                                    # i > 0 and j > 0:      # up_left_good:
				#up-wards
				indices.append_array([own_id, left_id, up_id])
				
				#down-wards
				indices.append_array([own_id + 1, up_id + 1, left_id + 1])
			
			## avoid down right issue
			if down_right_good:                                                 # i < length - 1 and j < width - 1         # down_right_good:
				#up-wards
				indices.append_array([own_id, right_id, down_id])
				
				#down-wards
				indices.append_array([own_id + 1, down_id + 1, right_id + 1])
	
	
	## Normals Processing
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
		
		var norm_up := vec2.cross(vec1).normalized() # Should be Vector3(0, 1, 0)
		var norm_down := -norm_up    #norm_up
		
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
	
	## Adding the arrays to the final surface array
	surface_array[Mesh.ARRAY_VERTEX] = verts
	surface_array[Mesh.ARRAY_TEX_UV] = uvs
	surface_array[Mesh.ARRAY_NORMAL] = normals
	surface_array[Mesh.ARRAY_INDEX] = indices
	
	## Finally creating the surface using triangles
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_array)
	mesh.surface_set_name(mesh.get_surface_count() - 1, "Water")
	
	## Setting up the material
	set_sea_material(mesh)
	





func reset_meshes() -> void:
	ground_mesh_instance.mesh = null
	sea_mesh_instance.mesh = null
	



func set_ground_material(mesh: Mesh) -> void:
	var new_material: ShaderMaterial = ShaderMaterial.new()
	new_material.shader = terrain_shader
	mesh.surface_set_material(mesh.get_surface_count() - 1, new_material)
	



func set_sea_material(mesh: Mesh) -> void:
	#var new_material: StandardMaterial3D = StandardMaterial3D.new()
	#new_material.albedo_color = Color.NAVY_BLUE
	#new_material.albedo_color.a = .7
	#new_material.transparency = BaseMaterial3D.TRANSPARENCY_ALPHA
	#mesh.surface_set_material(0, new_material)
	
	var new_material: ShaderMaterial = ShaderMaterial.new()
	new_material.shader = sea_shader
	mesh.surface_set_material(mesh.get_surface_count() - 1, new_material)
	
	var c: Color = Color(0.01, 0.03, 0.05) # Color.NAVY_BLUE
	var color_vec := Vector3(c.r, c.g, c.b)
	new_material.set_shader_parameter("base_color", color_vec)
	new_material.set_shader_parameter("alpha", 0.92)
	
	## Setting up the random value map
	var noise_texture: NoiseTexture2D = NoiseTexture2D.new()
	noise_texture.noise = FastNoiseLite.new()
	new_material.set_shader_parameter("noise", noise_texture)
	



func set_shader_dimensions() -> void:
	if ground_mesh_instance is MeshInstance3D and ground_mesh_instance.mesh is Mesh:
		var mat: ShaderMaterial = ground_mesh_instance.mesh.surface_get_material(0)
		mat.set_shader_parameter("map_dist", Vector2(distance_between_points.x, distance_between_points.z))
	
	if sea_mesh_instance is MeshInstance3D and sea_mesh_instance.mesh is Mesh:
		var mat: ShaderMaterial = sea_mesh_instance.mesh.surface_get_material(0)
		mat.set_shader_parameter("map_dist", Vector2(distance_between_points.x, distance_between_points.z))
	


func set_shader_distance() -> void:
	if ground_mesh_instance is MeshInstance3D and ground_mesh_instance.mesh is Mesh:
		var mat: ShaderMaterial = ground_mesh_instance.mesh.surface_get_material(0)
		mat.set_shader_parameter("map_dimensions", map_dimensions)
	
	if sea_mesh_instance is MeshInstance3D and sea_mesh_instance.mesh is Mesh:
		var mat: ShaderMaterial = sea_mesh_instance.mesh.surface_get_material(0)
		mat.set_shader_parameter("map_dimensions", map_dimensions)
	


func set_shader_cmap() -> void:
	if ground_mesh_instance is MeshInstance3D and ground_mesh_instance.mesh is Mesh:
		var mat: ShaderMaterial = ground_mesh_instance.mesh.surface_get_material(0)
		var cmap: ImageTexture = ImageTexture.create_from_image(color_map)
		mat.set_shader_parameter("color_map", cmap)
	
	#if sea_mesh_instance is MeshInstance3D and sea_mesh_instance.mesh is Mesh:
		#var mat: ShaderMaterial = sea_mesh_instance.mesh.surface_get_material(0)
		#var cmap: ImageTexture = ImageTexture.create_from_image(color_map)
		#mat.set_shader_parameter("color_map", cmap)
	



func shader_set_all() -> void:
	set_shader_dimensions()
	set_shader_distance()
	set_shader_cmap()
	



func generate_ground_collision_shape() -> void:
	if ground_mesh_instance is MeshInstance3D:
		var new_shape : ConcavePolygonShape3D = ground_mesh_instance.mesh.create_trimesh_shape()
		ground_coll_shape.shape = new_shape
	



func generate_sea_collision_shape() -> void:
	return
	
	#if sea_mesh_instance is MeshInstance3D:
		#var new_shape : ConcavePolygonShape3D = sea_mesh_instance.mesh.create_trimesh_shape()
		#sea_coll_shape.shape = new_shape
	



func generate_collision_shapes() -> void:
	reset_collision_shapes()
	
	generate_ground_collision_shape()
	generate_sea_collision_shape()
	



func reset_collision_shapes() -> void:
	if ground_coll_shape is CollisionShape3D:
		ground_coll_shape.shape = null
	
	if sea_coll_shape is CollisionShape3D:
		sea_coll_shape.shape = null
	



func generate_all() -> void:
	reset_all()
	
	generate_map_mesh()
	generate_collision_shapes()
	
	shader_set_all()
	



func reset_all() -> void:
	reset_collision_shapes()
	reset_meshes()




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
	
	var slevel: float = lines[2].to_float()
	
	
	var alt_mult: float = width / nb_chunks_width
	
	
	var min_alt: float = INF
	var max_alt: float = -INF
	
	
	var new_terrain: Terrain = TERRAIN_SCENE.instantiate()
	
	new_terrain.map_dimensions = Vector2(width, length)
	new_terrain.sea_level = alt_mult * slevel
	
	## Altitude
	for i in range(length):
		var line_parts := lines[3 + i].split("\t", false)
		
		for j in range(width):
			var alt = line_parts[j].to_float()
			var final_alt = alt_mult * alt
			new_terrain.map[i][j] = final_alt
			
			min_alt = min(final_alt, min_alt)
			max_alt = max(final_alt, max_alt)
	
	
	
	## Colors : Generate color map
	for i in range(length):
		for j in range(width):
			var alt : float = new_terrain.map[i][j]
			
			var c: Color = BASE_LAND_COLOR
			
			if min_alt != max_alt:
				c.g = (alt - min_alt) / (max_alt - min_alt)
			
			new_terrain.color_map.set_pixel(j, i, c)
	
	
	#for i in range(length):
		#var line_parts := lines[3 + length + i].split("\t", false)
		#
		#for j in range(width):
			#var color_parts := line_parts[j].split(",", false)
			#var r: float = color_parts[0].to_float()/255
			#var g: float = color_parts[1].to_float()/255
			#var b: float = color_parts[2].to_float()/255
			#var c : Color = Color(r, g, b)
			#
			#new_terrain.color_map.set_pixel(j, i, c)
	
	new_terrain.set_shader_cmap()
	
	return new_terrain
	
