extends Node

@onready var world_display = $Display

func _ready() -> void:
	# Called when the node enters the scene tree for the first time.
	pass

func _process(_delta: float) -> void:
	# Called every frame. 'delta' is the elapsed time since the previous frame.
	pass

func _on_tree_sim_position_changed(_node: Object, _new_pos: Vector2) -> void:
	#print("wow")
	pass
	
func _on_tree_sim_world_updated(_node: Object, new_world: Array) -> void:
	#print("World updated! " + node.get_class() + "'s world is " + str(new_world))
	world_display.get_material().set_shader_parameter("world", new_world)
