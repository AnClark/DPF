#ifndef AMSYNTH_DPF_BANKS_H
#define AMSYNTH_DPF_BANKS_H

#include "EmbedPresetController.h"
#include <cstdint>

const uint32_t numBanks = EmbedPresetController::getPresetBanks().size();

class AmsynthBankState {
    uint32_t fCurrentBankId;

public:
    void setCurrentBank(const uint32_t bankId)
    {
        if (bankId > numBanks || bankId < 0)
            fCurrentBankId = 0; // Fallback to 0 if invalid input detected
        else
            fCurrentBankId = bankId;
    }
    uint32_t getCurrentBank() { return fCurrentBankId; }
};

static AmsynthBankState s_bank_state;

#endif // AMSYNTH_DPF_BANKS_H
