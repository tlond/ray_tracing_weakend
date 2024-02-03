#pragma once

#include "math.hpp"
#include "ray.h"

#include <optional>

class material;

struct HitRecord
{
  Vec3 p;
  Vec3 normal;
  float t;
  bool front_face;
  std::shared_ptr<material> mat;

  void set_face_normal(const Ray &r, const Vec3 &normal)
  {
    // Sets the hit record normal vector.
    // NOTE: the parameter `normal` is assumed to have unit length.

    front_face = dot(r.direction(), normal) < 0;
    this->normal = front_face ? normal : Vec3() - normal;
  }
};

struct Range
{
  float start = std::numeric_limits<float>::min();
  float end = std::numeric_limits<float>::max();
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual std::optional<HitRecord> hit(const Ray &r, const Range &range) const = 0;
};
