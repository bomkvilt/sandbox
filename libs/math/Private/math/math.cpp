#include "math/math.hpp"
#include <cmath>


namespace Math
{
	float  Sqrt(float  x) 				{ return sqrtf(x); 		}
	double Sqrt(double x) 				{ return sqrt (x); 		}

	float  Cos(float  x) 				{ return cosf(x); 		}
	double Cos(double x) 				{ return cos (x); 		}
	float  Sin(float  x) 				{ return sinf(x); 		}
	double Sin(double x) 				{ return sin (x); 		}
	float  Tan(float  x) 				{ return tanf(x); 		}
	double Tan(double x) 				{ return tan (x); 		}

	float  Acos(float  x) 				{ return acosf(x); 		}
	double Acos(double x) 				{ return acos (x); 		}
	float  Asin(float  x) 				{ return asinf(x); 		}
	double Asin(double x) 				{ return asin (x); 		}
	float  Atan(float  x) 				{ return atanf(x); 		}
	double Atan(double x) 				{ return atan (x); 		}
	float  Atan2(float  x, float  y) 	{ return atan2f(x, y); 	}
	double Atan2(double x, double y) 	{ return atan2 (x, y); 	}

	float  Abs(float  x) 				{ return fabsf(x); 		}
	double Abs(double x) 				{ return std::abs  (x);	}
	float  Max(float  x, float  y) 		{ return fmaxf(x, y); 	}
	float  Min(float  x, float  y) 		{ return fminf(x, y); 	}
	double Max(double x, double y) 		{ return std::max(x, y); }
	double Min(double x, double y) 		{ return std::min(x, y); }
}
