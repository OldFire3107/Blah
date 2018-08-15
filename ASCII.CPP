#include<iostream.h>
#include<conio.h>

void main()
{
	clrscr();
	char a;
	for(int c = 50; c < 255; c++)
	{
		a = c;
		cout<<c<<") "<<a<<endl;
		getch();
	}
}