#include "arcball_camera.h"
#include <cmath>
#include <iostream>
//#include "..\glm\ext.hpp"
//#include "..\glm\gtx/transform.hpp"

// Project the point in [-1, 1] screen space onto the arcball sphere
static Quaternion screen_to_arcball(const Vector2& p);

ArcballCamera::ArcballCamera(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	const Vector3 dir = center - eye;
	Vector3 z_axis = Vector3::Normalize(dir);
	Vector3 x_axis = Vector3::Normalize(Vector3::Cross(z_axis, Vector3::Normalize(up)));
	Vector3 y_axis = Vector3::Normalize(Vector3::Cross(x_axis, z_axis));
	x_axis = Vector3::Normalize(Vector3::Cross(z_axis, y_axis));

	center_translation = Matrix::CreateTranslation(center).Invert();
	translation = Matrix::CreateTranslation(Vector3(0.f, 0.f, dir.Length()));
	rotation = Quaternion::CreateFromRotationMatrix(Matrix(x_axis, y_axis, -z_axis).Transpose());
	rotation.Normalize();

	update_camera();
}

void ArcballCamera::rotate(Vector2 prev_mouse, Vector2 cur_mouse)
{
	// Clamp mouse positions to stay in NDC
	cur_mouse.Clamp(Vector2{ -1, -1 }, Vector2{ 1, 1 });

	prev_mouse.Clamp(Vector2{ -1, -1 }, Vector2{ 1, 1 });

	const Quaternion mouse_cur_ball = screen_to_arcball(cur_mouse);
	const Quaternion mouse_prev_ball = screen_to_arcball(prev_mouse);

	rotation = mouse_cur_ball * mouse_prev_ball * rotation;
	update_camera();
}

void ArcballCamera::pan(Vector2 mouse_delta)
{
	const float zoom_amount = std::abs(translation.m[3][2]);
	Vector4 motion(mouse_delta.x * zoom_amount, mouse_delta.y * zoom_amount, 0.f, 0.f);
	// Find the panning amount in the world space
	motion = inv_camera * motion;

	center_translation = glm::translate(Vector3(motion)) * center_translation;
	update_camera();
}

void ArcballCamera::zoom(const float zoom_amount)
{
	const Vector3 motion(0.f, 0.f, zoom_amount);

	translation = glm::translate(motion) * translation;
	update_camera();
}

const Matrix& ArcballCamera::transform() const
{
	return camera;
}

const Matrix& ArcballCamera::inv_transform() const
{
	return inv_camera;
}

Vector3 ArcballCamera::eye() const
{
	return Vector3{ inv_camera * Vector4{0, 0, 0, 1} };
}

Vector3 ArcballCamera::dir() const
{
	return Vector3::Normalize(Vector3{ inv_camera * Vector4{0, 0, -1, 0} });
}

Vector3 ArcballCamera::up() const
{
	return Vector3::Normalize(Vector3{ inv_camera * Vector4{0, 1, 0, 0} });
}

void ArcballCamera::update_camera()
{
	camera = translation * Matrix_cast(rotation) * center_translation;
	inv_camera = glm::inverse(camera);
}

Quaternion screen_to_arcball(const Vector2& p)
{
	const float dist = glm::dot(p, p);
	// If we're on/in the sphere return the point on it
	if (dist <= 1.f) {
		return Quaternion(0.0, p.x, p.y, std::sqrt(1.f - dist));
	}
	else {
		// otherwise we project the point onto the sphere
		const Vector2 proj = Vector2::Normalize(p);
		return Quaternion(0.0, proj.x, proj.y, 0.f);
	}
}