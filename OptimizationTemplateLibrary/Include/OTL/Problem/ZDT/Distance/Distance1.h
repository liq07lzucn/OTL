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

#pragma once

#include <cassert>
#include <cmath>
#include <boost/math/constants/constants.hpp>

namespace otl
{
namespace problem
{
namespace zdt
{
namespace distance
{
template <typename _TReal, typename _TIterator>
_TReal Distance1(_TIterator begin, _TIterator end)
{
	const size_t nDecisions = std::distance(begin, end);
	if (nDecisions)
		return std::accumulate(begin, end, (_TReal)0) / nDecisions;
	else
		return 0;
}
}
}
}
}
