#include <tigcclib.h>

// Custom String Input Function
void InputStr(char *buffer, unsigned short maxlen)
{
  SCR_STATE ss;
  short key;
  unsigned short i = 0;
  buffer[0] = 0;
  SaveScrState (&ss);
  do
    {
      MoveTo (ss.CurX, ss.CurY);
      printf ("%s_  ", buffer);
        // Note that two spaces are required only if the F_4x6 font is used
      key = ngetchx ();
      if (key >= ' ' && key <= '~' && i < maxlen)
        buffer[i++] = key;
      else if (key == KEY_BACKSPACE && i)
        i--;
      buffer[i] = 0;
    } while (key != KEY_ENTER);
}

char *IntToStr (unsigned long an_integer)
{
  static char result [] = "          \0";    // 10 spaces and \0
  char *ptr = result + 10;
  while (an_integer)
    {
      *ptr-- = an_integer % 10 + '0';
      an_integer/=10;
    }
  return ptr;
}

int InputInt (int min, int max)
{	
	int ivalidinput;
	int iresult;		
	char cinput[20];
	do{	  
	  iresult = 0;
		ivalidinput = 1;
		InputStr (cinput, 20);
	  int i = 0;
	  for (i = 0; cinput[i] != 0x00; i++)  {
	  	int inumber = cinput[i] - '0';	  	
	  	iresult *= 10;
	  	iresult += inumber;
	  	
	  	//Input validation
	  	if ((inumber < 0 || inumber > 9) || iresult < min || iresult > max)
	  	{
	  		ivalidinput = 0;
	  		printf("\n%s\n", "invalid input, try again:");
	  		break;
	  	}
	  }
	  
	} while(!ivalidinput);
	  
	return iresult;
}

// Main Function
void _main(void)
{
  clrscr ();
  
  int iaddress[4];
  int inetmask;
  
  int i;
  for (i = 0; i < 4; i++)
  {
  	printf("\n%i. Adressoctet: ", i+1);
  	iaddress[i] = InputInt(0, 255);
  }
  printf("\nNetmask (bits): ");
  inetmask = InputInt(0, 32);
  
 	printf("CIDR: \n%i.%i.%i.%i/%i", iaddress[0], iaddress[1], iaddress[2], iaddress[3], inetmask);
  
 //for (i = 0; i < 3; i++)  {  }
  
  ngetchx ();
}
