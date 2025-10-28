#include "decoder.h"

void decoder_init() {

    // Disable writeprotection
    PMC->PMC_WPMR = (0x504D43 << 8);
    TC2->TC_WPMR = (0x54494D << 8);
    PIOC->PIO_WPMR = (0x50494F << 8);

    // Disable PIO on pins
    PIOC->PIO_PDR = PIO_PC25 | PIO_PC26;
    // Select Peripheral to TC2
    PIOC->PIO_ABSR |= PIO_PC25 | PIO_PC26;

    // Enable peripheral clock
    // This must be done in three commands, since the 5 LSB are the id of the peripheral
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC6);
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC7);
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC8);

    // Enable Quadratic Decoder with Position meassurement
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_MAXFILT(DECODER_MAXFILT - 1);

    // Select XC0, set trigger to RISING, external trigger on TIOA
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 | TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;
    TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_XC0 | TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;

    // Enable TC2 channel 0 and 1
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    // Enable writeprotection
    // PMC->PMC_WPMR = (0x504D43 << 8) | PMC_WPMR_WPEN;
    // TC2->TC_WPMR = (0x504D43 << 8) | TC_WPMR_WPEN;
    // PIOC->PIO_WPMR = (0x50494F << 8) | PIO_WPMR_WPEN;
}

int decoder_read() {
    int position = TC2->TC_CHANNEL[0].TC_CV;
    // uint32_t rotation = TC2->TC_CHANNEL[1].TC_CV;
    // printf("Position %d, Rotation %d\n", position, rotation);

    return position;
}

void decoder_setnullpoint() {
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}