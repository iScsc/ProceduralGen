@tool

extends Node


@export var loading_path: String = "res://data/map.save"

@export var load_terrain_from_path: bool = false:
	set(value):
		if Engine.is_editor_hint():
			load_terrain()
			terrain.generate_all()

@export var remove_terrain: bool = false:
	set(value):
		if Engine.is_editor_hint():
			free_terrain()

@export var terrain: Terrain

# Called when the node enters the scene tree for the first time.
func _ready():
	if not Engine.is_editor_hint(): # and not terrain is Terrain:
		load_terrain()
		terrain.generate_all()


func free_terrain() -> void:
	if terrain is Terrain:
		terrain.queue_free()
		terrain = null


func load_terrain() -> void:
	free_terrain()
	
	var new_terrain := Terrain.load_terrain(loading_path)
	add_child(new_terrain, true)
	new_terrain.owner = self
	terrain = new_terrain
