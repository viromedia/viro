//
//  VROGeometry.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROGeometry_h
#define VROGeometry_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <atomic>
#include "VRORenderContext.h"
#include "VRODriver.h"
#include "VROSortKey.h"
#include "VROBoundingBox.h"
#include "VROAnimatable.h"
#include "VROSkinner.h"
#include "VROAllocationTracker.h"

class VRONode;
class VROLight;
class VROSortKey;
class VROMaterial;
class VROTexture;
class VROGeometryElement;
class VROGeometrySource;
class VROGeometrySubstrate;
class VROMatrix4f;
class VROInstancedUBO;
class VRORenderMetadata;
enum class VROGeometrySourceSemantic;

/*
 Represents a three-dimensional shape, a collection of vertices, normals and texture coordinates
 that define a surface, also known as a model or mesh. Geometries attached to VRONode objects form
 the visible elements of a scene, and VROMaterial objects attached to a geometry determine its 
 appearance.
 */
class VROGeometry : public VROAnimatable {
    
public:
    
    /*
     Construct a new geometry with the given sources and elements.
     */
    VROGeometry(std::vector<std::shared_ptr<VROGeometrySource>> sources,
                std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        _geometrySources(sources),
        _geometryElements(elements),
        _cameraEnclosure(false),
        _screenSpace(false),
        _boundingBoxComputed(false),
        _substrate(nullptr),
        _instancedUBO(nullptr) {

        _bounds = VROBoundingBox();
        ALLOCATION_TRACKER_ADD(Geometry, 1);
    }
    
    /*
     Construct a new geometry with no sources or elements. These are expected
     to be set by the subclass.
     */
    VROGeometry() :
        _cameraEnclosure(false),
        _screenSpace(false),
        _boundingBoxComputed(false),
        _substrate(nullptr) {

        _bounds = VROBoundingBox();
        ALLOCATION_TRACKER_ADD(Geometry, 1);
    }
    
    /*
     Copy the given geometry. The materials will *not* be copied, and the
     underlying immutable geometry data will be shared.
     */
    VROGeometry(std::shared_ptr<VROGeometry> geometry) :
        _geometrySources(geometry->_geometrySources),
        _geometryElements(geometry->_geometryElements) {
        
         ALLOCATION_TRACKER_ADD(Geometry, 1);
    }
    
    virtual ~VROGeometry();
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     */
    void deleteGL();
    
    /*
     Get the geometry ready for usage now, in advance of when it's visible. If not invoked,
     the geometry will be initialized when it is made visible.
     */
    void prewarm(std::shared_ptr<VRODriver> driver);

    /*
     Render the given element of the geometry with full texturing and
     lighting. Assumes the material's shader and geometry-independent
     properties have already been bound.
     */
    void render(int elementIndex,
                const std::shared_ptr<VROMaterial> &material,
                VROMatrix4f transform,
                VROMatrix4f normalMatrix,
                float opacity,
                const VRORenderContext &context,
                std::shared_ptr<VRODriver> &driver);
    
    /*
     Render the silhouette of the entire geometry (all elements). Renders
     using the given material, which is assumed to already be bound, ignoring
     texturing and lighting. Typically this is used for rendering to a stencil
     buffer or shadow map.
     */
    void renderSilhouette(VROMatrix4f transform,
                          std::shared_ptr<VROMaterial> &material,
                          const VRORenderContext &context,
                          std::shared_ptr<VRODriver> &driver);
    
    /*
     Render the silhouette of the given element of the given geometry.
     Renders using the provided material, which is assumed to already be
     bound, and binds its associated texture.
     */
    void renderSilhouetteTextured(int element,
                                  VROMatrix4f transform,
                                  std::shared_ptr<VROMaterial> &material,
                                  const VRORenderContext &context,
                                  std::shared_ptr<VRODriver> &driver);
    
    void updateSortKeys(VRONode *node, uint32_t hierarchyId, uint32_t hierarchyDepth,
                        uint32_t lightsHash, const std::vector<std::shared_ptr<VROLight>> &lights,
                        float opacity, float distanceFromCamera, float zFar,
                        std::shared_ptr<VRORenderMetadata> &metadata,
                        const VRORenderContext &context,
                        std::shared_ptr<VRODriver> &driver);
    void getSortKeys(std::vector<VROSortKey> *outKeys);
    
    std::shared_ptr<VROMaterial> &getMaterialForElement(int elementIndex) {
        return _materials[elementIndex % _materials.size()];
    }
    
    virtual void setMaterials(std::vector<std::shared_ptr<VROMaterial>> materials) {
        _materials = materials;
    }
    const std::vector<std::shared_ptr<VROMaterial>> &getMaterials() {
        return _materials;
    }
       
    const std::vector<std::shared_ptr<VROGeometrySource>> &getGeometrySources() const {
        return _geometrySources;
    }
    const std::vector<std::shared_ptr<VROGeometryElement>> &getGeometryElements() const {
        return _geometryElements;
    }
    
    const VROBoundingBox &getBoundingBox();
    VROBoundingBox getLastBoundingBox() const;
    void updateBoundingBox();

    VROVector3f getCenter();
    
    bool isCameraEnclosure() const {
        return _cameraEnclosure;
    }
    void setCameraEnclosure(bool enabled) {
        _cameraEnclosure = enabled;
    }
    
    bool isScreenSpace() const {
        return _screenSpace;
    }
    void setScreenSpace(bool screenSpace) {
        _screenSpace = screenSpace;
    }
    
    std::string getName() const {
        return _name;
    }
    void setName(std::string name) {
        _name = name;
    }
    
    const std::unique_ptr<VROSkinner> &getSkinner() const {
        return _skinner;
    }
    void setSkinner(std::unique_ptr<VROSkinner> skinner) {
        _skinner = std::move(skinner);
    }
    
    std::vector<std::shared_ptr<VROGeometrySource>> getGeometrySourcesForSemantic(VROGeometrySourceSemantic semantic) const;

    void setInstancedUBO(std::shared_ptr<VROInstancedUBO> instancedUBO) {
        _instancedUBO = instancedUBO;
    }

    const std::shared_ptr<VROInstancedUBO> &getInstancedUBO() const{
        return _instancedUBO;
    }
protected:
    
    void setSources(std::vector<std::shared_ptr<VROGeometrySource>> sources) {
        _geometrySources = sources;
        updateSubstrate();
    }
    void setElements(std::vector<std::shared_ptr<VROGeometryElement>> elements) {
        _geometryElements = elements;
        updateSubstrate();
    }
    
private:
    /*
     User-assigned name of this geometry.
     */
    std::string _name;
    
    /*
     The materials, which define the surface appearance (color, lighting, texture, and effects)
     of each geometry element.
     
     If a geometry has the same number of materials as it has geometry elements, the material 
     index corresponds to the element index. For geometries with fewer materials than elements,
     the material index for each element is determined by calculating the index of that element
     modulo the number of materials. For example, in a geometry with six elements and three materials, 
     the element at index 5 is rendered using the material at index 5 % 3 = 2.
     */
    std::vector<std::shared_ptr<VROMaterial>> _materials;
    std::vector<std::shared_ptr<VROGeometrySource>> _geometrySources;
    std::vector<std::shared_ptr<VROGeometryElement>> _geometryElements;
    
    /*
     Used for sorting the elements prior to rendering.
     */
    std::vector<VROSortKey> _sortKeys;
    
    /*
     True if this geometry is a camera enclosure, e.g. a skybox. Camera enclosures follow
     the camera and ignore interlens distance (since they generally simulate far away objects).
     */
    bool _cameraEnclosure;
    
    /*
     True if this geometry's coordinates are specified in screen space; e.g., the coordinates of
     the viewport. If true, then the geometry will be rendered orthographically (ignoring perspective)
     and with an identity view matrix (ignoring the camera). The geometry may still have 
     node transforms.
     */
    bool _screenSpace;
    
    /*
     The bounding box of this geometry.
     */
    VROBoundingBox _bounds;

    /*
     Atomic version of the bounding box. Not used by the rendering thread.
     */
    std::atomic<VROBoundingBox> _lastBounds;

    /*
     True if the bounding box for this VROGeometry has been computed.
     */
    bool _boundingBoxComputed;
    
    /*
     Representation of this geometry in the underlying graphics library.
     */
    VROGeometrySubstrate *_substrate;
    
    /*
     The skinner ties this geometry to a skeleton, enabling skeletal animation.
     */
    std::unique_ptr<VROSkinner> _skinner;

    /*
     If this geometry has no source data installed (_geometrySources and _geometryElements),
     then returns false.
     */
    bool isRenderable() const;
    
    /*
     Invoke when the substrate needs to be refreshed (typically when underlying
     geometry sources or elements change).
     */
    void updateSubstrate();

    /*
     If set, this geometry is instanced rendered with the configurations set by this
     instancedUBO.
     */
    std::shared_ptr<VROInstancedUBO> _instancedUBO;
};

#endif /* VROGeometry_h */
