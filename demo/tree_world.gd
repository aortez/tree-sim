extends Node

@onready var world_display = $WorldDisplay

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func _on_world_world_updated(_node: Object, new_world: Array) -> void:
	#print("World updated! " + node.get_class() + "'s world is " + str(new_world))
	world_display.get_material().set_shader_parameter("world", new_world)
