#pragma once

#include "hittable.h"
#include "math.hpp"

class Sphere : public Hittable {
  public:
    Sphere(Vec3 center, float radius, std::shared_ptr<material> _material) : m_center(center), m_radius(radius), mat(_material) {}

    Vec3 center() const { return m_center; }
    auto radius() const { return m_radius; }

    std::optional<HitRecord> hit(const Ray& r, const Range& range) const override {
        const Vec3 oc = r.origin() - m_center;
        const auto a = r.direction().length_squared();
        const auto half_b = dot(oc, r.direction());
        const auto c = oc.length_squared() - m_radius*m_radius;

        const auto discriminant = half_b*half_b - a*c;
        
        if (discriminant < 0)
          return std::nullopt;
        
        const auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root <= range.start || range.end <= root) {
            root = (-half_b + sqrtd) / a;
            if (root <= range.start || range.end <= root)
                return std::nullopt;
        }
        HitRecord rec;
        rec.t = root;
        rec.p = r.at(rec.t);
        rec.mat = mat;
    
        const auto normal = (rec.p - m_center) / m_radius;
        rec.set_face_normal(r, normal);
        return std::make_optional(std::move(rec));
    }

  private:
    Vec3 m_center;
    float m_radius;
    std::shared_ptr<material> mat;
};
