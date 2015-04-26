// ipacalc.c
// Maintainer: Cyrill Leutwiler <bigcyrill @ hotmail dot com>
// Created 26.04.2015; 21:14:21

// Delete or comment out the items you do not need.
#define COMMENT_STRING         "An simple IP-Addresscalculator for TI-89 Titanium."
#define COMMENT_PROGRAM_NAME   "ipacalc"
#define COMMENT_VERSION_STRING "1.0"
#define COMMENT_VERSION_NUMBER 1,0,0,0 /* major, minor, revision, subrevision */

#include <tigcclib.h>

/* InputStr
 * Custom String Input Function
 * This is not my own code! I just copied this from the help..
 */
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

/* InputInt
 * Returns an integer from input.
 * Also checks the user input, if its not a number it will ask for input again.
 */
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

/* getNetmask
 * Calculates the netmask for a given number of netmask bits.
 */
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

/* getNetaddress
 * Calculates the network address for for the given IP Adress
 */
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

/* getBroadcast
 * Calculates the Broadcast address for a specific IP Adress
 */
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

/* getBroadcast
 * Calculates how many Adresses are available for the given netmaskbits.
 */
long getMaxclients(int netmaskbit)
{
	return (long)pow(2, 32 - netmaskbit) - 2;
}

/* _main
 * The Main functions. Asks for an IP-Adress and shows its details.
 */
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
