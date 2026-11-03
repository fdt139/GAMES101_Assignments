//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    if (depth > maxDepth) {
        return 0;
    }
    auto inter = intersect(ray);
    if (!inter.happened) {
        return 0;
    }
    if (inter.m->hasEmission()) {
        return inter.m->getEmission();
    }
    else {
        const auto x = inter.coords;
        const auto N = inter.normal;
        Vector3f wo = -1.0f*ray.direction;
        auto ws = inter.m->sample(wo, N);
        float pdf = inter.m->pdf(wo, ws, N);
        Vector3f L_dir = 0;
        Vector3f L_indir = 0;
        if (pdf > 0) {
            sampleLight(inter, pdf);
            Ray ray2{ x + 1e-2 * N, inter.coords - (x + 1e-2 * N) };
            auto inter2 = intersect(ray2);
            if ((inter2.coords - inter.coords).norm() <= 1e-2) {
                Vector3f NN = inter.normal;
                ws = normalize(inter.coords - x);
                auto pdf_light = pdf;
                auto p = inter2.coords;
                auto emit = inter2.m->getEmission();
                L_dir = emit * inter.m->eval(wo, ws, N) * dotProduct(-ws, N) * dotProduct(ws,
                    NN) / dotProduct((x - p) , (x - p))/ pdf_light;
            }
            if (get_random_float() < 0.8) {
                auto wi = inter.m->sample(wo, N);
                Ray ray3{ x + 1e-2 * N, wi };
                auto inter3 = intersect(ray3);

                if (inter3.happened && !inter3.m->hasEmission()) {
                    L_indir = castRay(ray3, depth + 1) * inter.m->eval(wo, wi, N) * dotProduct(wi, N)
                        / inter.m->pdf(wo, wi, N) / RussianRoulette;
                }
            }
        }
        return L_dir + L_indir;
    }
    return 0;
}