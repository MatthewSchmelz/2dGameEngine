#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_vector.h"
#include "gfc_shape.h"


/*
	@brief Get Camera position in world space
	@return the position
*/

Vector2D camera_get_position();


/*
	@brief get the offset to draw things relatvie to the camera
	@return the offset
*/
Vector2D camera_get_offset();

/*
	@brief Set the camer'as position in world space
	@param The position of the camera
*/

void camera_set_position(Vector2D position);



void camera_apply_bounds();

void camera_set_bounds(Rect bounds);

void camera_enable_binding(Bool bindCamera);

void camera_set_size(Vector2D size);


void camera_center_on(Vector2D target);
#endif