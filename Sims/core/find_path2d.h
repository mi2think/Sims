/********************************************************************
	created:	2016/09/25
	created:	25:9:2016   16:17
	filename: 	D:\Code\Sims\Sims\core\find_path2d.h
	file path:	D:\Code\Sims\Sims\core
	file base:	find_path2d
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Find Path2D
*********************************************************************/
#ifndef __FIND_PATH2D_H__
#define __FIND_PATH2D_H__

#include "sims.h"
#include "math/vector2.h"

namespace sims
{
	class IPathJudge
	{
	public:
		virtual ~IPathJudge() {}
		virtual bool IsReachable(const Vector2i& pt) const = 0;
	};

	// A star
	class FindPath2D
	{
	public:
		FindPath2D() : pathJudge_(nullptr) {}

		enum { ChildNum = 8 };

		struct stNode
		{
			Vector2i pt;
			float f;		// f = g + h
			float g;
			float h;
			stNode* parent;
			stNode* child[ChildNum];
		};

		void SetPathJudge(IPathJudge* judge) { pathJudge_ = judge; }

		bool FindPath(const Vector2i& ptBegin, const Vector2i& ptEnd);

		void GenerateSucc(stNode* best_node, const Vector2i& pt, const Vector2i& ptEnd);

		stNode* FindNode(vector<stNode*>& vec, const Vector2i& pt);

		void PropagateDown(stNode* node);

		const vector<Vector2i>& GetPath() const { return path_; }
	private:
		IPathJudge* pathJudge_;

		vector<stNode*>	openTable_;
		vector<stNode*>	closeTable_;;
		vector<Vector2i> path_;
	};
}

#endif