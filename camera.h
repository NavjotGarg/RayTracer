#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
	camera(point3 lookFrom, point3 lookAt, vec3 vup, float vfov, float aspect_ratio, float aperture, float focus_dist) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2.0f);
		auto viewport_height = 2.0f * h;
		auto viewport_width = aspect_ratio * viewport_height;
		auto focal_length = 1.0f;

		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vup, w));
		v = unit_vector(cross(w, u));

		origin = lookFrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist*w;

		lens_radius = aperture / 2;
	}

	ray get_ray(float s, float t) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
	}

private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius;
};
#endif
