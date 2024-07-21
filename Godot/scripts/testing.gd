@tool

extends Node

@export var testing_mesh: MeshInstance3D

## Launch test function when pressed in editor
@export var launch_test_function: bool = false:
	set(value):
		if Engine.is_editor_hint():
			test()


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func test() -> void:
	if not testing_mesh is MeshInstance3D:
		var new_mesh_instance = MeshInstance3D.new()
		add_child(new_mesh_instance, true)
		new_mesh_instance.owner = self
		testing_mesh = new_mesh_instance
		new_mesh_instance.position = Vector3(0, 2, 0)
	
	
	
	var vertices = PackedVector3Array()
	vertices.push_back(Vector3(0, 1, 0))
	vertices.push_back(Vector3(1, 0, 0))
	vertices.push_back(Vector3(0, 0, 1))
	
	vertices.push_back(Vector3(2, 1, 0))
	vertices.push_back(Vector3(3, 0, 0))
	vertices.push_back(Vector3(2, 0, 1))
	
	vertices.push_back(Vector3(0, 1, 2))
	vertices.push_back(Vector3(1, 0, 2))
	vertices.push_back(Vector3(0, 0, 3))
	
	
	var indices = PackedInt32Array()
	indices.push_back(0)
	indices.push_back(1)
	indices.push_back(2)
	
	indices.push_back(3)
	indices.push_back(4)
	indices.push_back(5)
	
	#indices.push_back(6)
	#indices.push_back(7)
	#indices.push_back(8)
	
	
	# Initialize the ArrayMesh.
	var arr_mesh := ArrayMesh.new()
	var arrays = []
	arrays.resize(Mesh.ARRAY_MAX)
	arrays[Mesh.ARRAY_VERTEX] = vertices
	arrays[Mesh.ARRAY_INDEX] = indices

	# Create the Mesh.
	arr_mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, arrays)
	
	testing_mesh.mesh = arr_mesh
