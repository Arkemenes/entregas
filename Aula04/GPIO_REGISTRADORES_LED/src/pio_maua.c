#include <pio_maua.h>

void _pio_set_output(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_default_level, const uint32_t ul_pull_up_enable)
{
	p_pio->PIO_PER |= ul_mask;
	p_pio->PIO_WPMR = 0;
	p_pio->PIO_OER |=  ul_mask;
	_pio_pull_up(p_pio,ul_mask,ul_pull_up_enable);

	if(ul_default_level){
		_pio_set(p_pio,  ul_mask);
	}else{
		_pio_clear(p_pio,   ul_mask);
	}
}


void _pio_set_input(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_attribute)
{
	p_pio->PIO_ODR  = ul_mask;
	
	
	if(ul_attribute & (1 << 0)){		//PULLUP
		p_pio->PIO_PUER = ul_mask;
	}else{
		p_pio->PIO_PUDR = ul_mask;
	}
	/*
	if(ul_attribute & (1 << 1)){		//DEGLITH
		p_pio->PIO_IFER = 1 << ul_mask;
	}else{
		p_pio->PIO_IFDR = 1 << ul_mask;
	}
	*/
	if(ul_attribute & (1 << 2)){		//OPENDRAIN
		p_pio->PIO_IFER = ul_mask;
	}else{
		p_pio->PIO_IFDR = ul_mask;
	}
	
	if(ul_attribute & (1 << 3)){		//DEBOUNCE
		p_pio->PIO_IFSCER = ul_mask;
	}else{
		p_pio->PIO_IFSCDR = ul_mask;
	}
	
	
	
}

void _pio_set(Pio *p_pio, const uint32_t ul_mask)
{

		 if(p_pio==PIOA){
		p_pio->PIO_CODR = ul_mask;
		 }
	else{
		p_pio->PIO_SODR = ul_mask;	
	}

}

void _pio_clear(Pio *p_pio, const uint32_t ul_mask)
{
		 if(p_pio==PIOA){
		p_pio->PIO_SODR = ul_mask;
		 }
	else{
		p_pio->PIO_CODR = ul_mask;	
	}


}

uint32_t _pio_get_output_data_status(const Pio *p_pio, const uint32_t ul_mask)
{

}

void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_pull_up_enable)
{
	if(ul_pull_up_enable){
	p_pio->PIO_PUER = ul_mask;
	}else{
	p_pio->PIO_PUDR = ul_mask;
	}
}

void _pio_pull_down(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_pull_down_enable)
{
		if(ul_pull_down_enable){
	p_pio->PIO_PPDER = ul_mask;
	}else{
	p_pio->PIO_PPDDR = ul_mask;
	}
}
