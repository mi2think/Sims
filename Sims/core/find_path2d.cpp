/********************************************************************
	created:	2016/09/25
	created:	25:9:2016   16:31
	filename: 	D:\Code\Sims\Sims\core\find_path2d.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	find_path2d
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Find Path2D
*********************************************************************/
#include "find_path2d.h"
#include "log.h"
#include <stack>

namespace sims
{
	/***
	 * Best_First_Search()
	 * {   
	 *     Open = [start node];
	 *     Close = [];
	 *     while(Open not empty)
	 *     {
	 *         get node x from Open, and delete it from Open
	 *         if (x is dest node)
	 *         {
	 *             eval path
	 *             return path
	 *         }
	 *         for (for each child node y of x)
	 *         {
	 *             if (y not in Open and Close)
	 *             {
	 *                 eval estimation of y
	 *                 insert y to Open
	 *             }
	 *             // not sort yet
	 *             else if (y is in Open)
	 *             {
	 *                 if (estimation of y < Open)
	 *                     update estimation of Open
	 *             }
	 *             else // y is in Close
	 *             {
	 *                 if (estimation of y < Close)
	 *                 {
	 *                     update estimation of Close
	 *                     remove it from Close, and put it to Open
	 *                 }
	 *             }
	 *             insert X to Close
	 *             sort Open by estimation of node
	 *         }
	 *     }
	 * }
	 **/

	static bool SortNode(FindPath2D::stNode* node1, FindPath2D::stNode* node2)
	{
		if (!node1 || !node2)
			return false;

		return node1->f < node2->f;
	}

	float heuristic(const Vector2i& start, const Vector2i& end)
	{
		/*	int dx = abs(start.x - end.x);
		int dy = abs(start.y - end.y);
		int D = 100;
		if (dx > dy)
		return D * dx;
		else
		return D * dy;
		*/
		return Distance(start, end);
	}

	bool FindPath2D::FindPath(const Vector2i& ptBegin, const Vector2i& ptEnd)
	{
		if (!pathJudge_ || ptBegin == ptEnd)
			return false;

		// generate start node, and put into Open
		stNode* node = new stNode;
		node->pt = ptBegin;
		node->g = 0.0f;
		node->h = heuristic(ptBegin, ptEnd);
		node->f = node->g + node->h;
		node->parent = NULL;
		memset(node->child, 0, sizeof(node->child));
		openTable_.push_back(node);

		path_.clear();
		stNode* best_node = NULL;
		for (;;)
		{
			if (openTable_.empty())
				break;
			std::sort(openTable_.begin(), openTable_.end(), SortNode);
			// get best estimation from Open
			best_node = openTable_.at(0);

			LOG_INFO("get node: %d %d %f\n", best_node->pt.x, best_node->pt.y, best_node->f);

			// find it, return
			if (best_node && best_node->pt == ptEnd)
				break;
			// else generate children

			// left top
			Vector2i pt;
			pt = Vector2i(best_node->pt.x - 1, best_node->pt.y - 1);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// top
			pt = Vector2i(best_node->pt.x, best_node->pt.y - 1);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// right top
			pt = Vector2i(best_node->pt.x + 1, best_node->pt.y - 1);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// left
			pt = Vector2i(best_node->pt.x - 1, best_node->pt.y);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// right
			pt = Vector2i(best_node->pt.x + 1, best_node->pt.y);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// left bottom
			pt = Vector2i(best_node->pt.x - 1, best_node->pt.y + 1);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// bottom
			pt = Vector2i(best_node->pt.x, best_node->pt.y + 1);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);
			// right bottom
			pt = Vector2i(best_node->pt.x + 1, best_node->pt.y + 1);
			if (pathJudge_->IsReachable(pt))
				GenerateSucc(best_node, pt, ptEnd);


			openTable_.erase(openTable_.begin());
			closeTable_.push_back(best_node);
		}

		while (best_node)
		{
			path_.push_back(best_node->pt);
			best_node = best_node->parent;
		}
		std::reverse(path_.begin(), path_.end());

		for (auto e : openTable_)
		{
			delete e;
		}
		for (auto e : closeTable_)
		{
			delete e;
		}

		return true;
	}

	void FindPath2D::GenerateSucc(stNode* best_node, const Vector2i& pt, const Vector2i& ptEnd)
	{
		stNode* old_node = NULL;
		float g = best_node->g + 1;
		int c = 0;

		// child node is in Open
		if ((old_node = FindNode(openTable_, pt)) != NULL)
		{
			for (c = 0; c < ChildNum; c++)
			{
				if (best_node->child[c] == NULL)
					break;
			}
			best_node->child[c] = old_node;

			// compare estimation of Open and current node(just compare value g)
			if (g < old_node->g)
			{
				old_node->parent = best_node;
				old_node->g = g;
				old_node->f = g + old_node->h;
			}
		}
		// child node is in Close
		else if ((old_node = FindNode(closeTable_, pt)) != NULL)
		{
			for (c = 0; c < ChildNum; c++)
			{
				if (best_node->child[c] == NULL)
					break;
			}
			best_node->child[c] = old_node;

			if (g < old_node->g)
			{
				old_node->parent = best_node;
				old_node->g = g;
				old_node->f = g + old_node->h;

				// update estimation of all children node of Old
				PropagateDown(old_node);
			}
		}
		// child node not in Open or Close
		else
		{
			// generate new node
			stNode* new_node = new stNode;
			new_node->pt = pt;
			new_node->g = g;
			new_node->h = heuristic(pt, ptEnd);
			new_node->f = new_node->g + new_node->h;
			new_node->parent = best_node;
			memset(new_node->child, 0, sizeof(new_node->child));
			// insert to Open
			openTable_.push_back(new_node);

			for (c = 0; c < ChildNum; c++)
			{
				if (best_node->child[c] == NULL)
					break;
			}
			best_node->child[c] = new_node;

			LOG_INFO("new node: %d %d %f\n", pt.x, pt.y, new_node->f);
		}
	}

	FindPath2D::stNode* FindPath2D::FindNode(vector<stNode*>& vec, const Vector2i& pt)
	{
		for (uint32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i]->pt == pt)
				return vec[i];
		}
		return NULL;
	}

	void FindPath2D::PropagateDown(stNode* node)
	{
		std::stack<stNode*>  stack_node;
		stack_node.push(node);

		while (!stack_node.empty())
		{
			stNode* tp = stack_node.top();
			stack_node.pop();

			for (int i = 0; i < ChildNum; ++i)
			{
				stNode* c = tp->child[i];
				if (c)
				{
					c->g = tp->g + 1;
					c->f = c->g + c->h;
					stack_node.push(c);
				}
			}
		}
	}

}