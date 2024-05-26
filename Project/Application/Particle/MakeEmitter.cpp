#include "MakeEmitter.h"
#include <cassert>
#include "EmitterName.h"
#include "../../../Application/Particle/user/InfiniteEmitter/InfiniteEmiiter.h"

MakeEmitter* MakeEmitter::GetInstance()
{
    return nullptr;
}

IEmitter* MakeEmitter::Run(EmitterDesc* emitterDesc, uint32_t emitterName)
{
	
	IEmitter* emitter = nullptr;

	switch (emitterName)
	{
	case kDefaultEmitter:
		emitter = new IEmitter();
		emitter->Initialize(emitterDesc);
		break;
	case kInfiniteEmitter:
		emitter = new InfiniteEmiiter();
		emitter->Initialize(emitterDesc);
		break;
	case kCountOfParticleName:
	default:
		assert(0);
		break;
	}

	return emitter;

}
