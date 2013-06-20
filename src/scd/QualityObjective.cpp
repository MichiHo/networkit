/*
 * QualityObjective.cpp

 *
 *  Created on: 16.06.2013
 *      Author: Yassine Marrakchi
 */

#include "QualityObjective.h"

namespace NetworKit {

QualityObjective::QualityObjective(Graph& G, std::unordered_set<node>& community) {
	this->G = &G;
	this->community = &community;
}

QualityObjective::~QualityObjective() {
}

LocalModularityM::LocalModularityM(Graph& G, std::unordered_set<node>& community)
	: QualityObjective(G, community){
}

LocalModularityM::~LocalModularityM() {
}

double LocalModularityM::getValue(node v) {

	double inside = 0;
	double outside = 0;
	bool modified = false;
	if (community->find(v) == community->end()) {
		modified = true;
	}
	community->insert(v);
	for (node u : (*community)) {
		this->G->forNeighborsOf(u, [&](node x){
			if (community->find(x) == community->end()){
				outside ++;
			} else {
				if (u == x) {
					inside++;
				} else {
					inside = inside + 0.5;
				}
			}
		});
	}

	if (modified == true) {
		community->erase(v);
	}

	if (outside == 0) {
		return G->numberOfEdges();
	}
	return inside / outside;
}


Conductance::Conductance(Graph& G, std::unordered_set<node>& community) : QualityObjective(G, community), degSum(0), nBoundaryEdges(0), volume(0) {
	// precompute degree sum
	this->degSum = this->G->parallelSumForNodes([&](node u){
		return this->G->degree(u);
	});
}

Conductance::~Conductance() {
}

double Conductance::getValue(node v) {
	bool modified = false;
	if (community->find(v) == community->end()) {
		modified = true;
	}
	community->insert(v);

	for (node u : (*community)) {
		volume = volume + this->G->degree(u);
		this->G->forNeighborsOf(u, [&](node v){
			if (community->find(v) == community->end()) {
				nBoundaryEdges++;
			}
		});
	}

	if (modified == true) {
		community->erase(v);
	}
	if (volume == 0 || degSum - volume == 0)
		return 0;
	return 1 - (nBoundaryEdges / std::min(volume, degSum - volume));
}

LocalModularityL::LocalModularityL(Graph& G, std::unordered_set<node>& community)
	: QualityObjective(G, community){
}

LocalModularityL::~LocalModularityL() {
}

double LocalModularityL::getValue(node v) {
	double inside = 0;
	double outside = 0;
	std::unordered_set<node> boundary;
	bool modified = false;
	if (community->find(v) == community->end()) {
		modified = true;
	}
	community->insert(v);

	for (node u : *community) {
		this->G->forNeighborsOf(u, [&](node x){
			if (community->find(x) == community->end()){
				outside ++;
				if (boundary.find(u) == boundary.end()) {
					boundary.insert(u);
				}
			} else {
				if (u == x) {
					inside++;
				} else {
					inside = inside + 0.5;
				}
			}
		});
	}
	if (modified == true) {
		community->erase(v);
	}
	return (inside / community->size()) / (outside / boundary.size());
}

}


