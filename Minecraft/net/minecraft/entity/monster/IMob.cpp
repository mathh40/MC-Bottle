#include "IMob.h"

#include "Util.h"
#include "../Entity.h"

bool IMob::MOB_SELECTOR(Entity *p_apply_1_) const {
    return Util::instanceof<IMob>(p_apply_1_);
}

bool IMob::VISIBLE_MOB_SELECTOR(Entity *p_apply_1_) const {
    return Util::instanceof<IMob>(p_apply_1_) && !p_apply_1_->isInvisible();
}
