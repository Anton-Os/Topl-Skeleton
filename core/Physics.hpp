#include <cstdlib>
#include <cmath>
#include <vector>

#include <Eigen/Dense>

// TODO: ADD DEFINITIONS TO SCALE PHYSICS REACTIONS

#define TOPL_FORCE_UNIT 0.04
#define TOPL_DEFAULT_DAMPING 0.9999
#define TOPL_DEFAULT_MASS 7.0
#define TOPL_DEFAULT_K 4000.0

enum CONNECT_Type {
    CONNECT_Spring, // Oscillating force both ways
    CONNECT_Bungee, // Exerts pulling force, no push
    CONNECT_String, // Fixed maximum distance apart
    CONNECT_Rod, // Fixed distance apart
    CONNECT_Cable // Fixed min and max distance apart
};

struct Phys_Connector {
	Eigen::Vector3f centerPoint = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f restAngleVec1, restAngleVec2; // Normalized vectors that point to first and second linked items respectively AT REST!
    Eigen::Vector3f angleVec1, angleVec2; // Normalized vectors that point to first and second linked items respectively UPDATED!

    double length = 0.5f; // Tries to reach rest length from here
    double restLength = 0.5f; // Zero forces act at this length
	double angle = 0.0f; // Tries to reach rest angle from here

    CONNECT_Type type = CONNECT_Rod;
	double kVal = TOPL_DEFAULT_K; // 100.0 seems to be normal
    // double deviation = 0.1f;
};

#define MAX_PHYS_FORCES 64

struct Phys_Properties { // This binds to a Geo_Component
    Phys_Properties(){
        forces = (Eigen::Vector3f*)malloc(MAX_PHYS_FORCES * sizeof(Eigen::Vector3f));
        // Add the gravity force here, it will always be acting on the body
    }
    ~Phys_Properties(){ if(forces != nullptr) free(forces); }

	const double damping = TOPL_DEFAULT_DAMPING;
    double mass = TOPL_DEFAULT_MASS;

	Eigen::Vector3f velocity = Eigen::Vector3f(0.0, 0.0, 0.0);
    Eigen::Vector3f acceleration = Eigen::Vector3f(0.0, 0.0, 0.0);

    unsigned actingForceCount = 0; // Indicates the gravity force
    Eigen::Vector3f* forces = nullptr;
};