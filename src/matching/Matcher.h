/*
 * Matcher.h
 *
 *  Created on: 30.10.2012
 *      Author: Christian Staudt (christian.staudt@kit.edu)
 */

#ifndef MATCHER_H_
#define MATCHER_H_

#include "Matching.h"

namespace NetworKit {


class Matcher {

public:

	Matcher();

	virtual ~Matcher();

	virtual Matching run(Graph& G) = 0;
};

} /* namespace NetworKit */
#endif /* MATCHER_H_ */
