/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <vector>
#include <list>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/constants/constants.hpp>
#include <OTL/Indicator/Epsilon/AdditiveEpsilon.h>
#include <OTL/Indicator/Epsilon/MultiplicativeEpsilon.h>

template <typename _TReal>
std::list<std::vector<_TReal> > GenerateCirclePoints(const _TReal radius, const size_t nPoints)
{
	typedef std::vector<_TReal> _TPoint;
	std::list<_TPoint> population;
	for (size_t i = 0; i < nPoints; ++i)
	{
		const _TReal angle = i * boost::math::constants::pi<_TReal>() / 2 / (nPoints - 1);
		std::vector<_TReal> point(2);
		point[0] = radius * cos(angle);
		point[1] = radius * sin(angle);
		population.push_back(point);
	}
	return population;
}

BOOST_AUTO_TEST_CASE(AdditiveEpsilon)
{
	typedef double _TReal;
	typedef otl::indicator::epsilon::AdditiveEpsilon<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	typedef _TIndicator::TFront _TFront;
	const std::list<_TPoint> _pf = GenerateCirclePoints<_TReal>(1, 10000);
	const _TFront pf(_pf.begin(), _pf.end());
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(2, 100);
	const _TFront front(_front.begin(), _front.end());
	_TIndicator indicator(pf);
	indicator(front);
}

BOOST_AUTO_TEST_CASE(MultiplicativeEpsilon)
{
	typedef double _TReal;
	typedef otl::indicator::epsilon::MultiplicativeEpsilon<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	typedef _TIndicator::TFront _TFront;
	const std::list<_TPoint> _pf = GenerateCirclePoints<_TReal>(1, 10000);
	const _TFront pf(_pf.begin(), _pf.end());
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(2, 100);
	const _TFront front(_front.begin(), _front.end());
	_TIndicator indicator(pf);
	indicator(front);
}
