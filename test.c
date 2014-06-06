/* fibonacci */
int iterate(int n)
{
	int a;
	int b;
	int c;
	int i;
	a = 1;
	b = 1;
	if( n == 0 ){
		return 0;
	}
	i = 3;
	while(i <= n){
		int c;
		c = a + b;
		a = b;
		b = c;
		i = i + 1;
	}
	return b;
}

int recur(int n)
{
	if(n == 0)
		return 0;
	else if(n == 1)
		return 1;
	else if(n > 1)
		return recur(n-1) + recur(n-2);
	else
		return 0;
}
int a;
int b[2];
void main(void){
	int x;
	int y;
	x = input();
	output(iterate(x));
	y = input();
	output(recur(y));
}
