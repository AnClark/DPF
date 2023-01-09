#ifndef AMSYNTH_DPF_BANKS_H
#define AMSYNTH_DPF_BANKS_H

#include "EmbedPresetController.h"
#include <cstdint>

const uint32_t numBanks = EmbedPresetController::getPresetBanks().size();
const uint32_t numPrograms = EmbedPresetController::kNumPresets;

class AmsynthBankState {
    uint32_t fCurrentBankId;
    uint32_t fCurrentProgramId;

public:
    void setCurrentBank(const uint32_t bankId)
    {
        if (bankId > numBanks || bankId < 0)
            fCurrentBankId = 0; // Fallback to 0 if invalid input detected
        else
            fCurrentBankId = bankId;
    }
    uint32_t getCurrentBank() { return fCurrentBankId; }

    void setCurrentProgram(const uint32_t progId)
    {
        if (progId > numPrograms || progId < 0)
            fCurrentProgramId = 0; // Fallback to 0 if invalid input detected
        else
            fCurrentProgramId = progId;
    }
    uint32_t getCurrentProgram() { return fCurrentProgramId; }
};

static AmsynthBankState s_bank_state;

#endif // AMSYNTH_DPF_BANKS_H
