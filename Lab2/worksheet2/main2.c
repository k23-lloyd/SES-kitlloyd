#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

int i = 0;
int k = 1;
int off = 5;

void inc(void){
  i += off;
  k += off;
}


int main(void){
  while (1) {
    inc();
  }
}

