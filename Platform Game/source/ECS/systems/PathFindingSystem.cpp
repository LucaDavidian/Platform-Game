#include "PathFindingSystem.h"

PathFindingSystem::PathFindingSystem()
{
	mPathNodesMesh.LoadVertexAttribute("POSITION", nullptr, 100 * 3 *sizeof(float), 3, true);  // TODO: buffer size?
	mPathEdgesMesh.LoadVertexAttribute("POSITION", nullptr, 2 * 100 * 3 * sizeof(float), 3, true);  // TODO: buffer size?
}

Vector<glm::vec3> PathFindingSystem::ComputePath(glm::vec3 const &start, const glm::vec3 &destination)
{
	// add node to mesh
	//float v1[3] = { start.x, start.y, start.z };
	//mGraphMeshNodes.UpdateDynamicVertexAttribute("POSITION", mVisibilityGraph.Size() * 3 * sizeof(float), v1, 3 * sizeof(float));
	//
	//float v2[3] = { destination.x, destination.y, destination.z };
	//mGraphMeshNodes.UpdateDynamicVertexAttribute("POSITION", (mVisibilityGraph.Size() + 1) * 3 * sizeof(float), v2, 3 * sizeof(float));

	// add temp start and destination nodes to visibility graph
	mVisibilityGraph.AddNode(start);
	mVisibilityGraph.AddNode(destination);

	unsigned int startNodeIndex = mVisibilityGraph.Size() - 2;
	unsigned int destinationNodeIndex = mVisibilityGraph.Size() - 1;

	// visibility test for start and destination nodes - add edges to visibility graph   
	for (unsigned int nodeIndex = 0U; nodeIndex < mVisibilityGraph.Size() - 2; ++nodeIndex)
	{
		if (IsVisible(start, mVisibilityGraph.GetData(nodeIndex)))
		{
			float weight = glm::length(start - mVisibilityGraph.GetData(nodeIndex));
			mVisibilityGraph.AddEdge(startNodeIndex, nodeIndex, weight);
		}

		if (IsVisible(destination, mVisibilityGraph.GetData(nodeIndex)))
		{
			float weight = glm::length(destination - mVisibilityGraph.GetData(nodeIndex));
			mVisibilityGraph.AddEdge(destinationNodeIndex, nodeIndex, weight);
		}
	}

	// add mesh edges from start node
	unsigned int newConnections = 0U;
	for (unsigned int nodeIndex = 0U; nodeIndex < mVisibilityGraph.Size() - 2; nodeIndex++)
		if (mVisibilityGraph.Connected(startNodeIndex, nodeIndex))
		{
			glm::vec3 vertex1 = mVisibilityGraph.GetData(startNodeIndex);
			glm::vec3 vertex2 = mVisibilityGraph.GetData(nodeIndex);

			float v1[3] = { vertex1.x, vertex1.y, vertex1.z };
			float v2[3] = { vertex2.x, vertex2.y, vertex2.z };

			mGraphEdgesMesh.UpdateDynamicVertexAttribute("POSITION", (mConnections + newConnections) * 3 * sizeof(float), v1, 3 * sizeof(float));
			newConnections++;
			mGraphEdgesMesh.UpdateDynamicVertexAttribute("POSITION", (mConnections + newConnections) * 3 * sizeof(float), v2, 3 * sizeof(float));
			newConnections++;
		}

	// add mesh edges from destination node
	for (unsigned int nodeIndex = 0U; nodeIndex < mVisibilityGraph.Size() - 2; nodeIndex++)
		if (mVisibilityGraph.Connected(destinationNodeIndex, nodeIndex))
		{
			glm::vec3 vertex1 = mVisibilityGraph.GetData(destinationNodeIndex);
			glm::vec3 vertex2 = mVisibilityGraph.GetData(nodeIndex);

			float v1[3] = { vertex1.x, vertex1.y, vertex1.z };
			float v2[3] = { vertex2.x, vertex2.y, vertex2.z };

			mGraphEdgesMesh.UpdateDynamicVertexAttribute("POSITION", (mConnections + newConnections) * 3 * sizeof(float), v1, 3 * sizeof(float));
			newConnections++;
			mGraphEdgesMesh.UpdateDynamicVertexAttribute("POSITION", (mConnections + newConnections) * 3 * sizeof(float), v2, 3 * sizeof(float));
			newConnections++;
		}

	mGraphEdgesMesh.SetVertexCount(mConnections + newConnections);

	Vector<unsigned int> indexPath = mVisibilityGraph.AStar(startNodeIndex, destinationNodeIndex);

	Vector<glm::vec3> path;

	if (IsVisible(start, destination))
		path = Vector<glm::vec3>{ start, destination };
	else
	{
		for (auto nodeIndex : indexPath)
			path.InsertLast(mVisibilityGraph.GetData(nodeIndex));
	}

	unsigned int offset = 0U;
	for (auto vertex : path)
	{
		float v[3] = { vertex.x, vertex.y, vertex.z };
		mPathNodesMesh.UpdateDynamicVertexAttribute("POSITION", offset, v, 3 * sizeof(float));
		offset += 3 * sizeof(float);
	}

	mPathNodesMesh.SetVertexCount(path.Size());
	
	offset = 0U;
	for (unsigned int vertexIndex = 0U; vertexIndex < path.Size() - 1; vertexIndex++)
	{
		glm::vec3 v1 = path[vertexIndex];
		glm::vec3 v2 = path[vertexIndex + 1];
		float ver1[3] = { v1.x, v1.y, v1.z };
		float ver2[3] = { v2.x, v2.y, v2.z };
		mPathEdgesMesh.UpdateDynamicVertexAttribute("POSITION", offset, ver1, 3 * sizeof(float));
		offset += 3 * sizeof(float);
		mPathEdgesMesh.UpdateDynamicVertexAttribute("POSIITON", offset, ver2, 3 * sizeof(float));
		offset += 3 * sizeof(float);
	}

	mPathEdgesMesh.SetVertexCount(path.Size() * 2);

	// remove temp start and destination nodes
	mVisibilityGraph.RemoveNode(mVisibilityGraph.Size() - 1);
	mVisibilityGraph.RemoveNode(mVisibilityGraph.Size() - 1);

	if (!IsInside(destination, mWalkArea, false))
	{
		mPathEdgesMesh.SetVertexCount(0);
		mPathNodesMesh.SetVertexCount(0);

		path = { start };
	}
	else
		for (auto hole : mHoles)
			if (IsInside(destination, hole, false))
			{
				mPathEdgesMesh.SetVertexCount(0);
				mPathNodesMesh.SetVertexCount(0);
	
				path = { start };

				break;
			}

	return path;
}

bool PathFindingSystem::IsConcave(unsigned int index, const Vector<glm::vec3> &polygon) const
{
	glm::vec3 previousVertex = index ? polygon[index - 1] : polygon[polygon.Size() - 1];
	glm::vec3 vertex = polygon[index];
	glm::vec3 nextVertex = index < polygon.Size() - 1 ? polygon[index + 1] : polygon[0];

	return glm::cross(vertex - previousVertex, nextVertex - vertex).z < 0.0f;   // polygon vertices specified in counter-clockwise winding order
}

// raycasting (odd/even)
bool PathFindingSystem::IsInside(glm::vec3 vertex, Vector<glm::vec3> const &polygon, bool tolerance) const
{
	static const float epsilon = 0.000001f;

	bool inside = false;

	for (unsigned int vertexIndex = 0U; vertexIndex < polygon.Size() - 1; vertexIndex++)
	{
		glm::vec3 firstVertex = polygon[vertexIndex];
		glm::vec3 secondVertex = polygon[vertexIndex + 1];

		//glm::vec3 leftVertex = firstVertex;
		//glm::vec3 rightVertex = secondVertex;

		//if (firstVertex.x > secondVertex.x)
		//{
		//	leftVertex = secondVertex;
		//	rightVertex = firstVertex;
		//}

		//if (vertex.x >= leftVertex.x && vertex.x <= rightVertex.x)
		//	if ((vertex.y - firstVertex.y) >= (secondVertex.y - firstVertex.y) * (vertex.x - firstVertex.x) / (secondVertex.x - firstVertex.x))
		//		inside = !inside;

		float sqrDistance = glm::length(firstVertex - secondVertex) * glm::length(firstVertex - secondVertex);
		float firstSqrDistance = glm::length(vertex - firstVertex) * glm::length(vertex - firstVertex);
		float secondSqrDistance = glm::length(vertex - secondVertex) * glm::length(vertex - secondVertex);

		if (firstSqrDistance + secondSqrDistance + 2.0f * glm::sqrt(firstSqrDistance * secondSqrDistance) - sqrDistance < epsilon)
			return tolerance;

		if (vertex.x >= firstVertex.x != vertex.x >= secondVertex.x)
			if ((vertex.y - firstVertex.y) >= ((secondVertex.y - firstVertex.y) / (secondVertex.x - firstVertex.x)) * (vertex.x - firstVertex.x))
				inside = !inside;
	}

	float sqrDistance = glm::length(polygon[polygon.Size() - 1] - polygon[0]) * glm::length(polygon[polygon.Size() - 1] - polygon[0]);
	float firstSqrDistance = glm::length(vertex - polygon[polygon.Size() - 1]) * glm::length(vertex - polygon[polygon.Size() - 1]);
	float secondSqrDistance = glm::length(vertex - polygon[0]) * glm::length(vertex - polygon[0]);

	if (firstSqrDistance + secondSqrDistance + 2.0f * glm::sqrt(firstSqrDistance * secondSqrDistance) - sqrDistance < epsilon)
		return tolerance;

	if (vertex.x >= polygon[polygon.Size() - 1].x != vertex.x >= polygon[0].x)
		if ((vertex.y - polygon[polygon.Size() - 1].y) >= ((polygon[0].y - polygon[polygon.Size() - 1].y) / (polygon[0].x - polygon[polygon.Size() - 1].x)) * (vertex.x - polygon[polygon.Size() - 1].x))
			inside = !inside;

	return inside;
}

bool PathFindingSystem::Intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 q1, glm::vec3 q2) const
{
	float denominator = (q2 - q1).x * (p2 - p1).y - (q2 - q1).y * (p2 - p1).x;

	if (denominator == 0.0f)   // parallel lines
			return false;

	float s = ((p1 - q1).x * (p2 - p1).y - (p1 - q1).y * (p2 - p1).x) / denominator;

	float t = ((p1 - q1).x * (q2 - q1).y - (p1 - q1).y * (q2 - q1).x)  / denominator;

	if (s > 0.0001f && s < 0.9999f && t > 0.0001f && t < 0.9999f)
		return true;

	return false;
}

bool PathFindingSystem::IsVisible(glm::vec3 from, glm::vec3 to) const
{
	// segment/segment intersection with all other segments from geometry
	for (unsigned int vertexIndex = 0U; vertexIndex < mWalkArea.Size() - 1; vertexIndex++)
		if (Intersect(from, to, mWalkArea[vertexIndex], mWalkArea[vertexIndex + 1]))
			return false;

	if (Intersect(from, to, mWalkArea[mWalkArea.Size() - 1], mWalkArea[0]))
		return false;

	for (auto hole : mHoles)
	{
		for (unsigned int vertexIndex = 0U; vertexIndex < hole.Size() - 1; vertexIndex++)
			if (Intersect(from, to, hole[vertexIndex], hole[vertexIndex + 1]))
				return false;

		if (Intersect(from, to, hole[hole.Size() - 1], hole[0]))
			return false;
	}

	glm::vec3 midPoint = (from + to) / 2.0f;

	for (auto hole : mHoles)
		if (IsInside(midPoint, hole, false))
			return false;

	return IsInside(midPoint, mWalkArea, true);
}

void PathFindingSystem::UpdateVisibilityGraph()
{
	mVisibilityGraph.Clear();

	// graph - add all concave vertices
	for (unsigned int i = 0U; i < mWalkArea.Size(); ++i)
		if (IsConcave(i, mWalkArea))
			mVisibilityGraph.AddNode(mWalkArea[i]);

	for (auto hole : mHoles)
		for (unsigned int i = 0U; i < hole.Size(); ++i)
			if (!IsConcave(i, hole))  // concave test is reversed for holes (alternative: specify clockwise)
				mVisibilityGraph.AddNode(hole[i]);

	// visibility test - add edges to graph
	for (unsigned int node1Index = 0U; node1Index < mVisibilityGraph.Size(); ++node1Index)
		for (unsigned int node2Index = 0U; node2Index < mVisibilityGraph.Size(); ++node2Index)
		{
			if (node1Index == node2Index)
				continue;

			if (IsVisible(mVisibilityGraph.GetData(node1Index), mVisibilityGraph.GetData(node2Index)))
			{
				float weight = glm::length(mVisibilityGraph.GetData(node1Index) - mVisibilityGraph.GetData(node2Index));
				mVisibilityGraph.AddEdge(node1Index, node2Index, weight, true);     // directed edge (added two times)
			}
		}

	// generate visibility graph mesh - nodes
	mGraphNodesMesh.Clear();
	mGraphNodesMesh.LoadVertexAttribute("POSITION", nullptr, (mVisibilityGraph.Size() + 2) * 3 * sizeof(float), 3, true);

	unsigned int offset = 0U;
	unsigned int index = 0U;
	for (unsigned int i = 0; i < mVisibilityGraph.Size(); i++)
	{
		auto vertex = mVisibilityGraph.GetData(i);

		float v[3] = { vertex.x, vertex.y, vertex.z };
		mGraphNodesMesh.UpdateDynamicVertexAttribute("POSITION", offset, v, 3 * sizeof(float));

		offset += 3 * sizeof(float);
	}

	mGraphNodesMesh.SetVertexCount(mVisibilityGraph.Size());

	// generate visibility graph mesh - edges
	mGraphEdgesMesh.Clear();
	mGraphEdgesMesh.LoadVertexAttribute("POSITION", nullptr, ((mVisibilityGraph.Size() + 2) * (mVisibilityGraph.Size() + 2)) * 3 * sizeof(float), 3, true);

	mConnections = 0U;
	for (unsigned int nodeIndex1 = 0U; nodeIndex1 < mVisibilityGraph.Size(); nodeIndex1++)
		for (unsigned int nodeIndex2 = 0U; nodeIndex2 < mVisibilityGraph.Size(); nodeIndex2++)
			if (mVisibilityGraph.Connected(nodeIndex1, nodeIndex2))
			{
				glm::vec3 vertex1 = mVisibilityGraph.GetData(nodeIndex1);
				glm::vec3 vertex2 = mVisibilityGraph.GetData(nodeIndex2);

				float v1[3] = { vertex1.x, vertex1.y, vertex1.z };
				float v2[3] = { vertex2.x, vertex2.y, vertex2.z };

				mGraphEdgesMesh.UpdateDynamicVertexAttribute("POSITION", mConnections * 3 * sizeof(float), v1, 3 * sizeof(float));
				mConnections++;
				mGraphEdgesMesh.UpdateDynamicVertexAttribute("POSITION", mConnections * 3 * sizeof(float), v2, 3 * sizeof(float));
				mConnections++;
			}

	mGraphEdgesMesh.SetVertexCount(mConnections);
}

void PathFindingSystem::AddWalkArea(Vector<glm::vec3> const &vertices) 
{ 
	mWalkArea = vertices; 
	UpdateVisibilityGraph(); 

	// add walk area mesh
	mGeometryMeshes.InsertLast(StaticMesh());

	mGeometryMeshes.Last().LoadVertexAttribute("POSITION", nullptr, vertices.Size() * 3 * sizeof(float), 3, true);

	Vector<unsigned int> indices;

	unsigned int offset = 0U;
	unsigned int index = 0U;
	for (auto vertex : vertices)
	{
		float v[3] = { vertex.x, vertex.y, vertex.z };
		mGeometryMeshes.Last().UpdateDynamicVertexAttribute("POSITION", offset, v, 3 * sizeof(float));

		offset += 3 * sizeof(float);

		indices.InsertLast(index);
		index++;
	}

	indices.InsertLast(0);

	mGeometryMeshes.Last().LoadIndices(indices.Data(), indices.Size());
}

void PathFindingSystem::AddHole(Vector<glm::vec3> const &vertices) 
{ 
	mHoles.InsertLast(vertices); 
	UpdateVisibilityGraph(); 

	// add hole mesh
	Vector<unsigned int> indices;

	mGeometryMeshes.InsertLast(StaticMesh());

	mGeometryMeshes.Last().LoadVertexAttribute("POSITION", nullptr, vertices.Size() * 3 * sizeof(float), 3, true);

	unsigned int offset = 0U;
	unsigned int index = 0U;
	for (auto vertex : vertices)
	{
		float v[3] = { vertex.x, vertex.y, vertex.z };
		mGeometryMeshes.Last().UpdateDynamicVertexAttribute("POSITION", offset, v, 3 * sizeof(float));

		offset += 3 * sizeof(float);

		indices.InsertLast(index);
		index++;
	}

	indices.InsertLast(0);

	mGeometryMeshes.Last().LoadIndices(indices.Data(), indices.Size());
}


