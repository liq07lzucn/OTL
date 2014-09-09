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

#include <cassert>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/constants/constants.hpp>
#include <OTL/Problem/DTLZ/DTLZ2.h>
#include <OTL/Initial/UniformReal.h>
#include <OTL/Crossover/SimulatedBinaryCrossover.h>
#include <OTL/Crossover/DifferentialEvolution.h>
#include <OTL/Crossover/CoupleCoupleCrossoverAdapter.h>
#include <OTL/Crossover/XTripleCrossoverAdapter.h>
#include <OTL/Mutation/PolynomialMutation.h>
#include <OTL/Optimizer/NSGA-II/NSGA-II.h>

BOOST_AUTO_TEST_CASE(CoupleCoupleCrossover)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	_TRandom random;
	const size_t populationSizeList[] = {1, 2, 3, 4, 100, 101};
	for (size_t i = 0; i < sizeof(populationSizeList) / sizeof(*populationSizeList); ++i)
	{
		const size_t populationSize = populationSizeList[i];
		_TProblem problem(nObjectives);
		_TCrossover _crossover(random, 1, problem.GetBoundary(), 20);
		otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
		_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
		const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), populationSize);
		_TOptimizer optimizer(random, problem, initial, crossover, mutation);
		BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
		for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
		{
			optimizer();
			BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
		}
	}
}

BOOST_AUTO_TEST_CASE(XTripleCrossover)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::DifferentialEvolution<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	_TRandom random;
	const size_t populationSizeList[] = {1, 2, 3, 4, 5, 6, 100, 101, 102};
	for (size_t i = 0; i < sizeof(populationSizeList) / sizeof(*populationSizeList); ++i)
	{
		const size_t populationSize = populationSizeList[i];
		_TProblem problem(nObjectives);
		const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), populationSize);
		_TCrossover _crossover(random, 1, problem.GetBoundary());
		otl::crossover::XTripleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
		_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
		_TOptimizer optimizer(random, problem, initial, crossover, mutation);
		BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
		for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
		{
			optimizer();
			BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
		}
	}
}
