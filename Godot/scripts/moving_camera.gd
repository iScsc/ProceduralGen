class_name MovingCamera extends Camera3D

# CONSTANTS

const SPEED := 5.0
const ROTATION_SPEED := 5e-2

const CAMERA_LIMITS := Vector2(-PI/2, PI/2)



@export var speed_multiplier : float = 1
@export var sprint_multiplier : float = 2



func _init():
	Input.mouse_mode = Input.MOUSE_MODE_CONFINED_HIDDEN



func _physics_process(delta):
	if DisplayServer.window_is_focused():
		var input_rot: Vector2 = get_viewport().get_mouse_position() - Vector2(get_viewport().size/2)
		var rot := Vector3(input_rot.y, input_rot.x, 0)
		if rot:
			rotation -= rot * ROTATION_SPEED * delta
			rotation.x = clamp(rotation.x, CAMERA_LIMITS.x, CAMERA_LIMITS.y)
	
	
	var local_mult := speed_multiplier
	if Input.is_action_pressed("sprint"):
		local_mult *= sprint_multiplier
	
	
	## Handle rising
	if Input.is_action_pressed("jump"):
		position.y += local_mult * SPEED * delta
	## Handle lowering
	if Input.is_action_pressed("crouch"):
		position.y -= local_mult * SPEED * delta
	
	
	## Get the input direction and handle the movement/deceleration.
	var input_dir := Input.get_vector("left", "right", "forward", "backward")
	var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	
	position += direction * local_mult * SPEED * delta
	
	
	## Keep cursor in the middle of the screen
	if DisplayServer.window_is_focused():
		get_viewport().warp_mouse(get_viewport().size/2)
