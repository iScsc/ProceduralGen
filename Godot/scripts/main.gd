@tool

extends Node


@export var loading_path: String = "res://data/map.save"

@export var load_terrain_from_path: bool = false:
	set(value):
		if Engine.is_editor_hint():
			load_terrain_from_path = value
			
			if load_terrain_from_path:
				load_terrain()
				terrain.generate_all()
				load_terrain_from_path = false

@export var terrain: Terrain

# Called when the node enters the scene tree for the first time.
func _ready():
	if not Engine.is_editor_hint():
		load_terrain()
		terrain.generate_all()


func load_terrain() -> void:
	if terrain is Terrain:
		terrain.queue_free()
	
	var new_terrain := Terrain.load_terrain(loading_path)
	add_child(new_terrain, true)
	new_terrain.owner = self
	terrain = new_terrain
