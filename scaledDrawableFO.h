#include "scaledDrawable.h"

class ScaledDrawableFO {
public:
	bool operator()(const ScaledDrawable *lhs, const ScaledDrawable *rhs) const{
		return(lhs->getScale() < rhs->getScale());
	}
};
