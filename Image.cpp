#include <iostream>
#include "rtweekend.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

// get color per sample per geometry
color ray_color(const ray& r, const hittable_list& world, int depth) {
	hit_record rec;
	// exceeded ray bounce limit, return 0 light
	if (depth == 0)
	{
		return color(0.0f, 0.0f, 0.0f);
	}
	if (world.hit(r, 0.001f, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if(rec.mat_ptr->scatter(r,rec,attenuation,scattered))
			return attenuation * ray_color(scattered, world, depth-1);
		return color(0.0f, 0.0f, 0.0f);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5f*(unit_direction.y() + 1.0f);
	return (1.0f - t)*color(1.0f, 1.0f, 1.0f) + t*color(0.5, 0.7f, 1.0f);
}

hittable_list random_scene() {
	hittable_list world;

	// small test case below
	/*world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f, make_shared<lambertian>(color(0.7f,0.3f,0.3f))));
	world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, make_shared<lambertian>(color(0.8f, 0.8f, 0.0f))));
	world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, make_shared<metal>(color(0.8f, 0.6f, 0.2f), 0.3f)));
	world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, make_shared<dielectric>(1.5f)));*/

	auto ground_material = make_shared<lambertian>(color(0.5f, 0.5f, 0.5f));
	world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_float();
			point3 center(a + 0.9f*random_float(), 0.2f, b + 0.9f*random_float());

			if ((center - point3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95f) {
					// metal
					auto albedo = color::random(0.5f, 1.0f);
					auto fuzz = random_float(0.0f, 0.5f);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5f);
					world.add(make_shared<sphere>(center, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5f);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0f, material1));

	auto material2 = make_shared<lambertian>(color(0.4f, 0.2f, 0.1f));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0f, material2));

	auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0f, material3));

	return world;
}

int main() {
	const auto aspect_ratio = 16.0f / 9.0f;
	const int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// create a list of things in the world and how you can hit them
	auto world = random_scene();

	//uncomment below for a hollow glass ball
	//world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), -0.45f, make_shared<dielectric>(1.5f)));

	// create a camera with viewport and aspect ratio
	// ray launched from this camera origin
	point3 lookfrom(13.0f, 2.0f, 3.0f);
	point3 lookat(0, 0, 0.0f);
	vec3 vup(0, 1.0f, 0);
	auto dist_to_focus = 10.0f;
	auto aperture = 0.1f;
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	for (int j = image_height - 1; j >= 0; --j)
	{
		// progress update
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			color pixel_color(0.0f, 0.0f, 0.0f);
			// iterate over pixel sample to anti-alias
			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = (float(i) + random_float()) / (image_width - 1);
				auto v = (float(j) + random_float()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
				
			}
			// averge out the accumlated color per pixel for all samples
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}