extends CharacterBody3D

# CONSTANTS

const SPEED := 5.0
const JUMP_VELOCITY := 4.5
const ROTATION_SPEED := 5e-2

const RESET_POSITION := Vector3(0, 10, 0)
const LOWER_LIMIT := -100.

const CAMERA_LIMITS := Vector2(-PI/3, PI/3)

# VARIABLES

@export var camera: Camera3D


# Get the gravity from the project settings to be synced with RigidBody nodes.
var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")


func _init():
	Input.mouse_mode = Input.MOUSE_MODE_CONFINED_HIDDEN



func _physics_process(delta):
	## Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta
	
	## Handle jump.
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY
	
	
	var input_rot: Vector2 = get_viewport().get_mouse_position() - Vector2(get_viewport().size/2)
	var rot := Vector3(0, input_rot.x, 0)
	if rot and DisplayServer.window_is_focused():
		rotation -= rot * ROTATION_SPEED * delta
		camera.rotation.x = clamp(camera.rotation.x - input_rot.y * ROTATION_SPEED * delta,
								 CAMERA_LIMITS.x, CAMERA_LIMITS.y)
	
	## Get the input direction and handle the movement/deceleration.
	var input_dir := Input.get_vector("left", "right", "forward", "backward")
	var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if direction:
		velocity.x = direction.x * SPEED
		velocity.z = direction.z * SPEED
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED)
		velocity.z = move_toward(velocity.z, 0, SPEED)
	
	
	## Actually moving
	move_and_slide()
	
	
	## Keep cursor in the middle of the screen
	if DisplayServer.window_is_focused():
		get_viewport().warp_mouse(get_viewport().size/2)
	
	
	## Avoid falling off infinetly
	if position.y < LOWER_LIMIT:
		position = RESET_POSITION
		velocity = Vector3.ZERO
