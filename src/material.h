#pragma once

#include "ray.h"
#include "utils.h"
#include "hittable.h"

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const Vec3 &a) : albedo(a) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered)
        const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (near_zero(scatter_direction))
        {
            scatter_direction = rec.normal;
        }

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    Vec3 albedo;
};

class metal : public material
{
public:
    metal(const Vec3 &a, float fuzz_) : albedo(a), fuzz(fuzz_) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered)
        const override
    {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_unit_vector());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

private:
    Vec3 albedo;
    float fuzz;
};

class dielectric : public material {
  public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered)
        const override
    {
        attenuation = Vec3(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Vec3 unit_direction = unit_vector(r_in.direction());
        Vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray(rec.p, refracted);
        return true;
    }

  private:
    double ir; // Index of Refraction
};