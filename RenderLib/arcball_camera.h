#pragma once

//#include "..\glm\ext.hpp"
//#include "..\glm\glm.hpp"
#include "..\DirectXTK\Inc\SimpleMath.h"

using namespace DirectX::SimpleMath;

/* A simple arcball camera that moves around the camera's focal point.
 * The mouse inputs to the camera should be in normalized device coordinates,
 * where the top-left of the screen corresponds to [-1, 1], and the bottom
 * right is [1, -1].
 */
class ArcballCamera {
	// We store the unmodified look at matrix along with
	// decomposed translation and rotation components
	Matrix center_translation, translation;
	Quaternion rotation;
	// camera is the full camera transform,
	// inv_camera is stored as well to easily compute
	// eye position and world space rotation axes
	Matrix camera, inv_camera;

public:
	/* Create an arcball camera focused on some center point
	 * screen: [win_width, win_height]
	 */
	ArcballCamera(const Vector3& eye, const Vector3& center, const Vector3& up);

	/* Rotate the camera from the previous mouse position to the current
	 * one. Mouse positions should be in normalized device coordinates
	 */
	void rotate(Vector2 prev_mouse, Vector2 cur_mouse);

	/* Pan the camera given the translation vector. Mouse
	 * delta amount should be in normalized device coordinates
	 */
	void pan(Vector2 mouse_delta);

	/* Zoom the camera given the zoom amount to (i.e., the scroll amount).
	 * Positive values zoom in, negative will zoom out.
	 */
	void zoom(const float zoom_amount);

	// Get the camera transformation matrix
	const Matrix& transform() const;

	// Get the camera's inverse transformation matrix
	const Matrix& inv_transform() const;

	// Get the eye position of the camera in world space
	Vector3 eye() const;

	// Get the eye direction of the camera in world space
	Vector3 dir() const;

	// Get the up direction of the camera in world space
	Vector3 up() const;

private:
	void update_camera();
};
