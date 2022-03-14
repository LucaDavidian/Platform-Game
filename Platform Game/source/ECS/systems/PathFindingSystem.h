#ifndef PATHFINDING_SYSTEM_H
#define PATHFINDING_SYSTEM_H

#include <glm/glm.hpp>
#include "../../data structures/graph.hpp"
#include "StaticMesh.h"

class PathFindingSystem
{
public:
	static PathFindingSystem &GetInstance() { static PathFindingSystem instance; return instance; }

	void AddWalkArea(Vector<glm::vec3> const &vertices);
	void AddHole(Vector<glm::vec3> const &vertices);

	Vector<glm::vec3> const &GetWakArea() const { return mWalkArea; }
	Vector<Vector<glm::vec3>> const &GetHoles() const { return mHoles; }
	Graph<glm::vec3> const &GetVisibilityGraph() const { return mVisibilityGraph; }

	Vector<glm::vec3> ComputePath(glm::vec3 const &start, const glm::vec3 &destination);

	Vector<StaticMesh> &GetMeshes() { return mGeometryMeshes; }
	StaticMesh &GetGraphNodes() { return mGraphNodesMesh; }
	StaticMesh &GetGraphEdges() { return mGraphEdgesMesh; }
	StaticMesh &GetPathNodes() { return mPathNodesMesh; }
	StaticMesh &GetPathEdges() { return mPathEdgesMesh; }


	void Update()
	{
		for (Entity *entity : Scene::GetInstance().GetEntities())
		{
			if (entity->HasComponents < PathfindingComponent, PlayerFSMComponent())
			{
				// get path edge and move FSM
			}
		}
	}

private:
	PathFindingSystem();

	Vector<glm::vec3> mWalkArea;
	Vector<Vector<glm::vec3>> mHoles;

	bool IsInside(glm::vec3 vertex, Vector<glm::vec3> const &polygon, bool tolerance) const;
	bool IsConcave(unsigned int index, const Vector<glm::vec3> &polygon) const;
	bool Intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 q1, glm::vec3 q2) const;
	bool IsVisible(glm::vec3 from, glm::vec3 to) const;
	void UpdateVisibilityGraph();

	Graph<glm::vec3> mVisibilityGraph;

	Vector<StaticMesh> mGeometryMeshes;

	StaticMesh mGraphNodesMesh;
	StaticMesh mGraphEdgesMesh;

	StaticMesh mPathNodesMesh;
	StaticMesh mPathEdgesMesh;

	unsigned int mConnections;
};

#endif // PATHFINDING_SYSTEM_H
