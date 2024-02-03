#include "camera.h"
#include "math.hpp"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "scene.h"
#include "utils.h"

#include "image.h"

#include <array>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <cmath>
#include <span>
#include <random>
#include <algorithm>

// Basic ray-sphere intersection test
std::optional<float> intersect(const Sphere& sphere, const Vec3& rayOrigin, const Vec3& rayDir) {
    Vec3 oc = rayOrigin - sphere.center();
    const auto a = dot(rayDir, rayDir); //length_squared();

    const auto b = 2.0f * dot(oc, rayDir);
    const auto c = dot(oc, oc) - sphere.radius() * sphere.radius();
    const auto discriminant = b * b - 4 * a * c;

    if (discriminant < 0.f)
    {
        return std::nullopt;
    }
    else
    {
        return std::optional((-b - sqrt(discriminant)) / (2.f * a));
    }
}

// Ray tracing function
Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres) {
    for (const auto& sphere : spheres) {
        //auto hitResult = intersect(spher, ray.origin(), ray.direction());
        const auto hitResult = sphere.hit(ray, {});
        if (hitResult && hitResult->t > 0.f)
        {
            //Vec3 N = unit_vector(ray.at(*intersetcResult) - spher.center());
            const auto& normal = hitResult->normal;
            return 0.5 * Vec3(normal.x + 1, normal.y + 1, normal.z + 1);
        }
    }

    Vec3 unit_direction = unit_vector(ray.direction());
    auto a = 0.5f*(unit_direction.y + 1.f);
    return (1.f-a)*Vec3(1.f, 1.f, 1.f) + a*Vec3(0.5f, 0.7f, 1.0f);
}

Vec3 trace( const Scene& scene, const Ray& ray, int depth) {
    if (depth <= 0)
        return {0.f, 0.f, 0.f};

    // auto hitResult = intersect(spher, ray.origin(), ray.direction());
    const auto hitResult = scene.hit(ray, {0.001f, std::numeric_limits<float>::max()});
    if (hitResult && hitResult->t > 0.f)
    {
        //const auto dir = random_on_hemisphere(hitResult->normal);

        //const auto dir = hitResult->normal + random_unit_vector();
        // Vec3 N = unit_vector(ray.at(*intersetcResult) - spher.center());
        // const auto &normal = hitResult->normal;
        // return 0.5 * Vec3(normal.x + 1, normal.y + 1, normal.z + 1);
        //return 0.5f * trace(scene, {hitResult->p, dir}, depth - 1);

        Ray scattered;
        Vec3 attenuation;
        if (hitResult->mat->scatter(ray, *hitResult, attenuation, scattered))
            return attenuation * trace(scene, scattered, depth - 1);
        return {0, 0, 0};
    }

    Vec3 unit_direction = unit_vector(ray.direction());
    auto a = 0.5f*(unit_direction.y + 1.f);
    return (1.f-a)*Vec3(1.f, 1.f, 1.f) + a*Vec3(0.5f, 0.7f, 1.0f);
}

using Color = Vec3;

void write_color(std::ostream &out, Color pixel_color) {
    auto r = sqrt(pixel_color.x);
    auto g = sqrt(pixel_color.y);
    auto b = sqrt(pixel_color.z);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * std::clamp(r, 0.000f, 0.999f)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.000f, 0.999f)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.000f, 0.999f)) << '\n';
}

void save_ppm(const Image<char, 3>& img, const std::string& filename) {
    std::ofstream ppmFile(filename, std::ios::binary);
    if (!ppmFile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    ppmFile << "P6\n";
    ppmFile << img.width << " " << img.height << "\n";
    ppmFile << "255\n";

    for (int y = 0; y < img.size().y; ++y) {
        for (int x = 0; x < img.size().x; ++x) {
            auto row = img[y];
            const char* pixelData = &row[x];

            ppmFile.write(pixelData, img.numChannels);
        }
    }

    std::cout << "Image saved to " << filename << std::endl;
}

std::vector<Vec2f> get_pixels(const Vec2f& pixel, const Vec2f& windowSize, int numSamples) {
    std::vector<Vec2f> sampledPixels;

    float halfWindowSizeX = windowSize.x / 2.0f;
    float halfWindowSizeY = windowSize.y / 2.0f;

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    for (int i = 0; i < numSamples; ++i) {
        float offsetX = dist(rng) * halfWindowSizeX;
        float offsetY = dist(rng) * halfWindowSizeY;

        Vec2f sampledPixel(
            std::clamp(pixel.x + offsetX, -1.0f, 1.0f),
            std::clamp(pixel.y + offsetY, -1.0f, 1.0f)
        );

        sampledPixels.push_back(sampledPixel);
    }

    return sampledPixels;
}

std::vector<Vec2i> get_pixels(const Vec2i& pixel, const Vec2i& windowSize, const Vec2i& imageSize) {
    const int halfWindowSizeX = windowSize.x / 2;
    const int halfWindowSizeY = windowSize.y / 2;

    const int windowOriginX = std::max(0, pixel.x - halfWindowSizeX);
    const int windowOriginY = std::max(0, pixel.y - halfWindowSizeY);

    const int maxX = std::min(pixel.x + halfWindowSizeX, imageSize.x - 1);
    const int maxY = std::min(pixel.y + halfWindowSizeY, imageSize.y - 1);

    const int numPixels = windowSize.x * windowSize.y;
    std::vector<Vec2i> nearestPixels;
    nearestPixels.reserve(numPixels); // Reserve space for efficiency

    for (int y = windowOriginY; y <= maxY; ++y) {
        for (int x = windowOriginX; x <= maxX; ++x) {
            nearestPixels.emplace_back(x, y);
        }
    }

    return nearestPixels;
}

// sample pixel and store clor vaue to image
void MSAA(const Camera& camera, const Scene& scene, auto& img)
{
    // const auto sreenPoint = NDC_to_screen_space(raster_to_NDC(pixel, img.width, img.height));
    // const auto tmp_ray = camera.generateRay(sreenPoint);
    // const auto world_tmp_ray = camera.generateWorldRay(sreenPoint);

    // Color color = trace(scene, world_tmp_ray);
    // const Vec2i windowSize(2, 2);
    for (int y = 0; y < img.height; ++y)
    {
        for (int x = 0; x < img.width; ++x)
        {
            const auto sreenPoint = NDC_to_screen_space(raster_to_NDC(Vec2i{x, y}, img.width, img.height));
            //const auto sreenPoint = NDC_to_screen_space(raster_to_NDC(Vec2f{(float)x, (float)y}, img.width, img.height));
            std::vector<Vec2f> nearestPixels;
            nearestPixels = get_pixels(sreenPoint, {05.f /(2.f * img.width), 5.f /(2.f * img.height)}, 500);
            //nearestPixels.push_back(sreenPoint);
            Color color;
            for (const auto &pixel : nearestPixels)
            {
                const auto tmp_ray = camera.generateRay(pixel);
                const auto world_tmp_ray = camera.generateWorldRay(pixel);

                color += trace(scene, world_tmp_ray, 50);
            }
            color /= nearestPixels.size();
            //typename std::decay_t<decltype(img)>::RowProxy
            auto row = img[y]; // Access the row
            auto *pixelData = &row[x];    // Access the pixel data

            // Assign color values to the pixel data (assuming RGB format)
            // pixelData[0] = static_cast<char>(color.x * 255);
            // pixelData[1] = static_cast<char>(color.y * 255);
            // pixelData[2] = static_cast<char>(color.z * 255);
            auto r = sqrt(color.x);
            auto g = sqrt(color.y);
            auto b = sqrt(color.z);

            // Write the translated [0,255] value of each color component.
            pixelData[0] =  static_cast<int>(256 * std::clamp(r, 0.000f, 0.999f));
            pixelData[1] =  static_cast<int>(256 * std::clamp(g, 0.000f, 0.999f));
            pixelData[2] =  static_cast<int>(256 * std::clamp(b, 0.000f, 0.999f));
            //std::cout << "x: " << x << " y: " << y << '\n';
        }
    }
}

float random_float(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vec3 random_vec3(float min, float max)
{
    return Vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

Vec3 random_vec3()
{
    return random_vec3(0.f, 1.f);
}

int main()
{
    const int image_width = 1200;
    float aspectRatio = 16.0f / 9.0f;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspectRatio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera properties
    Vec3 cameraPosition(13.0f, 2.0f, 3.0f);
    // the point that the camera is looking at. It helps define the camera's orientation.
    Vec3 target(0.f, 0.f, 0.f);
    Vec3 up(0.0f, 1.0f, 0.0f);
    float fov = 20.0f;

    Camera camera(cameraPosition, target, up, fov, aspectRatio);

    auto material_ground = std::make_shared<lambertian>(Color(0.8f, 0.8f, 0.f));
    auto material_center = std::make_shared<lambertian>(Color(0.1f, 0.1f, 0.2f));
    auto material_left = std::make_shared<dielectric>(1.5);
    auto material_right = std::make_shared<metal>(Color(0.8f, 0.6f, 0.2f), 0.f);
    //auto material_center = std::make_shared<dielectric>(1.5);
    //auto material_left = std::make_shared<dielectric>(1.5);
    //auto material_right = std::make_shared<metal>(Color(0.8f, 0.6f, 0.2f), 1.f);

    Scene scene;
    auto ground_material = std::make_shared<lambertian>(Color(0.8f, 0.8f, 0.f));
    scene.add(std::make_shared<Sphere>(Vec3(0.f,-100.5f, -1.f), 1000.f, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float(0.f, 1.f);
            Vec3 center(a + 0.9f*random_float(0.f, 1.f), 0.2f, b + 0.9f*random_float(0.f, 1.f));

            if ((center - Vec3(4.f, 0.2f, 0.f)).length() > 0.9f) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    scene.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = random_vec3(0.5f, 1.f);
                    auto fuzz = random_float(0.f, 0.5f);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    scene.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5f);
                    scene.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    // Scene scene(std::make_shared<Sphere>(target, 0.5f, material_center));
    // scene.add(std::make_shared<Sphere>(Vec3(0.f, 0.f, -1.f), 0.5f, material_center));
    // scene.add(std::make_shared<Sphere>(Vec3(-1.f, 0.f, -1.f), 0.5f, material_left));
    // scene.add(std::make_shared<Sphere>(Vec3(-1.f, 0.f, -1.f), -0.4f, material_left));
    // scene.add(std::make_shared<Sphere>(Vec3(1.f, 0.f, -1.f), 0.5f, material_right));

    Image<char, 3> img(image_width, image_height);
    MSAA(camera, scene, img);
    save_ppm(img, "camera_output_msaa.ppm");
    return 0;
}
