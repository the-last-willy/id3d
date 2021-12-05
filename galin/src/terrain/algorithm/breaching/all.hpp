#pragma once

#include "terrain/resolution.hpp"
#include "terrain/terrain.hpp"

#include <cmath>
#include <limits>
#include <queue>
#include <utility>

namespace breaching {

// Neighbour Directions
const int DIRECTIONS = 8;
const int dx[DIRECTIONS] = { -1, -1,  0,  1, 1, 1, 0, -1 };
const int dy[DIRECTIONS] = {  0, -1, -1, -1, 0, 1, 1,  1 };
// const int DIRECTIONS = 4;
// const int dx[DIRECTIONS] = { -1,  0,  0,  1 };
// const int dy[DIRECTIONS] = {  0, -1,  1,  0 };

using Point = std::array<std::size_t, 2>;

using Element = std::pair<float, Point>;

struct comp {
  // Compare elevation, then x, then y
  bool operator()(const Element& a, const Element& b) {
	const Point& lhs = a.second;
	const Point& rhs = a.second;

	if(a.first > b.first) {
		return true;
	} else if((a.first == b.first) && (lhs[0] > rhs[0] || (lhs[0] == rhs[0] && lhs[1] > rhs[1])))
	  return true;
	else return false;
  }
};

using ZPriorityQueue = std::priority_queue< Element, std::vector<Element>, comp >;

inline
float previous(float f) {
	return std::nextafter(f, std::numeric_limits<float>::lowest());
}

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

	auto nx = resolution(t)[0];
	auto ny = resolution(t)[1];

	auto backlinks = agl::common::grid<int>(nx, ny);
	for(auto& b : backlinks) {
		b = -1;
	}
	auto pits = agl::common::grid<char>(nx, ny);
	for(auto it = pits.begin(); it != pits.end(); ++it) {
		*it = false;
	}
	auto visited = agl::common::grid<char>(nx, ny);
	for(auto it = visited.begin(); it != visited.end(); ++it) {
		*it = false;
	}

	ZPriorityQueue pq;

	// Seed the priority queue
	for(std::size_t x = 0; x < nx; x++)
	for(std::size_t y = 0; y < ny; y++) {
		// Valid edge cells go on priority-queue
		if(x == 0 || x == (nx - 1) || y == 0 || y == (ny - 1)) {
			pq.emplace(std::make_pair(h(x, y), std::array<std::size_t, 2>{x, y}));
			at(visited, x, y) = true;
			continue;
		}

		// Determine if this is an edge cell, gather information used to determine if it is a pit cell
		float lowest_neighbour = std::numeric_limits<float>::max();
		for(std::size_t n = 0; n < DIRECTIONS; n++) {
			auto px = x + dx[n];
			auto py = y + dy[n];

			// No need for an inGrid check here because edge cells are filtered above
			// Used for identifying the lowest neighbour
			lowest_neighbour = std::min(h(px, py), lowest_neighbour);
		}

		// This is a pit cell if it is lower than any of its neighbours. In this
		// case: raise the cell to be just lower than its lowest neighbour. This
		// makes the breaching/tunneling procedures work better.
		if(h(x, y) < lowest_neighbour) {
			h(x, y) = previous(lowest_neighbour);
		}
		
		// Since depressions might have flat bottoms, we treat flats as pits. Mark 
		// flat/pits as such now.
		if(h(x, y) <= lowest_neighbour) {
			at(pits, x, y) = true;
		}
	}

	// The Priority-Flood operation assures that we reach pit cells by passing into
	// depressions over the outlet of minimal elevation on their edge.
	while (!pq.empty()) {
		auto c = pq.top();
		pq.pop();

		// float elevation = c.first;
		Point p = c.second;

		//This cell is a pit: let's consider doing some breaching
		if(at(pits, p[0], p[1])) {
			//Locate a cell that is lower than the pit cell, or an edge cell
			auto cc = at(backlinks, p[0], p[1]);                                  //Current cell on the path
			float target_height = previous(h(p[0], p[1]));                 //Depth to which the cell currently being considered should be carved

			//Trace path back to a cell low enough for the path to drain into it, or
			//to an edge of the DEM
			while(cc != -1 && at(t.height.values, cc) >= target_height) {
				at(t.height.values, cc) = target_height;
				cc = at(backlinks, cc); //Follow path back
				target_height = previous(target_height); //Decrease target depth slightly for each cell on path to ensure drainage
			}
		}

		//Looks for neighbours which are unvisited
		for(int n = 0; n < DIRECTIONS; n++) {
			auto px = p[0] + dx[n];
			auto py = p[1] + dy[n];

			if(px >= nx or py >= ny) {
				continue;
			}
			if(at(visited, px, py)) {
				continue;
			}
			//The neighbour is unvisited. Add it to the queue
			pq.push(std::make_pair(h(px, py), std::array{px, py}));
			at(visited, px, py) = true;
			at(backlinks, px, py) = int(at(indexing(backlinks), p[0], p[1]));
		}
	}
}

}
