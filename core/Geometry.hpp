#ifndef GEOMETRY_H

#include <cstdlib>
#include <cmath>

#include <vector>
#include <map>
#include <string> // For the scenegraph

#define TOPL_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406
#include <Eigen/Dense> // Examine documentation

// Simple Primitive Types

struct Rect {
    float width;
    float height;
};

struct Circle {
    float radius;
    unsigned segments;
};

// Renderable primitive types

// Typedefs for "safe" types
typedef const Eigen::Vector3f* const vec3f_cptr;
typedef const unsigned* const ui_cptr;

class Geo_RenderObj {
public:
    Geo_RenderObj(){}
    ~Geo_RenderObj(){
        if(mVData != nullptr) free(mVData);
        if(mIData != nullptr) free(mIData);
    }

    unsigned getVCount() const { return mVCount; }
    unsigned getICount() const { return mICount; }

    vec3f_cptr getVData() const { return mVData; }
    ui_cptr getIData() const { return mIData; }
protected:
    // Eigen::Vector3f vec; // Work plz
    virtual Eigen::Vector3f* genVertices() = 0;
    virtual unsigned* genIndices() = 0;

    const unsigned short mPerVertex = 3; // Elements per vertex, should be configurable later
    unsigned mVCount;
    unsigned mICount;

    Eigen::Vector3f* mVData = nullptr;
	unsigned* mIData = nullptr;
};

// Override the virtual functions above
class Geo_Rect2D : public Geo_RenderObj {
public:
    Geo_Rect2D(float width, float height) : Geo_RenderObj() {
        mVCount = 4; // Rectangle has 4 vertices
        mICount = 6; // Rectangle has 6 indices
        mRect.width = width;
        mRect.height = height;

        mVData = genVertices();
        mIData = genIndices();
    }
private:
    Eigen::Vector3f* genVertices() override;
    unsigned* genIndices() override;
    //unsigned color;
    int mDrawOrder = 0;
    Rect mRect;
};

class Geo_Sphere2D : Geo_RenderObj {
    Geo_Sphere2D(float radius, unsigned segments) : Geo_RenderObj() {
        mVCount = segments + 1; // Number of segments + center point
        mICount = segments * 3; // Rectangle has 6 indices
        mCircle.radius = radius;
        mCircle.segments = segments;

        mVData = genVertices();
        mIData = genIndices();
    }
private:
    Eigen::Vector3f* genVertices() override;
    unsigned* genIndices() override;
    //unsigned color;
    int mDrawOrder = 0;
    Circle mCircle;
};



// More Complex types

struct Topl_BaseEntity { // Acts as a node
	Topl_BaseEntity() {
		mId_count++;
		mId = mId_count;
	}
	~Topl_BaseEntity() { mId_count--; }

	// const unsigned const* mId_ref = &mId;
	unsigned getId() const { return mId; }
    Topl_BaseEntity* getParent() const { return mParent; }
    Topl_BaseEntity* getChild(unsigned childNum) const {
        if(childNum > mChildCount) return nullptr;
        else return *(mChild + childNum - 1);
    }

    vec3f_cptr getLocation() const { return &mRelWorldPos; }
	void updateLocation(Eigen::Vector3f vec) { mRelWorldPos += vec; }; // Follow by more spatial update things
    // ADD CHILD FUNCTION
private:
	static unsigned mId_count; // Grows/shrinks when objects are created/deleted
	unsigned mId; // Each object has a unique id

    Topl_BaseEntity* mParent = nullptr;
    unsigned mChildCount = 0;
    Topl_BaseEntity** mChild = nullptr;

    // Eigen::Vector3f mRelWorldPos = Eigen::Vector3f(0.0, 0.0, 0.0); // Positions by which to offset
	Eigen::Vector3f mRelWorldPos = Eigen::Vector3f(0.43, 0.3, 0.0); // Testing only!!!
    Eigen::Vector3f mObjOrientAngl = Eigen::Vector3f(0.0, 0.0, 0.0); // Angles by which to rotate
};

struct Topl_GeoEntity : Topl_BaseEntity {
    Topl_GeoEntity(const Geo_RenderObj* renderObj){ mRenderObj = renderObj; }
	Topl_GeoEntity(const Geo_Rect2D* geoObj) { mType.gRect = geoObj; }
    Topl_GeoEntity(const Geo_Sphere2D* geoObj) { mType.gSphere = geoObj; }
    // A unique constructor must exist for every geometry type
    enum GeoBehavior {
        GEO_Fixed = 1,
        GEO_Dynamic = 2
    } mBehavior;
    union GeoType {
        const Geo_Rect2D* gRect;
        const Geo_Sphere2D* gSphere;
        // ADD MORE TYPES OF GEOMETRY
    } mType;

    const Geo_RenderObj* mRenderObj; // Trying to eliminate the mType
};


typedef const Topl_GeoEntity* const tpl_gEntity_cptr;

class Topl_SceneGraph {
public:
    Topl_SceneGraph(){}
    ~Topl_SceneGraph(){}

    void addGeometry(const std::string& name, tpl_gEntity_cptr geoEntity);
    tpl_gEntity_cptr getGeoEntity(unsigned index) const;
    tpl_gEntity_cptr getGeoEntity(const std::string& name) const;
    // void addTextures(const char* name, const Topl_Texture** textures);
    // void addShaders(const char* name, const Topl_Shader** shaders);
private:
    // unsigned mKeyCount = 0;
    std::map<std::string, unsigned> mNameToId_map; // Associates names to object by IDs
    std::map<unsigned, tpl_gEntity_cptr> mIdToGeo_map;
    // std::map<unsigned, const bool*> mIdToUpdateStat; // DOES OBJECT REQUIRE UPDATING
    // std::map<unsigned, const Topl_Texture**> mIdToTextures_map // WILL MAP TEXTURES TO OBJECTS
    // std::map<unsigned, const Topl_Shader**> mIdToShaders_map // WILL MAP SHADERS TO OBJECTS
};

#define GEOMETRY_H
#endif
