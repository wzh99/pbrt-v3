#pragma once
#include "bdpt.h"

namespace pbrt {

using LightIndexMap = std::unordered_map<const Light*, size_t>;

class VCMIntegrator : public Integrator {
public:
    VCMIntegrator(std::shared_ptr<Camera> camera, int nIte, int maxDepth);
    void Render(const Scene& scene) override;

private:
    struct Vertex;
    struct PathState;

    PathState sampleLight(const Scene& scene, Sampler& sampler) const;
    size_t lightRandomWalk(const Scene& scene, Sampler& sampler, MemoryArena& arena, PathState& lightState,
        Vertex* path, Film* film) const;
    void connectToCamera(const Scene& scene, Sampler& sampler, const Vertex& lightVert, Film* film) const;
    PathState sampleCamera(Sampler& sampler, const Point2f& pFilm) const;
    Spectrum connectToLight(const Scene& scene, Sampler& sampler, const PathState& camState) const;
    Spectrum hitLightSource(const Light* light, PathState& camState, SurfaceInteraction* isect) const;
    Spectrum connectVertex(const Scene& scene, Sampler& sampler, const Vertex& lightVertex,
        const PathState& camState) const;
    bool sampleScattering(Sampler& sampler, const Vertex& vertex, PathState& state, TransportMode mode) const;
    
    // Initialized at construction time
    std::shared_ptr<Camera> camera; 
    int nIte, maxDepth;

    // Shared data at render time
    int nPixels, nLightPaths;
    std::unique_ptr<Distribution1D> lightDistrib;
    LightIndexMap lightIndex;
};

}
