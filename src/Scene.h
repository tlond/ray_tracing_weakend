#pragma once 
#include "hittable.h"

#include <memory>
#include <vector>

class Scene{
  public:
    std::vector<std::shared_ptr<Hittable>> objects;

    Scene() {}
    Scene(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    std::optional<HitRecord> hit(const Ray &r, const Range &range) const {
        std::optional<HitRecord> temp_rec;
        std::optional<HitRecord> res;
        bool hit_anything = false;
        auto closest_so_far = range.end;

        for (const auto& object : objects) {
            temp_rec = object->hit(r, {range.start, closest_so_far});
            if (temp_rec) {
                closest_so_far = temp_rec->t;
                res = temp_rec;
            }
        }

        return res;
    }
};