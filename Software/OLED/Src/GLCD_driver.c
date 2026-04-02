#include "stm32l0xx_hal.h"

#include "GLCD_driver.h"

//#define 	oled_addr 			0x78
//#define 	Contrast_volue 	0xFF
//#define  	Column   64
//#define  	Row      48
//#define  	segment  32	    //IC start segment line

volatile 	uint8_t i = 0;
uint8_t		OledBuf[8];

extern I2C_HandleTypeDef hi2c1;


unsigned char page_pixel = 0;
unsigned char tamanho_x_max;
unsigned char tamanho_x;
unsigned char tamanho_y;
volatile unsigned char pages = 0;
unsigned char dado_lido = 0;
unsigned char posicao_espaco = 0;

unsigned char  * fonte;

volatile unsigned char x,y,cs;
volatile unsigned int ptr;

uint8_t normal = 0;

unsigned char display_buffer[Column*Row/8];


//extern const unsigned char FONT1[95][5];
//extern const unsigned char Calibri15x15[];


void LCD_OLED_Write_command (uint8_t valor)
{
	uint8_t aTxBuffer[2];
	aTxBuffer[0]	= 0x00;
	aTxBuffer[1]	= valor;	
	HAL_I2C_Master_Transmit(&hi2c1, oled_addr, aTxBuffer , 2 ,10 );
}

void LCD_OLED_Write_data (uint8_t valor)
{
	uint8_t aTxBuffer[2];
	aTxBuffer[0]	= 0x40;
	aTxBuffer[1]	= valor;	
	HAL_I2C_Master_Transmit(&hi2c1, oled_addr, aTxBuffer , 2 ,10 );
}

void LCD_OLED_Init (void)
{
	LCD_OLED_Write_command(0xAE);  // Display Off

	LCD_OLED_Write_command(0xD5);  //SET DISPLAY CLOCK
	LCD_OLED_Write_command(0x80);  //105HZ

	LCD_OLED_Write_command(0xA8);  // Select Multiplex Ratio
	LCD_OLED_Write_command(47);

	LCD_OLED_Write_command(0xD3);  //Setting Display Offset
	LCD_OLED_Write_command(0x00);  //00H Reset,  set common start

	LCD_OLED_Write_command(0x40);  //Set Display Start Line

	LCD_OLED_Write_command(0x8D);  // Set Charge Pump
	//Write_command(0x10);  // Disable Charge Pump
	LCD_OLED_Write_command(0x14);  // Endable Charge Pump


	LCD_OLED_Write_command(0xA1);  //Set Segment Re-Map Default 
	//0xA0 (0x00) => column Address 0 mapped to 127
	//0xA1 (0x01) => Column Address 127 mapped to 0

	LCD_OLED_Write_command(0xC8);  //Set COM Output Scan Direction 
	// 0xC0	(0x00) => normal mode (RESET) Scan from COM0 to COM[N-1];Where N is the Multiplex ratio.
	// 0xC8	(0xC8) => remapped mode. Scan from COM[N-1] to COM0;;Where N is the Multiplex ratio.

	LCD_OLED_Write_command(0xDA);  //Set COM Hardware Configuration
	//(0x02)=> A4=0;Sequential COM pin configuration;A5=0;Disable COM Left/Right remap 
	LCD_OLED_Write_command(0x12);

	LCD_OLED_Write_command(0x81);  //Set Contrast Control
	LCD_OLED_Write_command(Contrast_volue);

	LCD_OLED_Write_command(0xD9);  //Set Pre-Charge period        
	LCD_OLED_Write_command(0x22); 

	LCD_OLED_Write_command(0xDB);  //Set Deselect Vcomh level
	LCD_OLED_Write_command(0x40); 

	LCD_OLED_Write_command(0xA4);  //Entire Display ON

	LCD_OLED_Write_command(0xA6);  //Set Normal Display	
	LCD_OLED_Write_command(0xAF);  // Display ON	 
}



void LCD_OLED_Set_Linha_Coluna (uint8_t linha, uint8_t coluna)
{
	// linha [ 0.. 5]
	//coluna [ 0..47]
	LCD_OLED_Write_command(0xB0 + linha);								//Define a pagina (linha em pixel / 8)
	coluna = coluna + segment;
	LCD_OLED_Write_command(coluna & 0x0F);								//Definie a parte baixa 0x00 a 0x0F 
	LCD_OLED_Write_command(((coluna & 0xF0)>>4)+0x10);		//Definie a parte alta  0x10 a 0x1F 
}

//
void LCD_OLED_Clear_screen(void)//CLEAR
{
unsigned char x,y,Z=0xB0;

for(x = 0;x<(Row/8);x++){
		LCD_OLED_Write_command(Z);
		Z++;
		LCD_OLED_Write_command(0);
		LCD_OLED_Write_command(0x12);
		for(y = 0;y<Column;y++){
                     LCD_OLED_Write_data (0x00);     //Clear Column
					 
				 //Copia na RAM
				display_buffer[(Column*x)+y] = 0x00;
		}
	}

}

void LCD_OLED_Fill_screen(void)
{
unsigned char x,y,Z=0xB0;

for(x = 0;x<(Row/8);x++){
		LCD_OLED_Write_command(Z);
		Z++;
		LCD_OLED_Write_command(0);
		LCD_OLED_Write_command(0x12);
		for(y = 0;y<Column;y++){
                     LCD_OLED_Write_data (0xFF);     //Fill Column
					 
				 //Copia na RAM
				display_buffer[(Column*x)+y] = 0xFF;
		}
	}

}



//
void LCD_OLED_show_pic(const unsigned char *p){

	unsigned char  x,y,Z=0xB0;

	for(x = 0;x<(Row/8);x++){
		LCD_OLED_Write_command(Z);
		Z++;
		LCD_OLED_Write_command(0);
		LCD_OLED_Write_command(0x12);
		for(y = 0;y<Column;y++){
			
			//Copia na RAM
			display_buffer[(Column*x)+y] = *p;
			
			LCD_OLED_Write_data(*p++);
		}
	} 
}

void LCD_OLED_Monta_Linha_Coluna_Texto(unsigned char linha, unsigned char coluna, char dados[])
{
/*
Função de fonte basica	
Exemplo : LCD_OLED_Monta_Linha_Coluna_Texto(0,0, 	"   NewTech   ");
	
*/	
	
	unsigned char i =0;
	unsigned char j =0;
	
	unsigned char buff_index =0;

	LCD_OLED_Set_Linha_Coluna(linha,coluna);

	  j = 0;
	  do
	  {
	    i = 0;
	    do
	    {
	    LCD_OLED_Write_data(FONT1[(dados[j] - 0x20)][i]);
			
			//Copia na RAM
			display_buffer[(Column*linha)+coluna + buff_index] = FONT1[(dados[j] - 0x20)][i];
				
			i++;
			buff_index++;
			
	    }while(i != 5);
	    ++j;
	  }while((dados[j]));
}

//Monta imegens em qualquer posição da tela. Apenas imagens multiplas de 8 bits de altura.
void LCD_OLED_Draw_bitmap(unsigned char xb, unsigned char x_size, unsigned char yb, unsigned char y_size, unsigned char *bmp_img)
{
    unsigned int s = 0x0000;
    unsigned char x_pos = 0x00;
    unsigned char y_pos = 0x00;
    
    unsigned char y_aux = yb + (y_size/8) ;
    unsigned char x_aux = xb + x_size;
  
    for(y_pos = yb; y_pos < y_aux ; y_pos++)
    {
      LCD_OLED_Set_Linha_Coluna(y_pos,xb);
      for(x_pos = xb; x_pos < x_aux; x_pos++)
      {
        //Copia na RAM
		display_buffer[(Column*y_pos)+x_pos] = bmp_img[s];
		
        LCD_OLED_Write_data (bmp_img[s]);
        s++;
      }
    }
}


//


void LCD_OLED_Set_font(unsigned char * font, unsigned char font_height,  unsigned char font_width)
{
	fonte = font;							//fonte é um ponteiro global
	tamanho_x_max = font_width;

	//calcula apenas tamanho_y pois tamanho_x varia... então calcular de acordo com cada caracter
	tamanho_y = font_height / 8;

	if ( (font_height % 8) != 0 )
	{
		tamanho_y = tamanho_y + 1;
	}

	pages = (font_height / 8) + 1;

	posicao_espaco = 0;

}



unsigned char LCD_OLED_Le_dado(unsigned char linha, unsigned char coluna)
{
	unsigned char dado;

	dado = display_buffer[(linha)+coluna];

	return dado;
}
//


//Escreve texto com fontes selecionadas
void LCD_OLED_escreve_texto( unsigned char linha, unsigned char coluna, unsigned char data[] )
{
	static unsigned int var_auxiliar = 0;
	static unsigned char indice_coluna = 0;
	static unsigned char indice_coluna_atual = 0;
	static unsigned int indice_vetor = 0;
	static unsigned char i = 0;
	static unsigned char j = 0;
	static unsigned char k = 0;
	static unsigned char dado_plot = 0;
	static unsigned char aux_plot = 0;
	static unsigned char aux = 0;
	//static unsigned char index = 0;
	static unsigned int posicao_leitura = 0;

	indice_coluna = coluna;
	indice_coluna_atual = coluna;

	LCD_OLED_Set_Linha_Coluna (linha / 8 , coluna);			//linha / 8
	
	// if (((tamanho_y * 8) + linha) > 63)					//verifica linha valida
	// {
		// var_auxiliar = 63 - linha;
		// var_auxiliar = (tamanho_y * 8) - var_auxiliar;
		// linha = linha - var_auxiliar;					//desloca escrita para maximo permitido
		// // OU
		// // linha = 0;									//escreve na primeira linha
		// // OU
		// // return;										//sai da função caso tamanho invalido
	// }

	if((linha % 8) != 0)								//calcula pixel offset
	{
		page_pixel = linha >> 3;						//divide por 8 bits
		page_pixel = page_pixel << 3;					//multiplica por 8 bits
		page_pixel = linha - page_pixel;				//guarda valor do bit na página.  Ex: 35, pagina 4 bit 3 (0 1 2 3)
	}
	else
	{
		page_pixel = 0;
	}

	if(page_pixel == 0)
	{
		tamanho_y = tamanho_y - 1;
	}

	//A partir deste ponto Escrever todas as posições do vetor
	do															//loop para caracteres
	{
		//posicionar na posição do primeiro byte do primeiro caracter
		tamanho_x =  fonte[ (data[k] - 0x20) * ( pages * tamanho_x_max + 1) ];
		//tamanho_x =  Calibri15x15[ (data[k] - 0x20) * ( pages * tamanho_x_max + 1) ];							//teste fonte fixa
		
		//Indice_vetor neste ponto é a posição do primeiro byte do caracter
		var_auxiliar = (data[k] - 0x20) * ( pages * tamanho_x_max + 1) + 1;
		indice_vetor =  var_auxiliar ;

		indice_coluna_atual = coluna;

		for ( i = 0 ; i <= tamanho_y ; i++ )					//loop para páginas de um caracter
		{

			//indice_coluna = indice da coluna do caracter atual!
			indice_coluna = indice_coluna_atual;

			//Tratar o dado necessário de acordo com a linha e plotar
			for (j = 0 ; j < tamanho_x ; j++)						//loop para colunas de uma página, de cada caracter
			{
				if ( i == 0 )										//primeira página
				{
					if (page_pixel == 0)
					{
						dado_plot = fonte[indice_vetor];
						//dado_plot = Calibri15x15[indice_vetor];																					//teste fonte fixa
					}
					else
					{
					//pagina 1
					//buffer na linha inicial + pagina atual + coluna atual
					posicao_leitura = (Column * (linha/8)) + (Column * i) + indice_coluna;

					dado_lido = display_buffer[posicao_leitura];

					//dado_lido = 0 ;
					dado_plot = dado_lido & (0xFF >> (8 - page_pixel));

					dado_plot = dado_plot | (fonte[indice_vetor] << page_pixel);
					//dado_plot = dado_plot | (Calibri15x15[indice_vetor] << page_pixel);							//teste fonte fixa

					//plot byte
					}
				}
				else if ( i == tamanho_y )							//última página
				{
					if (page_pixel == 0)
					{
						dado_plot = fonte[indice_vetor];
						//dado_plot = Calibri15x15[indice_vetor];																							//teste fonte fixa
					}
					else
					{
					posicao_leitura = (Column * (linha/8)) + (Column * i) + indice_coluna;

					dado_lido = display_buffer[posicao_leitura];

					aux_plot = dado_lido & (0xFF << page_pixel);

					dado_plot = fonte[(indice_vetor - i)] >> (8 - page_pixel);
					//dado_plot = Calibri15x15[(indice_vetor - i)] >> (8 - page_pixel);						//teste fonte fixa

					dado_plot = dado_plot | aux_plot;

					//plot byte
					}
				}
				else												//páginas intermediárias
				{
					if (page_pixel == 0)
					{
						dado_plot = fonte[indice_vetor];
						//dado_plot = Calibri15x15[indice_vetor];																							//teste fonte fixa
					}
					else
					{
					//página 2 ou outra intermediária
					aux = fonte[indice_vetor - 1] >> (8 - page_pixel);
					//aux = Calibri15x15[indice_vetor - 1 ] >> (8 - page_pixel);	 										//teste fonte fixa

					aux_plot = fonte[indice_vetor] << page_pixel;  //dado da linha atual
					//aux_plot = Calibri15x15[indice_vetor] << page_pixel;  //dado da linha atual						//teste fonte fixa

					dado_plot = aux | aux_plot;

					//plot byte
					}
				}

				LCD_OLED_Set_Linha_Coluna ((linha / 8) + (i) , indice_coluna);			//linha / 8

				//atualiza_texto
				LCD_OLED_Write_data (dado_plot);
				//Copia na RAM
				display_buffer[(( Column * (linha/8)) + ( Column * i ))+indice_coluna] = dado_plot;

				//incrementa vetor de acordo com numero de paginas para seguir fonte
				indice_vetor = indice_vetor + pages;
				
				indice_coluna++;
				
			}
			
			indice_vetor = indice_vetor - (pages * tamanho_x) + i + 1;
		}
		indice_vetor = 0;
		k++;

		coluna = coluna + tamanho_x;
	}
	while ((data[k]) != '\0');

	indice_vetor = 0;
	if(page_pixel == 0)
	{
		tamanho_y = tamanho_y + 1;
	}
	k = 0;

}






//Reescreve o buffer na tela
void LCD_OLED_Write_buffer(unsigned char* buffer) {
//  unsigned char coluna, pagina;
//  unsigned int  pointer;
  
  unsigned char  x,y,Z=0xB0;

	for(x = 0;x<(Row/8);x++){
		LCD_OLED_Write_command(Z);
		Z++;
		LCD_OLED_Write_command(0);
		LCD_OLED_Write_command(0x12);
		for(y = 0;y<Column;y++){
			LCD_OLED_Write_data(*buffer++);
		}
	}
  
}


//
//--------------------------------------------------------------------------
//				Função para inverter o Display - são  necessários 02 comandos
//--------------------------------------------------------------------------
//
void LCD_OLED_Flip (void)
{
	if (!normal)
	{
		normal = 1;												// variável indicativa do sts do Display
		LCD_OLED_Write_command(0xA0);		// inverter o Display
		LCD_OLED_Write_command(0xC0);		// remapear o Scan da direção do Display
	}
	else
	{
		normal = 0;
		LCD_OLED_Write_command (0xC8);
		LCD_OLED_Write_command(0xA1);
	}
}



