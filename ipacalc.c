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
      printf ("%s  ", buffer);
        // Note that two spaces are required only if the F_4x6 font is used
      key = ngetchx ();
      if (key >= ' ' && key <= '~' && i < maxlen)
        buffer[i++] = key;
      else if (key == KEY_BACKSPACE && i)
        i--;
      buffer[i] = 0;
    } while (key != KEY_ENTER);
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

void getNetmask(int netmaskbit, int *netmask)
{
	int i;	
	int ioct = netmaskbit / 8;
	for (i = 0; i < 4; i++)
	{
		if (i < ioct)
		{
			netmask[i] = 255;			
		}
		else
		{
			netmask[i] = 0;
		}
	}
	
	int ispecial = netmaskbit - ioct * 8;
	for (i = 7; i > 0; i--)
	{
		 if (ispecial == 0) { break; }
		 netmask[ioct] += (int)pow(2, i);
		 ispecial--;
	}
}


void getNetaddress(int *address, int netmaskbit, int *netmask, int *netaddress)
{
	int i;
	int ioct = netmaskbit / 8;
	int ispecial = netmaskbit - ioct * 8;
	int ilength = 256 / (int)pow(2, ispecial);
	
	for (i = 0; i < 4; i++)
	{
		if (netmask[i] == 255)
		{
			netaddress[i] = address[i];			
		}
		else
		{
			netaddress[i] = 0;
		}
	}
	
	while (netaddress[ioct] + ilength < address[ioct])
	{
		netaddress[ioct] += ilength;
	}
}


void getBroadcast(int *address, int netmaskbit, int *netmask, int *netaddress)
{
	int i;
	int ioct = netmaskbit / 8;
	int ispecial = netmaskbit - ioct * 8;
	int ilength = 256 / (int)pow(2, ispecial);
	
	for (i = 0; i < 4; i++)
	{
		if (netmask[i] == 255)
		{
			netaddress[i] = address[i];			
		}
		else
		{
			netaddress[i] = 255;
		}
	}
	
	while (netaddress[ioct] -ilength >= address[ioct])
	{
		netaddress[ioct] -= ilength;
	}
	
}


long getMaxclients(int netmaskbit)
{
	int ireverse = 32 - netmaskbit;
	long result = (long)pow(2, ireverse);
	return result - 2;
}

// Main Function
void _main(void)
{
  clrscr ();
  
  int iaddress[4];
  int inetmask[4];
  int inetaddr[4];
  int ibrdcast[4];
  int inetmaskbit;
  
  int i;
  for (i = 0; i < 4; i++)
  {
  	printf("\n%i. Adressoctet: ", i+1);
  	iaddress[i] = InputInt(0, 255);
  }
  printf("\nNetmask (bits): ");
  inetmaskbit = InputInt(0, 32);
  
  clrscr ();
 	printf("\nAddress: %i.%i.%i.%i/%i", iaddress[0], iaddress[1], iaddress[2], iaddress[3], inetmaskbit);
 	
 	getNetmask(inetmaskbit, inetmask);
 	printf("\nNetmask: %i.%i.%i.%i", inetmask[0], inetmask[1], inetmask[2], inetmask[3]);
 	
 	getNetaddress(iaddress, inetmaskbit, inetmask, inetaddr);
 	printf("\nNetaddr: %i.%i.%i.%i", inetaddr[0], inetaddr[1], inetaddr[2], inetaddr[3]);
 	
 	getBroadcast(iaddress, inetmaskbit, inetmask, ibrdcast);
 	printf("\nBrdcast: %i.%i.%i.%i", ibrdcast[0], ibrdcast[1], ibrdcast[2], ibrdcast[3]);
 	
 	printf("\nClients: %lu", getMaxclients(inetmaskbit));
  
  ngetchx ();
}
