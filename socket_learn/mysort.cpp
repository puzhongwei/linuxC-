#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>

using namespace std;

void mysort(int a[], int low, int high)
{
	if(low<high)
	{
		int i = low, j = high; 
		int tmp = a[low];
		while(i<j)
		{
			while(i<j && a[j] >= tmp) j--;
			if(i<j)
			{
				a[i++] = a[j];
			}

			while(i<j && a[i] < tmp) i++;
			if(i<j)
			{
				a[j--] = a[i];
			}
		}
		a[i] = tmp;
		mysort(a, low, i-1);
		mysort(a, i+1, high);
	}
}

int main()
{

	int a[] = {1, -9, 0, 155, -56};
	mysort(a, 0, 4);
	for(int i =0 ; i< 5;++i)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
  return 0;
}

