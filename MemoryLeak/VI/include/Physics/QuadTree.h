/*!*****************************************************************************
\file	QuadTree.h
\author Lee Hsien Wei, Joachim
\par	DP email: l.hsienweijoachim@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	30-01-2023
\brief  This file contains the declaration of the QuadTree class data struct used
		for collision spatial partitioning / broad phase
*******************************************************************************/
#pragma once

// -----------------------------
// Include files
// -----------------------------
#include "pch.h"
#include "ECS_items.h"
#include "..\ECS\ECS_components.h"

/*!*****************************************************************************
\brief QuadBox class that encapsulates information about an entity's or a
		quadrant's general bounding box
*******************************************************************************/

class QuadBox {
public:
	/*!*****************************************************************************
	\brief
	Quadbox class default constructor that initializes the parameters of the general
	bounding box
	\param const Math::Vec2&
	A reference to a read-only variable containing the center
	\param const Math::Vec2&
	A reference to a read-only variable containing the scale
	\param const Math::Vec2&
	A reference to a read-only variable containing the center offset
	\param const Math::Vec2&
	A reference to a read-only variable containing the scale offset
	*******************************************************************************/
	QuadBox(const Math::Vec2& _center = Math::Vec2{0.f, 0.f}, const Math::Vec2& _scale = Math::Vec2{0.f, 0.f}, const Math::Vec2& _centerOffset = Math::Vec2{0.f, 0.f}, const Math::Vec2& _scaleOffset = Math::Vec2{0.f, 0.f}) :
		_mCenter(_center), _mScale(_scale), _mCenterOffset(_centerOffset), _mScaleOffset(_scaleOffset) {
	}

	/*!*****************************************************************************
	\brief
	Quadbox class overload constructor that initializes the parameters of the general
	bounding box based on a given entity's dimensions
	\param const Entity&
	A reference to a read-only entity
	*******************************************************************************/
	QuadBox(const Entity& _entity) {
		if (_entity.HasComponent<RectCollider>()) {
			this->_mCenter = _entity.GetComponent<Transform>().translation;
			this->_mScale = _entity.GetComponent<Transform>().scale * 2.f;
			this->_mCenterOffset = _entity.GetComponent<RectCollider>().centerOffset;
			this->_mScaleOffset = _entity.GetComponent<RectCollider>().scaleOffset;
		}
		else if (_entity.HasComponent<CircleCollider>()) {
			this->_mCenter = _entity.GetComponent<Transform>().translation;
			this->_mScale = _entity.GetComponent<Transform>().scale * 2.f;
			this->_mCenterOffset = _entity.GetComponent<CircleCollider>().centerOffset;
			this->_mScaleOffset = Math::Vec2{ _entity.GetComponent<CircleCollider>().scaleOffset, _entity.GetComponent<CircleCollider>().scaleOffset };
		}
	}
	
	/*!*****************************************************************************
	\brief
	Quadbox class GetLeft function that gets the quadbox's smallest x-axis value
	\param void
	NULL
	\return float
	Smallest x-axis value
	*******************************************************************************/
	float GetLeft() const {
		return _mCenter.x + _mCenterOffset.x - fabs(_mScale.x * _mScaleOffset.x) / 2.f;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetRight function that gets the quadbox's largest x-axis value
	\param void
	NULL
	\return float
	Largest x-axis value
	*******************************************************************************/
	float GetRight() const {
		return _mCenter.x + _mCenterOffset.x + fabs(_mScale.x * _mScaleOffset.x) / 2.f;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetTop function that gets the quadbox's largest y-axis value
	\param void
	NULL
	\return float
	Largest y-axis value
	*******************************************************************************/
	float GetTop() const {
		return _mCenter.y + _mCenterOffset.y + fabs(_mScale.y * _mScaleOffset.y) / 2.f;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetBottom function that gets the quadbox's smallest y-axis value
	\param void
	NULL
	\return float
	Smallest y-axis value
	*******************************************************************************/
	float GetBottom() const {
		return _mCenter.y + _mCenterOffset.y - fabs(_mScale.y * _mScaleOffset.y) / 2.f;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetCenterOffsetted function that gets the quadbox's center vector
	modified by its center offset
	\param void
	NULL
	\return Math::Vec2
	Modified center vector
	*******************************************************************************/
	Math::Vec2 GetCenterOffsetted() const {
		return _mCenter + _mCenterOffset;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetScaleOffsetted function that gets the quadbox's scale vector
	modified by its scale offset
	\param void
	NULL
	\return Math::Vec2
	Modified scale vector
	*******************************************************************************/
	Math::Vec2 GetScaleOffsetted() const {
		Math::Vec2 _actualScale{ _mScale };
		_actualScale.x *= _mScaleOffset.x;
		_actualScale.y *= _mScaleOffset.y;
		return _actualScale;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetCenter function that gets the quadbox's center vector
	\param void
	NULL
	\return Math::Vec2
	Center vector
	*******************************************************************************/
	Math::Vec2 GetCenter() const {
		return _mCenter;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class SetCenter function that sets the quadbox's center to the given
	vector
	\param const Math::Vec2&
	A reference to read-only vectir containing the new center
	\return void
	NULL
	*******************************************************************************/
	void SetCenter(const Math::Vec2& _newCenter) {
		_mCenter = _newCenter;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetCenter function that gets the quadbox's scale vector
	\param void
	NULL
	\return Math::Vec2
	Scale vector
	*******************************************************************************/
	Math::Vec2 GetScale() const {
		return _mScale;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class SetScale function that sets the quadbox's scale to the given
	vector
	\param const Math::Vec2&
	A reference to read-only vectir containing the new scale
	\return void
	NULL
	*******************************************************************************/
	void SetScale(const Math::Vec2& _newScale) {
		_mScale = _newScale;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetCenterOffset function that gets the modifier to the quadbox's
	center vector
	\param void
	NULL
	\return Math::Vec2
	Modifer vector to center vector
	*******************************************************************************/
	Math::Vec2 GetCenterOffset() const {
		return _mCenterOffset;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class SetCenterOffset function that sets the quadbox's center offset to
	the given vector
	\param const Math::Vec2&
	A reference to read-only vector containing the new center offset
	\return void
	NULL
	*******************************************************************************/
	void SetCenterOffset(const Math::Vec2& _newCenterOffset) {
		_mCenterOffset = _newCenterOffset;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetScaleOffset function that gets the modifier to the quadbox's
	scale vector
	\param void
	NULL
	\return Math::Vec2
	Modifer vector to scale vector
	*******************************************************************************/
	Math::Vec2 GetScaleOffset() const {
		return _mScaleOffset;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class SetScaleOffset function that sets the quadbox's scale offset to
	the given vector
	\param const Math::Vec2&
	A reference to read-only vector containing the new scale offset
	\return void
	NULL
	*******************************************************************************/
	void SetScaleOffset(const Math::Vec2& _newScaleOffset) {
		_mScaleOffset = _newScaleOffset;
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetTopLeft function that returns the vector to the quadbox's top
	left corner
	\param void
	NULL
	\return Math::Vec2
	Top left corner position vector
	*******************************************************************************/
	Math::Vec2 GetTopLeft() const {
		return Math::Vec2{ GetLeft(), GetTop() };
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetTopRight function that returns the vector to the quadbox's top
	right corner
	\param void
	NULL
	\return Math::Vec2
	Top right corner position vector
	*******************************************************************************/
	Math::Vec2 GetTopRight() const {
		return Math::Vec2{ GetRight(), GetTop() };
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetBottomLeft function that returns the vector to the quadbox's
	bottom left corner
	\param void
	NULL
	\return Math::Vec2
	Bottom left corner position vector
	*******************************************************************************/
	Math::Vec2 GetBottomLeft() const {
		return Math::Vec2{ GetLeft(), GetBottom() };
	}

	/*!*****************************************************************************
	\brief
	Quadbox class GetBottomRight function that returns the vector to the quadbox's
	bottom right corner
	\param void
	NULL
	\return Math::Vec2
	Bottom right corner position vector
	*******************************************************************************/
	Math::Vec2 GetBottomRight() const {
		return Math::Vec2{ GetRight(), GetBottom() };
	}

	/*!*****************************************************************************
	\brief
	Quadbox class Contains function that checks whether a given Quadbox is within
	its boundary
	\param const QuadBox&
	A reference to read-only Quadbox to check against
	\return bool
	Evaluation of whether given quadbox is within boundary
	*******************************************************************************/
	bool Contains(const QuadBox& _box) const {
		return this->GetLeft() <= _box.GetLeft() &&
			_box.GetRight() <= this->GetRight() &&
			_box.GetTop() <= this->GetTop() &&
			_box.GetBottom() <= this->GetBottom();
	}

	/*!*****************************************************************************
	\brief
	Quadbox class Intersects function that checks whether a given Quadbox collides
	with itself
	\param const QuadBox&
	A reference to read-only Quadbox to check against
	\return bool
	Evaluation of whether given quadbox is colliding
	*******************************************************************************/
	bool Intersects(const QuadBox& _box) const {
		return !(this->GetLeft() > _box.GetRight() || this->GetRight() < _box.GetLeft() || this->GetTop() < _box.GetBottom() || this->GetBottom() > _box.GetTop());
	}

private:
	Math::Vec2 _mCenter, _mCenterOffset,	// Center position
				_mScale, _mScaleOffset;		// Scale values
};

/*!*****************************************************************************
\brief QuadTree class for spatial partitioning
*******************************************************************************/
class QuadTree {
private:
	static constexpr size_t mThreshold{ 32 },	// Max number of nodes one quad should hold before subdividing itself
							mMaxDepth{ 8 };		// Max number of inner child nodes the root can have

	/*!*****************************************************************************
	\brief QuadNode struct that holds the entities in one quadrant
	*******************************************************************************/
	struct QuadNode {
		std::array<std::unique_ptr<QuadNode>, 4> child{};		// Child nodes
		std::vector<Entity> values{ std::vector<Entity>() };	// Entities in this region
	};

	QuadBox _mQuadBox{};				// Root node
	std::unique_ptr<QuadNode> _mRoot{};	// Dimensions of the root node
public:
	/*!*****************************************************************************
	\brief
	QuadTree class default constructor
	\param void
	NULL
	*******************************************************************************/
	QuadTree(){}

	/*!*****************************************************************************
	\brief
	QuadTree class overload constructor that initializes the root node itself and its
	dimensions
	\param const QuadBox&
	A reference to read-only quadbox containing the dimension
	*******************************************************************************/
	QuadTree(const QuadBox& _nodeBox) : _mQuadBox(_nodeBox), _mRoot(std::make_unique<QuadNode>()) {

	}

	/*!*****************************************************************************
	\brief
	QuadTree class GetBox function that gets the quadtree's root dimension that has
	already been stored as a quadbox
	\param void
	NULL
	\return QuadBox
	Dimensions of the root node
	*******************************************************************************/
	QuadBox GetBox() const {
		return this->_mQuadBox;
	}

	/*!*****************************************************************************
	\brief
	QuadTree class SetBox function that set the quadtree's quadbox dimensions to
	the given box
	\param const QuadBox&
	A reference to read-only quadbox containing the dimensions
	\return void
	NULL
	*******************************************************************************/
	void SetBox(const QuadBox& _box) {
		this->_mQuadBox = _box;
	}

	/*!*****************************************************************************
	\brief
	QuadTree class isLeaf function that evaluates whether a given node is a leaf
	\param const QuadNode*
	const Pointer to read-only node to check
	\return QuadBox
	Dimensions of the root node
	*******************************************************************************/
	bool isLeaf(const QuadNode* _node) const {
		return !static_cast<bool>(_node->child[0]);
	}

	/*!*****************************************************************************
	\brief
	QuadTree class ClearNode function that resets the data in a node
	\param QuadNode*
	Pointer to node to reset data
	\return void
	NULL
	*******************************************************************************/
	void ClearNode(QuadNode* _node) {
		if (isLeaf(_node)) 
			_node->values.clear();
		else {
			for (size_t i{ 0 }; i < _node->child.size(); ++i) {
				ClearNode(_node->child[i].get());
				_node->child[i].reset();
			}
		}
	}

	/*!*****************************************************************************
	\brief
	QuadTree class ClearQuadtree overload function that resets the quad tree data by
	calling the previous ClearNode function recursively into the root node's childs
	\param void
	NULL
	\return void
	NULL
	*******************************************************************************/
	void ClearQuadTree() {
		ClearNode(_mRoot.get());
		_mRoot.get()->values.clear();
	}

	/*!*****************************************************************************
	\brief
	QuadTree class UpdateDimensions function that checks through the entity set to
	obtain the game world's dimensions and check if the quadtree encompasses it. If
	not, it will update its dimensions
	\param const std::set<Entity> &
	A reference to updated entity set to check against
	\return void
	NULL
	*******************************************************************************/
	void UpdateDimensions(const std::set<Entity>& _entitySet) {
		Math::Vec2 worldMin{}, worldMax{};
		for (const auto& _entity : _entitySet) {
			if (!_entity.ShouldRun())
				continue;
					
			Math::Vec2 _entityPos{ _entity.GetComponent<Transform>().translation }, _entityScale{ _entity.GetComponent<Transform>().scale };
			if (_entity.HasComponent<RectCollider>()) {
				_entityPos += _entity.GetComponent<RectCollider>().centerOffset;
				_entityScale.x *= _entity.GetComponent<RectCollider>().scaleOffset.x;
				_entityScale.y *= _entity.GetComponent<RectCollider>().scaleOffset.y;
			}
			else if (_entity.HasComponent<CircleCollider>()) {
				_entityPos += _entity.GetComponent<CircleCollider>().centerOffset;
				_entityScale *= _entity.GetComponent<CircleCollider>().scaleOffset;
			}
			else
				continue;

			_entityScale.x = fabs(_entityScale.x);
			_entityScale.y = fabs(_entityScale.y);
			Math::Vec2 _entityMax{ _entityPos + _entityScale / 2.f }, _entityMin{ _entityPos - _entityScale / 2.f };

			worldMax.x = std::max(_entityMax.x, worldMax.x);
			worldMax.y = std::max(_entityMax.y, worldMax.y);

			worldMin.x = std::min(_entityMin.x, worldMin.x);
			worldMin.y = std::min(_entityMin.y, worldMin.y);
		}

		QuadBox dimension{ (worldMax + worldMin) / 2.f,
							(worldMax - worldMin) * 2.f };

		if (!_mQuadBox.Contains(dimension)) {
			_mQuadBox.SetCenter(dimension.GetCenter());
			_mQuadBox.SetScale(dimension.GetScale());
			_mQuadBox.SetCenterOffset(dimension.GetCenterOffset());
			_mQuadBox.SetScaleOffset(dimension.GetScaleOffset());
		}
	}

	/*!*****************************************************************************
	\brief
	QuadTree class UpdateQuadTree function that first clears its data before it
	inserts the entities into sorted format
	\param const std::set<Entity> &
	A reference to updated entity set to insert
	\return void
	NULL
	*******************************************************************************/
	void UpdateQuadTree(const std::set<Entity>& _entitySet) {
		ClearQuadTree();

		for (auto const& _entity : _entitySet) {
			if (_entity.ShouldRun())
				if (_entity.HasComponent<RectCollider>() || _entity.HasComponent<CircleCollider>())
					AddNode(_entity);
		}
	}

	/*!*****************************************************************************
	\brief
	QuadTree class AddNode function that attempts to add a given entity into the
	given node by recursively checking its boundary with and/or the node's childs
	\param QuadNode*
	A pointer to the node attempting to insert into
	\param size_t
	Variable containing current depth
	\param const QuadBox&
	A reference to read-only quadbox containing node's dimension
	\param const Entity&
	A reference to read-only entity to insert
	\return void
	NULL
	*******************************************************************************/
	void AddNode(QuadNode* _node, size_t _depth, const QuadBox& _box, const Entity& _entity) {
		//ASSERT(_node != nullptr);
		//ASSERT(_box.Contains(mGetBox(_entity));
		
		if (isLeaf(_node)) {
			// Insert the value in this node if possbile
			if (_depth >= mMaxDepth || _node->values.size() < mThreshold)
				_node->values.emplace_back(_entity);
			// Otherwise spilt and try again
			else {
				Spilt(_node, _box);
				AddNode(_node, _depth, _box, _entity);
			}
		} 
		else {
			QuadBox entityBox{_entity};
			int i{ GetQuadrant(_box, entityBox) };
			if (i != -1)
				AddNode(_node->child[i].get(), _depth + 1, ComputeQuadrantDimensions(_box, i), _entity);
			else
				_node->values.emplace_back(_entity);
		}
	}

	/*!*****************************************************************************
	\brief
	QuadTree class AddNode overload function that attempts to add a given entity into
	the quadtree by recursively calling the previous function
	\param const Entity&
	A reference to read-only entity to insert
	\return void
	NULL
	*******************************************************************************/
	void AddNode(const Entity& _entity) {
		AddNode(_mRoot.get(), 0, _mQuadBox, _entity);
	}

	/*!*****************************************************************************
	\brief
	QuadTree class RemoveNode function that attempts to remove a given entity from
	the given node by checking its boundary with and/or the node's childs
	\param QuadNode*
	A pointer to the node attempting to remove
	\param const QuadBox&
	A reference to read-only quadbox containing node's dimension
	\param const Entity&
	A reference to read-only entity to remove
	\return bool
	Evaluation of whether the entity was successfully removed
	*******************************************************************************/
	bool RemoveNode(QuadNode* _node, const QuadBox& _box, const Entity& _entity) {
		//ASSERT(_node != nullptr);
		//ASSERT(_box.contains(mGetBox(value)));
		if (isLeaf(_node))
		{
			// Remove the value from node
			RemoveValue(_node, _entity);
			return true;
		}
		else
		{
			// Remove the value in a child if the value is entirely contained in it
			QuadBox entityBox{ _entity };
			int i{ GetQuadrant(_box, entityBox) };
			if (i != -1){
				if (RemoveNode(_node->child[i].get(), ComputeQuadrantDimensions(_box, i), _entity))
					return TryMerge(_node);
			}
			// Otherwise, we remove the value from the current node
			else
				RemoveValue(_node, _entity);
			return false;
		}
	}
	
	/*!*****************************************************************************
	\brief
	QuadTree class RemoveNode overload function that attempts to remove a given
	entity from the quadtree by recursively calling the previous function
	\param const Entity&
	A reference to read-only entity to remove
	*******************************************************************************/
	void RemoveNode(const Entity& _entity) {
		RemoveNode(_mRoot.get(), _mQuadBox, _entity);
	}

	/*!*****************************************************************************
	\brief
	QuadTree class Query function that queries for a list of entities that fall within
	the given query box
	\param QuadNode*
	A pointer to the node
	\param const QuadBox&
	A reference to read-only quadbox containing node's dimension
	\param const QuadBox&
	A reference to read-only quadbox representing the query region
	\return void
	NULL
	*******************************************************************************/
	void Query(QuadNode* _node, const QuadBox& _box, const QuadBox& _queryBox, std::vector<Entity>& _resultList) const {
		//ASSERt(node != nullptr);
		//ASSERT(_queryBox.Intersects(_box));
		for (const auto& entity : _node->values){
			QuadBox entityBox{ entity };
			if (_queryBox.Intersects(entityBox))
				_resultList.emplace_back(entity);
		}
		if (!isLeaf(_node)){
			for (size_t i{ 0 }; i < _node->child.size(); ++i){
				QuadBox childBox{ ComputeQuadrantDimensions(_box, static_cast<int>(i)) };
				if (_queryBox.Intersects(childBox))
					Query(_node->child[i].get(), childBox, _queryBox, _resultList);
			}
		}
	}
	
	/*!*****************************************************************************
	\brief
	QuadTree class Query overload function that queries for a list of entities that
	fall within	the given query box by recursively calling the previous function
	\param const QuadBox&
	A reference to read-only quadbox representing the query region
	\return std::vector<Entity>
	Container with list of entities within the query box
	*******************************************************************************/
	std::vector<Entity> Query(const QuadBox& _box) const {
		std::vector<Entity> result{};
		Query(_mRoot.get(), _mQuadBox, _box, result);
		return result;
	}

	/*!*****************************************************************************
	\brief
	QuadTree class FindAllIntersectionInDescendants function that finds possible
	pairwise intersections within a node's subtree with a given entity
	\param QuadNode*
	A pointer to the node to check
	\param const Entity&
	A reference to read-only entity to check against
	\param std::vector<std::pair<Entity, Entity>>&
	A reference to the result container to add results to
	\return void
	NULL
	*******************************************************************************/
	void FindAllIntersectionsInDescendants(QuadNode* _node, const Entity& _entity, std::vector<std::pair<Entity, Entity>>& _resultList) const {
		// Test against the values stored in this node
		QuadBox entityBox{ _entity };
		for (const auto& otherEntity : _node->values){
			QuadBox otherEntityBox{ otherEntity };
			if (entityBox.Intersects(otherEntityBox))
				_resultList.emplace_back(_entity, otherEntity);
		}

		// Test against values stored into descendants of this node
		if (!isLeaf(_node)){
			for (const auto& child : _node->child)
				FindAllIntersectionsInDescendants(child.get(), _entity, _resultList);
		}
	}

	/*!*****************************************************************************
	\brief
	QuadTree class FindAllIntersections function that finds possible intersections
	of a given node. It first goes through the subtree, finding intersections within
	descendants. Once done, it looks for intersections in its child.
	\param QuadNode*
	A pointer to the node to check
	\param const Entity&
	A reference to read-only entity to check against
	\param std::vector<std::pair<Entity, Entity>>&
	A reference to the result container to add results to
	\return void
	NULL
	*******************************************************************************/
	void FindAllIntersections(QuadNode* _node, std::vector<std::pair<Entity, Entity>>& _resultList) const {
		// Find intersections between values stored in this node
		// Make sure to not report the same intersection twice
		for (size_t i{ 0 }; i < _node->values.size(); ++i){
			for (size_t j{ 0 }; j < i; ++j){
				QuadBox entityBox1{ _node->values[i] }, entityBox2{ _node->values[j] };
				if (entityBox1.Intersects(entityBox2))
					_resultList.emplace_back(_node->values[i], _node->values[j]);
			}
		}

		if (!isLeaf(_node)){
			// Values in this node can intersect values in descendants
			for (const auto& child : _node->child){
				for (const auto& entity : _node->values)
					FindAllIntersectionsInDescendants(child.get(), entity, _resultList);
			}

			// Find intersections in children
			for (const auto& child : _node->child)
				FindAllIntersections(child.get(), _resultList);
		}
	}
	
	/*!*****************************************************************************
	\brief
	QuadTree class FindAllIntersections overload function that finds possible
	intersections of the quadtree by recursively calling the previous function on
	the root node.
	\param void
	NULL
	\return std::vector<std::pair<Entity, Entity>>
	Container containing the possible pairwise intersections
	*******************************************************************************/
	std::vector<std::pair<Entity, Entity>> FindAllIntersections() const {
		std::vector<std::pair<Entity, Entity>> result{};
		FindAllIntersections(_mRoot.get(), result);
		return result;
	}

	/*!*****************************************************************************
	\brief
	QuadTree class ComputeQuadrantDimensions function that computes a quadrant's
	dimensions based on a larger quadbox and the given quadrant index
	\param void
	NULL
	\return QuadBox
	Quadbox containing dimensions of the requested quadrant
	*******************************************************************************/
	QuadBox ComputeQuadrantDimensions(const QuadBox& _box, int _quadrant) const {
		Math::Vec2 newScale{ _box.GetScale() / 2.f };

		switch (_quadrant) {
		case 0: // NW
			return QuadBox(Math::Vec2{ _box.GetCenter().x - newScale.x / 2.f, _box.GetCenter().y + newScale.y / 2.f }, newScale);
			break;
		case 1:	// NE
			return QuadBox(Math::Vec2{ _box.GetCenter().x + newScale.x / 2.f, _box.GetCenter().y + newScale.y / 2.f }, newScale);
			break;
		case 2:	// SW
			return QuadBox(Math::Vec2{ _box.GetCenter().x - newScale.x / 2.f, _box.GetCenter().y - newScale.y / 2.f }, newScale);
			break;
		case 3:	// SE
			return QuadBox(Math::Vec2{ _box.GetCenter().x + newScale.x / 2.f, _box.GetCenter().y - newScale.y / 2.f }, newScale);
			break;
		default:
			//ASSERT(_quadrant > 3);
			return QuadBox(Math::Vec2{}, Math::Vec2{});
			break;
		}
	}

	/*!*****************************************************************************
	\brief
	QuadTree class GetQuadrant function that finds which quadrant a given quadbox
	is within a node's quadrant
	\param const QuadBox&
	A reference to read-only reference quadbox
	\param const QuadBox&
	A reference to read-only query quadbox to find
	\return int
	Quadrant index of which quadrant the queried quadbox lies in
	*******************************************************************************/
	int GetQuadrant(const QuadBox& _nodeBox, const QuadBox& _valueBox) const {
		Math::Vec2 center{ _nodeBox.GetCenter() };
		// West
		if (_valueBox.GetRight() < center.x)
		{
			// North West
			if (_valueBox.GetBottom() < center.y)
				return 0;
			// South West
			else if (_valueBox.GetTop() >= center.y)
				return 2;
			// Not contained in any quadrant
			else
				return -1;
		}
		// East
		else if (_valueBox.GetLeft() >= center.x)
		{
			// North East
			if (_valueBox.GetBottom() < center.y)
				return 1;
			// South East
			else if (_valueBox.GetTop() >= center.y)
				return 3;
			// Not contained in any quadrant
			else
				return -1;
		}
		// Not contained in any quadrant
		else
			return -1;
	}

	/*!*****************************************************************************
	\brief
	QuadTree class Spilt function that breaks up a node quadrant into 4 smaller nodes
	\param QuadNode*
	A pointer to the node to spilt
	\param const QuadBox&
	A reference to read-only quadbox containing the node's dimensions
	\return void
	NULL
	*******************************************************************************/
	void Spilt(QuadNode* _node, const QuadBox& _box) {
		//ASSERT(_node != nullptr);
		//ASSERT(isLeaf(_node));

		// Create children
		for (auto& child : _node->child)
			child = std::make_unique<QuadNode>();

		// Assign value to child
		std::vector<Entity> initialValues{ std::vector<Entity>() };
		for (const auto& entity : _node->values) {
			QuadBox entityBox{entity};
			auto i{ GetQuadrant(_box, entityBox) };
			if (i != -1)
				_node->child[i]->values.emplace_back(entity);
			else
				initialValues.emplace_back(entity);
		}
		_node->values = std::move(initialValues);
	}

	/*!*****************************************************************************
	\brief
	QuadTree class RemoveValue function that removes a given entity from a node if
	found
	\param QuadNode*
	A pointer to the node to spilt
	\param const Entity&
	A reference to read-only entity to remove from the node
	\return void
	NULL
	*******************************************************************************/
	void RemoveValue(QuadNode* _node, const Entity& _entity) {
		// Find value in node values container
		auto it{ std::find_if(std::begin(_node->values), 
							  std::end(_node->values), 
							  [this, &_entity](const Entity& _rhs) {
								return _entity.id == _rhs.id; 
	    					  }) };
		////ASSERT(it != std::end(_node->values));
		//BREAKPOINT(it == std::end(_node->values));


		//// Swap with the last element and pop back
		//std::swap(*it, _node->values.back());
		//_node->values.pop_back();
		if (it != std::end(_node->values))
			_node->values.erase(it);
	}

	/*!*****************************************************************************
	\brief
	QuadTree class TryMerge function that merges 4 childs from a parent node into itself
	\param QuadNode*
	A pointer to the node that is the parent whose childs should merge
	\return bool
	Evaluation of whether it succeeded
	*******************************************************************************/
	bool TryMerge(QuadNode* _node) {
		//ASSERT(_node != nullptr);
		//ASSERT(!isLeaf(_node));
		size_t newSize{ _node->values.size()};
		for (const auto& child : _node->child){
			if (!isLeaf(child.get()))
				return false;
			newSize += child->values.size();
		}
		if (newSize <= mThreshold){
			_node->values.reserve(newSize);
			
			// Merge the values of all the child
			for (const auto& child : _node->child){
				for (const auto& entity : child->values)
					_node->values.emplace_back(entity);
			}
			// Remove the child
			for (auto& child : _node->child)
				child.reset();
			return true;
		}
		else
			return false;
	}

	//void InOrderPrint(QuadNode* _node) {
	//	if (_node != nullptr) {
	//		InOrderPrint(_node->child[0].get());
	//		InOrderPrint(_node->child[1].get());
	//		for (size_t i{ 0 }; i < _node->values.size(); ++i)
	//			std::cout << _node->values[i].id << " ";
	//		InOrderPrint(_node->child[2].get());
	//		InOrderPrint(_node->child[3].get());
	//	}
	//	std::cout << std::endl;
	//}
};