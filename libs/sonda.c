/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ADC_subscription.h>
#include <constants.h>
#include <sonda.h>

uint32_t sondaADCMemory;
uint8_t* puntSonda;

double validVoltSonda[NUM_VOLT]={3.3,3,2.48,1.98,1.65,1.416,1.1,0.66,0.3};

int indexSondaADC;

volatile int countSonda = 0;
volatile long double media=0;
uint8_t releasedSonda = 1;

int nearestVoltSonda(double voltage){
    int near = 0;
    double diff = -1;
    int i;

    diff = voltage - validVoltSonda[0];
    if(diff<0) diff = -diff;

    for (i=1;i<NUM_VOLT;i++){
        double d = voltage - validVoltSonda[i];
        if(d<0) d = -d;

        if(diff>d){
            diff = d;
            near = i;
        }
    }

    return near;
}

void ADCSondaIRQ(void){
    double volt = (ADC14_getResult(sondaADCMemory)/16382.0)*3.3;
    if(volt>0.2){
        media+=volt;
        countSonda++;

        if(countSonda==NUM_SAMPLES){
            countSonda=0;
            media/=NUM_SAMPLES;

            if(*puntSonda == 0 && releasedSonda) //evita che venga modificata mentre sto lavorando sull'input e di ridare lo stesso
            {
                *puntSonda = nearestVoltSonda(media)+1;
                releasedSonda = 0;
            }

            media=0;
        }

        //printf("volt: %f nearest: %d\n",volt, nearestSonda[countSonda-1]);
    }else{
        releasedSonda = 1;
        countSonda = 0;
        media = 0;
    }
}

void disableInterruptSonda(){
    disableADC(indexSondaADC);
}

void enableInterruptSonda(){
    enableADC(indexSondaADC);
}

void setupSonda(uint16_t pin, uint8_t port, uint32_t ADCInputMap, uint32_t ADCMemory, uint8_t* punt){

    sondaADCMemory = ADCMemory;
    puntSonda = punt;

    //setup port (A � sempre la terza)
    GPIO_setAsPeripheralModuleFunctionInputPin(port, pin, GPIO_TERTIARY_MODULE_FUNCTION);

    ADC14_configureConversionMemory(ADCMemory, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADCInputMap, false);

    indexSondaADC = registerADC(ADCSondaIRQ);
    disableInterruptSonda();
}