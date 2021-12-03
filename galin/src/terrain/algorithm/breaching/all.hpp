#pragma once

#include "terrain/all.hpp"

#include <queue>

namespace breaching {

// Neighbour Directions
const int DIRECTIONS = 8;
const int dx[DIRECTIONS] = { -1, -1,  0,  1, 1, 1, 0, -1 };
const int dy[DIRECTIONS] = {  0, -1, -1, -1, 0, 1, 1,  1 };

using Point = std::array<std::size_t, 2>;

using Element = std::pair<float, Point>;

struct comp {
  // Compare elevation, then x, then y
  bool operator()(const Element& a, const Element& b) {
	const Point& lhs = a.second;
	const Point& rhs = a.second;

	if(a.first > b.first) return true;
	else if((a.first == b.first) && (lhs[0] > rhs[0] || (lhs[0] == rhs[0] && lhs[1] > rhs[1])))
	  return true;
	else return false;
  }
};

using ZPriorityQueue = std::priority_queue< Element, std::vector<Element>, comp >;

enum LindsayMode {
  COMPLETE_BREACHING,
  SELECTIVE_BREACHING,
  CONSTRAINED_BREACHING
};

enum LindsayCellType {
  UNVISITED,
  VISITED,
  EDGE
};


/*!
\brief Breach depressions.

Depression breaching drills a path from a depression's pit cell (its lowest
point) along the least-cost (Priority-Flood) path to the nearest cell
outside the depression to have the same or lower elevation.

See https://rbarnes.org/

See https://github.com/r-barnes/richdem

See https://github.com/r-barnes/richdem/blob/master/include/richdem/depressions/Lindsay2016.hpp

\author John Lindsay, implementation by Richard Barnes (rbarnes@umn.edu).
*/
void breach(Terrain& t) {
	auto h = value_accessor(t.height);

	int mode = COMPLETE_BREACHING;
	bool fill_depressions = false;

	// Heightfield properties
	const Box2 box = Array2::GetBox();

	const int NO_BACK_LINK = -1;

	ArrayInteger2 backlinks(box, nx, ny, NO_BACK_LINK);
	ArrayInteger2 visited(box, nx, ny, LindsayCellType::UNVISITED);
	ArrayInteger2 pits(box, nx, ny, false);

	std::vector<int> flood_array;
	ZPriorityQueue pq;

	int total_pits = 0;

	// Seed the priority queue
	for(int x = 0; x < nx; x++)
	for(int y = 0; y < ny; y++) {
		// Valid edge cells go on priority-queue
		if(x == 0 || x == (nx - 1) || y == 0 || y == (ny - 1)) {
			pq.emplace(h(x, y), Point(x, y));
			visited(x, y) = LindsayCellType::EDGE;
			continue;
		}

		// Determine if this is an edge cell, gather information used to determine if it is a pit cell
		float lowest_neighbour = std::numeric_limits<float>::max();
		for(int n = 0; n < DIRECTIONS; n++) {
			const int px = x + dx[n];
			const int py = y + dy[n];

			// No need for an inGrid check here because edge cells are filtered above
			// Used for identifying the lowest neighbour
			lowest_neighbour = std::min(h(px, py), lowest_neighbour);
		}

		// This is a pit cell if it is lower than any of its neighbours. In this
		// case: raise the cell to be just lower than its lowest neighbour. This
		// makes the breaching/tunneling procedures work better.
		if(h(x, y) < lowest_neighbour) {
			//operator()(x, y) = nextafter(lowest_neighbour, -2.0 * HeightField::flat/*std::numeric_limits<float>::lowest()*/);
			operator()(x, y) = lowest_neighbour - 2.0 * HeightField::flat;
		}
		// Since depressions might have flat bottoms, we treat flats as pits. Mark 
		// flat/pits as such now.
		if(h(x, y) <= lowest_neighbour) {
			pits(x, y) = true;
			total_pits++; // May not need this
		}
	}

	// The Priority-Flood operation assures that we reach pit cells by passing into
	// depressions over the outlet of minimal elevation on their edge.
	while (!pq.empty()) {
		const auto c = pq.top();
		pq.pop();

		const float elevation = c.first;
		const Point p = c.second;

		//This cell is a pit: let's consider doing some breaching
		if(pits(p[0], p[1])) {
			//Locate a cell that is lower than the pit cell, or an edge cell
			int cc = VertexIndex(p);                                  //Current cell on the path
			const float pathdepth = -2.0 * HeightField::flat; // std::numeric_limits<float>::lowest(); //Maximum depth found along the path
			float target_height = h(p[0], p[1]);                 //Depth to which the cell currently being considered should be carved

			if(mode == COMPLETE_BREACHING) {
				//Trace path back to a cell low enough for the path to drain into it, or
				//to an edge of the DEM
				while (cc != NO_BACK_LINK && at(cc) >= target_height) {
					field[cc] = target_height;
					cc = backlinks.at(cc);                     //Follow path back
					//target_height = nextafter(target_height, pathdepth); //Decrease target depth slightly for each cell on path to ensure drainage
					target_height = target_height - 2.0 * HeightField::flat; //Decrease target depth slightly for each cell on path to ensure drainage
				}
			}
			--total_pits;
			if(total_pits == 0) {
				break;
			}
		}

		//Looks for neighbours which are either unvisited or pits
		for(int n = 0; n < DIRECTIONS; n++) {
			const int px = p[0] + dx[n];
			const int py = p[1] + dy[n];

			if(!InsideVertexIndex(px, py)) {
				continue;
			}
			if(visited(px, py) != LindsayCellType::UNVISITED) {
				continue;
			}

			const float my_e = h(px, py);

			//The neighbour is unvisited. Add it to the queue
			pq.emplace(my_e, Point(px, py));
			if(fill_depressions) {
				flood_array.emplace_back(VertexIndex(px, py));
			}
			visited(px, py) = LindsayCellType::VISITED;
			backlinks(px, py) = VertexIndex(p);
		}

		if(mode != COMPLETE_BREACHING && fill_depressions) {
			for(const auto f : flood_array) {
				int parent = backlinks.at(f);
				if(at(f) <= at(parent)) {
					field[f] = nextafter(at(parent), std::numeric_limits<float>::max());
				}
			}
		}
	}
}

}
