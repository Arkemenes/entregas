#include <pio_maua.h>

void _pio_set_output(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_default_level, const uint32_t ul_pull_up_enable)
{



}


void _pio_set_input(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_attribute)
{

}

void _pio_set(Pio *p_pio, const uint32_t ul_mask)
{

		 if(p_pio==PIOA){
		p_pio->PIO_CODR = (1 << ul_mask );
		 }
	else{
		p_pio->PIO_SODR = (1 << ul_mask );	
	}

}

void _pio_clear(Pio *p_pio, const uint32_t ul_mask)
{
		 if(p_pio==PIOA){
		p_pio->PIO_SODR = (1 << ul_mask );
		 }
	else{
		p_pio->PIO_CODR = (1 << ul_mask );	
	}


}

uint32_t _pio_get_output_data_status(const Pio *p_pio, const uint32_t ul_mask)
{

}
