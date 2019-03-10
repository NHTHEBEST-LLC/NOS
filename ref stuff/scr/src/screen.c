#include "../include/screen.h"
int cursorX = 0, cursorY = 0;
const uint8 sw = 80,sh = 26,sd = 2; 
int color = 0x0F;
extern int Graphics_Mode;


void clearLine(uint8 from,uint8 to)
{
        uint16 i = sw * from * sd;
        string vidmem=(string)0xb8000;
        for(i;i<(sw*to*sd);i++)
        {
                vidmem[(i / 2)*2 + 1 ] = color ;
                vidmem[(i / 2)*2 ] = 0;
        }
}
void updateCursor()
{
    unsigned temp;

    temp = cursorY * sw + cursorX-1;                                                      // Position = (y * width) +  x

    outportb(0x3D4, 14);                                                                // CRT Control Register: Select Cursor Location
    outportb(0x3D5, temp >> 8);                                                         // Send the high byte across the bus
    outportb(0x3D4, 15);                                                                // CRT Control Register: Select Send Low byte
    outportb(0x3D5, temp);                                                              // Send the Low byte of the cursor location
}
void clearScreen()
{
        clearLine(0,sh-1);
        cursorX = 0;
        cursorY = 0;
        updateCursor();
        clearScreen_vga();
}

void scrollUp(uint8 lineNumber)
{
        string vidmem = (string)0xb8000;
        uint16 i = 0;
        clearLine(0,lineNumber-1);                                            //updated
        for (i;i<sw*(sh-1)*2;i++)
        {
                vidmem[i] = vidmem[i+sw*2*lineNumber];
        }
        clearLine(sh-1-lineNumber,sh-1);
        if((cursorY - lineNumber) < 0 ) 
        {
                cursorY = 0;
                cursorX = 0;
        } 
        else 
        {
                cursorY -= lineNumber;
        }
        updateCursor();
}


void newLineCheck()
{
        if(cursorY >=sh-1)
        {
                scrollUp(1);
        }
}



void printch(char c)
{
    if (Graphics_Mode)
    {
    string vidmem = (string) 0xb8000;     
    switch(c)
    {
        case (0x08):
                if(cursorX > 0) 
                {
	                cursorX--;									
                        vidmem[(cursorY * sw + cursorX)*sd]=0;	     //(0xF0 & color)                          
	        }
	        break;
       /* case (0x09):
                cursorX = (cursorX + 8) & ~(8 - 1); 
                break;*/
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        default:
                vidmem [((cursorY * sw + cursorX))*sd] = c;
                vidmem [((cursorY * sw + cursorX))*sd+1] = color;
                cursorX++; 
                break;
	
    }
    if(cursorX >= sw)                                                                   
    {
        cursorX = 0;                                                                
        cursorY++;                                                                    
    }
    updateCursor();
    newLineCheck();
    }else
    {
        
    }
}

void print (string ch)
{
        uint16 i = 0;
        uint8 length = strlength(ch);              //Updated (Now we store string length on a variable to call the function only once)
        for(i;i<length;i++)
        {
                printch(ch[i]);
        }
       /* while((ch[i] != (char)0) && (i<=length))
                print(ch[i++]);*/
        
}
void set_screen_color(int text_color,int bg_color)
{
	color =  (bg_color << 4) | text_color;;
}
void set_screen_color_from_color_code(int color_code)
{
	color = color_code;
}
void print_colored(string ch,int text_color,int bg_color)
{
	int current_color = color;
	set_screen_color(text_color,bg_color);
	print(ch);
	set_screen_color_from_color_code(current_color);
}

void SetCursor(int x, int y){
        cursorX = x;
        cursorY = y;
        updateCursor();
}
typedef char* va_list;
#define __va_argsiz(t)	\
	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, pN)	\
	((ap) = ((va_list) __builtin_next_arg(pN)))
#define va_arg(ap, t)					\
	 (((ap) = (ap) + __va_argsiz(t)),		\
	  *((t*) (void*) ((ap) - __va_argsiz(t))))
#define va_end(ap)	((void)0)
void __itoa_s(int i,unsigned base,char* buf) {
   if (base > 16) return;
   if (i < 0) {
      *buf++ = '-';
      i *= -1;
   }
   __itoa(i,base,buf);
}
char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};      
void __itoa(unsigned i,unsigned base,char* buf) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top=pos--;
	for (opos=0; opos<top; pos--,opos++)
		buf[opos] = tbuf[pos];
	buf[opos] = 0;
}
int printf (const char* str, ...) {
	if(!str)
		return 0;
	char* s = 0;
	va_list ap;
	va_start(ap, str);
	for(uint32 i = 0; i < strlen((string)str); i++)
	{
		if(str[i] == '%')
		{
			switch(str[i+1])
			{
				/** string **/
				case 's':
					s = va_arg(ap, string);
					print(s);
					i++;
					continue;
				/** decimal **/
				case 'd': {
					int c = va_arg(ap, int);
					print(int_to_string(c));
					i++;
					continue;
				}
                                /** hex **/
				case 'x': {
					int c = va_arg(ap, int);
					print(decToHexa((int)c));
					i++;
					continue;
				}
				/** character **/
				case 'c': {
					// char gets promoted to int for va_arg!
					// clean it.
					char c = (char)(va_arg(ap, int) & ~0xFFFFFF00);
                                        int_to_ascii(va_arg(ap, int),c);
					print(c);
					i++;
					continue;
				}
				default:
					break;
			}
		} else {
			printch(str[i]);
		}
	}
	va_end(ap);
	return 1;
}