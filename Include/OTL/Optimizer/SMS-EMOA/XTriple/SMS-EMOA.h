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

#include <OTL/Crossover/WithXTripleCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Optimizer/SMS-EMOA/SMS-EMOA.h>

namespace otl
{
namespace optimizer
{
namespace sms_emoa
{
namespace xtriple
{
template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
class SMS_EMOA : public otl::optimizer::sms_emoa::SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>, public otl::crossover::WithXTripleCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef _TMakeHypervolume TMakeHypervolume;
	typedef otl::optimizer::sms_emoa::SMS_EMOA<TReal, TDecision, TRandom, TMakeHypervolume> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithXTripleCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;

	SMS_EMOA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, _TMakeHypervolume makeHypervolume);
	~SMS_EMOA(void);

protected:
	void _DoStep(void);
};

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::SMS_EMOA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, _TMakeHypervolume makeHypervolume)
	: TSuper(random, problem, initial, makeHypervolume)
	, otl::crossover::WithXTripleCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::~SMS_EMOA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
void SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::_DoStep(void)
{
	for (size_t i = 0; i < TSuper::GetSolutionSet().size(); ++i)
	{
		auto selected = TSuper::MatingSelection(3, TSuper::solutionSet_);
		TIndividual child;
		this->GetCrossover()(TSuper::GetSolutionSet()[i], *selected[0], *selected[1], *selected[2], child);
		this->GetMutation()(child);
		TSuper::GetProblem()(child);

		TSolutionSet mix(TSuper::solutionSet_.size() + 1);
		std::copy(TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), mix.begin());
		mix.back() = child;
		TSuper::_Reduce(mix, TSuper::solutionSet_);
	}
}
}
}
}
}
