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
#include <utility>
#include <vector>
#include <numeric>
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace grea
{
size_t GridDifference(const std::vector<size_t> &gridCoordinate1, const std::vector<size_t> &gridCoordinate2);

template <typename _TReal>
std::pair<_TReal, _TReal> ExpandHalfBox(const _TReal min, const _TReal max, const size_t dividion)
{
	assert(min <= max);
	std::pair<_TReal, _TReal> range;
	const _TReal width = max - min;
	const _TReal boxWidth = width / dividion;
	range.first = min - boxWidth / 2; //grid lower boundary
	range.second = (width + boxWidth) / dividion; //hyperbox width
	assert(range.second >= 0);
	return range;
}

template <typename _TReal>
std::vector<size_t> GridCoordinate(const std::vector<std::pair<_TReal, _TReal> > &ranges, const std::vector<_TReal> &objective)
{
	assert(ranges.size() == objective.size());
	std::vector<size_t> gridCoordinate(objective.size());
	for (size_t i = 0; i < gridCoordinate.size(); ++i)
	{
		auto &range = ranges[i];
		assert(range.first < objective[i]);
		assert(range.second >= 0);
		gridCoordinate[i] = (size_t)((objective[i] - range.first) / range.second);
	}
	return gridCoordinate;
}

template <typename _TReal>
std::vector<_TReal> GridCoordinatePoint(const std::vector<std::pair<_TReal, _TReal> > &ranges, const std::vector<size_t> &gridCoordinate)
{
	assert(ranges.size() == gridCoordinate.size());
	std::vector<_TReal> point(gridCoordinate.size());
	for (size_t i = 0; i < point.size(); ++i)
	{
		auto &range = ranges[i];
		assert(range.second >= 0);
		point[i] = range.first + gridCoordinate[i] * range.second;
	}
	return point;
}

template <typename _TReal, typename _TDecision>
_TReal GridCoordinatePointDistance(const std::vector<std::pair<_TReal, _TReal> > &ranges, const Individual<_TReal, _TDecision> &individual)
{
	const std::vector<_TReal> point = GridCoordinatePoint(ranges, individual.gridCoordinate_);
	assert(individual.objective_.size() == point.size());
	_TReal gcpd = 0;
	for (size_t i = 0; i < point.size(); ++i)
	{
		const _TReal diff = point[i] - individual.objective_[i];
		gcpd += diff * diff / ranges[i].second;
	}
	return sqrt(gcpd);
}

template <typename _TIterator>
_TIterator LocateBest(_TIterator begin, _TIterator end)
{
	_TIterator best = begin;
	for (_TIterator i = ++begin; i != end; ++i)
	{
		auto &individual = **i;
		if (individual.gr_ < (**best).gr_)
			best = i;
		else if (individual.gr_ == (**best).gr_)
		{
			if (individual.gcd_ < (**best).gcd_)
				best = i;
			else if (individual.gcd_ == (**best).gcd_)
			{
				if (individual.gcpd_ < (**best).gcpd_)
					best = i;
			}
		}
	}
	return best;
}

template <typename _TReal, typename _TDecision, typename _TIterator>
void AdjustGCD(const Individual<_TReal, _TDecision> &elite, _TIterator begin, _TIterator end)
{
	for (_TIterator i = begin; i != end; ++i)
	{
		Individual<_TReal, _TDecision> &individual = **i;
		const size_t gd = GridDifference(elite.gridCoordinate_, individual.gridCoordinate_);
		if (gd < elite.gridCoordinate_.size())
			(**i).gcd_ += elite.gridCoordinate_.size() - gd;
	}
}

template <typename _TReal, typename _TDecision, typename _TIterator, typename _TDominate>
void AdjustPunishmentDegree(const Individual<_TReal, _TDecision> &elite, _TIterator begin, _TIterator end, _TDominate dominate)
{
	for (_TIterator i = begin; i != end; ++i)
		(**i).pd_ = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		Individual<_TReal, _TDecision> &individual = **i;
		const size_t gd = GridDifference(elite.gridCoordinate_, individual.gridCoordinate_);
		if (gd < elite.gridCoordinate_.size())
		{
			const size_t gcd = elite.gridCoordinate_.size() - gd;
			if (individual.pd_ < gcd)
			{
				individual.pd_ = gcd;
				for (_TIterator j = begin; j != end; ++j)
				{
					Individual<_TReal, _TDecision> &_individual = **j;
					if (dominate(individual, _individual))
					{
						if (_individual.pd_ < gcd)
							_individual.pd_ = gcd;
					}
				}
			}
		}
	}
	for (_TIterator i = begin; i != end; ++i)
		(**i).gr_ += (**i).pd_;
}

template <typename _TReal, typename _TDecision, typename _TPointer, typename _TDominate>
void AdjustGR(const Individual<_TReal, _TDecision> &elite, std::list<_TPointer> &population, _TDominate dominate)
{
	std::list<_TPointer> weakDominated;
	for (auto i = population.begin(); i != population.end();)
	{
		Individual<_TReal, _TDecision> &individual = **i;
		if (elite.gridCoordinate_ == individual.gridCoordinate_)
		{
			individual.gr_ += individual.gridCoordinate_.size() + 2;
			auto move = i;
			++i;
			weakDominated.splice(weakDominated.end(), population, move);
		}
		else if (dominate(elite, individual))
		{
			individual.gr_ += individual.gridCoordinate_.size();
			auto move = i;
			++i;
			weakDominated.splice(weakDominated.end(), population, move);
		}
		else
			++i;
	}
	AdjustPunishmentDegree(elite, population.begin(), population.end(), dominate);
	population.splice(population.end(), weakDominated, weakDominated.begin(), weakDominated.end());
}
}
}
}
