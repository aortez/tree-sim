extends Node2D

func _on_position_changed(node: Object, new_pos: Vector2) -> void:
	print("The position of " + node.get_class() + " is now " + str(new_pos))
