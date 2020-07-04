#include <initializer_list>
#include <utility>
#include <cstdlib> // May be redundant
#include <vector> // May be redundant
#include <string>

#include "Geometry.hpp"
#include "Timer.hpp"

#include "Topl_SceneManager.hpp"

// Geometry wrapper class that can manage its states

typedef std::pair<std::string, Topl_GeoNode*> geoName_pair;

class Geo_Construct {
public:
    Geo_Construct(const std::string& prefix, Topl_SceneManager* sGraph, std::initializer_list<Geo_RenderObj*> renderObjs) {
		mNodeData = (Topl_GeoNode**)malloc(renderObjs.size() * sizeof(Topl_GeoNode*));
        for(std::initializer_list<Geo_RenderObj*>::iterator currentRenderObj = renderObjs.begin(); currentRenderObj < renderObjs.end(); currentRenderObj++){
            *(mNodeData + mNodeCount) = new  Topl_GeoNode(*(currentRenderObj));
            mNodeCount++;
        }
        // fillSceneManager(sGraph);
	}
	~Geo_Construct() { 
        for (unsigned g = 0; g < mNodeCount; g++)
			delete *(mNodeData + g);
        free(mNodeData);
    }
	virtual void updateSceneManager(Topl_SceneManager* sGraph) = 0;
protected:
	virtual void fillSceneManager(Topl_SceneManager* sGraph) = 0;
    std::string genUniqueName();

	std::vector<geoName_pair> mNamedNodes;
	unsigned mNodeCount = 0;
	Topl_GeoNode** mNodeData;

}; // Needs work, could use more abstraction

class Geo_CircleUp : public Geo_Construct {
public:
	Geo_CircleUp(const std::string& prefix, Topl_SceneManager* sGraph) : 
    Geo_Construct(prefix, sGraph, { (Geo_RenderObj*)&sphere1, (Geo_RenderObj*)&sphere2, (Geo_RenderObj*)&sphere3 }) 
		{ fillSceneManager(sGraph); }
	
    void updateSceneManager(Topl_SceneManager* sGraph) override;
private:
	void fillSceneManager(Topl_SceneManager* sGraph) override;

	Geo_Sphere2D sphere1 = Geo_Sphere2D(0.1f, 4);
	Geo_Sphere2D sphere2 = Geo_Sphere2D(0.4f, 200);
	Geo_Sphere2D sphere3 = Geo_Sphere2D(0.333f, 42);
};


Eigen::Vector3f updatePosAbs(const Eigen::Vector3f& pos);

class Geo_RandShapes {
public:
    Geo_RandShapes(Topl_SceneManager* sceneGraph){
        mBox1 = new Geo_Rect2D(0.8f, 0.6f);
        mGeoNode1 = new Topl_GeoNode((Geo_RenderObj*)mBox1);

        mBox2 = new Geo_Rect2D(0.2f, 0.3f);
        mGeoNode2 = new Topl_GeoNode((Geo_RenderObj*)mBox2);
        mGeoNode2->updatePos(Eigen::Vector3f(0.6f, -0.55f, 0.0f));

		mSphere1 = new Geo_Sphere2D(0.2f, 20);
        mGeoNode3 = new Topl_GeoNode((Geo_RenderObj*)mSphere1);
        mGeoNode3->updatePos(Eigen::Vector3f(-0.2f, -0.4f, 0.0f));

        fillSceneManager(sceneGraph);
    }
    ~Geo_RandShapes(){
        delete mBox1;
        delete mGeoNode1;
        delete mBox2;
        delete mGeoNode2;
        delete mSphere1;
        delete mGeoNode3;
    }
    // void updateSceneManager(Topl_SceneManager* Topl_SceneManager, Timer_DiscreteEvent timerEvent); // Should probably make private
private:
    void fillSceneManager(Topl_SceneManager* sceneGraph);
    
    Geo_Rect2D* mBox1;
    Geo_Rect2D* mBox2;
	Geo_Sphere2D* mSphere1;
    Topl_GeoNode* mGeoNode1;
    Topl_GeoNode* mGeoNode2;
    Topl_GeoNode* mGeoNode3;
    // Physics_MoveAbs mUpMovement = Physics_MoveAbs(&updatePosAbs);
};

void createQuadTree(Topl_SceneManager* sceneGraph, unsigned divs);
