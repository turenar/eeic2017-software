#include <stdio.h>

char inc_char1(char, int);
char inc_char2(char, int);
char inc_char3(char, int);

int main() {
	char str[] = "abigpcvozgabignwwtqap";
	for (int i = 0; i < 27; i++) {
		for (char* p = str; *p != '\0'; p++) {
			putchar(inc_char1(*p, i));
			putchar(inc_char2(*p, i));
			putchar(inc_char3(*p, i));
		}
		putchar('\n');
	}
}

char inc_char1(char c, int diff) {
	char ret = c;
	// 1ずつ進める をdiff回繰り返すやつ。直感的。
	for (int i = 0; i < diff; i++) {
		ret++;
		if (ret > 'z') {
			ret -= 'z' + 1 - 'a'; // zの次をaにしたい→zの次を引いてa足してあげればいい
		}
	}
	return ret;
}

char inc_char2(char c, int diff) {
	// aとの差を考えるタイプ。回りくどい。
	int az = c - 'a';
	az += diff;
	if (az > 'z' - 'a') {
		az -= 'z' + 1 - 'a';
	}
	return 'a' + az;
}

char inc_char3(char c, int diff) {
	// c+diffを計算して、a〜zに収めようという考え方。
	// 動きそうで実は動かないタイプ。
	char ret = c + diff;
	// charは (通常の環境では) -128〜127の値しか取らないのですが、実は 'z' は122なんですよね；；
	// なので、z+6するとオーバーフローして-128になってしまうのです。すると、
	// ret > 'z' (-128 > 122)に引っかからなくなってしまうので、思ったように答え出ません。。。
	// 今回の場合は、charをunsigned char (0〜255) または int (0〜65535以上) にしてあげれば解決なわけです。
	// こういうところで引っかかると訳がわからなくなってしまうので、注意をば。
	// gdbだったりprintfデバッグを使いこなせるようにしような・。・
//	unsigned char ret = c + diff;
//	printf("%d, %d\n", c + diff, ret);
	if (ret > 'z') {
		ret -= 'z' - 'a' + 1;
	}
	return ret;
}
