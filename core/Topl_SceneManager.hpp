// More Complex types
#ifndef TOPL_SCENE_MANAGER_H

#include "native_os_def.h" // This includes the Rasteron.h header, TODO Fix This!!!

#include <vector>
#include <map>
#include <string>

#include "Timer.hpp"
#include "physics_def.h"

#include "Geo_Component.hpp"

typedef const Geo_Component* const topl_geoComponent_cptr;
typedef std::pair<const Geo_Component*, const Geo_Component*> geoComponent_pair;

struct LinkedItems { // Wrapper around a physics connector and the two objects being linked
	Phys_Connector* connector;
	geoComponent_pair linkedItems;
};

typedef const LinkedItems* const topl_linkedItems_cptr;

#ifdef RASTERON_H
	typedef std::pair<unsigned, const Rasteron_Image*> idToImage_pair;
#endif

class Topl_Camera {
public:
	// Identity projection constructor
	Topl_Camera() {
		projMatrix = Eigen::Matrix4f::Identity(); // No transformation by default
	}
	// Customizable projection matrix constructor
	Topl_Camera(double angle, double ratio, double nearZ, double farZ) {
		projMatrix.row(0).col(0) << 1.0 / (angle * ratio * tan(angle / 2.0));
		projMatrix.row(1).col(1) << 1.0 / (tan(angle / 2.0));
		projMatrix.row(2).col(2) << (-1.0 * nearZ - farZ) / (nearZ - farZ);
		projMatrix.row(2).col(3) << (2.0 * nearZ * farZ) / (nearZ - farZ);
		projMatrix.row(3).col(2) << 1.0;
	}
	vec3f_cptr getPos() const { return &pos; }
	vec3f_cptr getDirection() const { return &direction; }
	mat4f_cptr getProjMatrix() const { return &projMatrix; }
private:
	Eigen::Vector3f pos = Eigen::Vector3f(0.0, 0.0, -1.0);
	Eigen::Vector3f direction = Eigen::Vector3f(0.0, 0.0, 0.0);
	Eigen::Matrix4f projMatrix = Eigen::Matrix4f::Zero();
};

typedef const Topl_Camera* const topl_camera_cptr;

// Scene Manager is essentially the singleton game object, everything passes through here to be renedered to the screen
// --------------------------------------------------------------------------------------------------------------------

class Topl_SceneManager {
public:
	Topl_SceneManager() {
		mPhysTicker.reset(); // Resets timer for dynamic scene manager operations
	}
	~Topl_SceneManager() {}

	void addGeometry(const std::string& name, Geo_Component* geoComponent);
#ifdef RASTERON_H
	void addTexture(const std::string& name, const Rasteron_Image* rstnImage);
#endif
	void addForce(const std::string& name, const Eigen::Vector3f& vec);
	void addPhysics(const std::string& name, Phys_Properties* pProp);
	void addConnector(Phys_Connector* connector, const std::string& name1, const std::string& name2);
	void modConnector(const std::string& targetName, Eigen::Vector3f rotAnglesVec, double lengthScale); // Rotates and scales all connectors associated with named geometry
	void remConnector(const std::string& targetName); // Breaks all connectors associated with named geometry
	void resolvePhysics(); // Iterates through all appropriate members in mIdToPhysProp_map

	unsigned getGeoCount() const { return mNamedGeos.size(); }
	topl_geoComponent_cptr getGeoComponent(unsigned index) const; // Access to geometry by index
	topl_geoComponent_cptr getGeoComponent(const std::string& name) const; // Access to geometry by name

	unsigned getLinkedItemsCount() const { return mLinkedItems.size(); }
	topl_linkedItems_cptr getLink(unsigned index) const; // Access to links sequentially

	topl_camera_cptr getCamera() const { return &mCamera; }
#ifdef RASTERON_H
	const Rasteron_Image* getFirstTexture(unsigned index) const; // Remove!
	const Rasteron_Image* getFirstTexture(const std::string& name) const;
	unsigned getTextures(unsigned index, const Rasteron_Image** images) const; // Sequential access, see MAX_BUFFERS_PER_TARGET in Renderer.hpp
#endif
private:
	std::vector<Geo_Component*> mNamedGeos; // Stores all geometries
	std::map<Geo_Component*, Phys_Properties*> mGeoPhys_map; // Associates geometry to a physics structure
	std::vector<LinkedItems> mLinkedItems; // Stores geometry connector data
	Timer_Ticker mPhysTicker; // This ticker is specific to physics updates

	Topl_Camera mCamera;
#ifdef RASTERON_H
	std::map<Geo_Component*, const Rasteron_Image*> mGeoTex_map; // Associates geometry to a single texture structure
#endif
};

#define TOPL_SCENE_MANAGER_H
#endif