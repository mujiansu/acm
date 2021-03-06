#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NTRIE (256*1024)
#define NCHAR (256*1024)

char buf[NCHAR], alpha[NTRIE];
int got[NCHAR], dict[1024], len[1024], bufptr, id, n;
int fin[NTRIE], child[NTRIE], sib[NTRIE], total;

void prn(int p)
{
	printf("(");
	for (p = child[p]; p != 0; p = sib[p]) {
		printf("%c", alpha[p]);
		if (fin[p]) printf("$%d ",fin[p]-1);
		if (child[p] != 0) prn(p);
	}
	printf(")");
}

void insert(char *s, int f)
{
	int p, q;

	for (p = 0; *s; s++) {
		for (q = child[p]; q && alpha[q] != *s; q = sib[q]);
		if (q) {
			p = q;
		} else {
			total++;
			alpha[total] = *s;
			child[total] = 0;
			fin[total] = 0;
			sib[total] = child[p];
			child[p] = total;
			p = total;
		}
	}

	fin[p] = f;
}

int sol(int st)
{
	static int queue[NCHAR], dist[NCHAR], q2[NTRIE], head, tail;
	int i, j, p;

	if (buf[st] == '\0') return 0;
	
/*for(i=0;i<n;i++)printf("dict[%d]=%s\n",i,buf+dict[i]);printf("\n");*/

	head = 0; tail = 1; got[queue[0] = st] = id; dist[0] = 0;
	for (; head < tail; head++) {
/*printf("%s:%d\n", buf+queue[head],dist[head]);*/
		
		for (p = 0, i = queue[head]; buf[i] != '\0'; i++) {
			if (fin[p] > 0 && got[i] != id) {
				got[queue[tail] = i] = id;
				dist[tail++] = dist[head] + 1;				
			}
			for (p = child[p]; p && alpha[p] != buf[i]; p = sib[p]);
			if (p == 0) break;
		}

		if (buf[i] == '\0' && fin[p] > 0)
			return (dist[head] + 1);
		
		if (p == 0) continue;
/*printf("  len=%d\n", i-queue[head]);
printf("  endings trie:");prn(p);printf("\n");*/

		p = child[p];
		if (p == 0) continue;

		for (q2[0] = p, j = 1; j-- > 0;) {
			p = q2[j];
			if (child[p] != 0) q2[j++] = child[p];
			if (sib[p] != 0) q2[j++] = sib[p];
			if (fin[p]) {
/*printf("<%d> %s\n",fin[p]-1, buf+dict[fin[p]-1] + i - queue[head]);*/
				p = dict[fin[p] - 1] + i - queue[head];
				if (got[p] != id) {
					got[queue[tail] = p] = id;
					dist[tail++] = dist[head] + 1;
				}
			}			
		}


/*		for (k = 0; k < n; k++) {
			i = queue[head]; j = dict[k];
			while (buf[i] == buf[j] && buf[i] != '\0') i++, j++;

			if (buf[i] == '\0' && buf[j] == '\0')
				return (dist[head] + 1);

			if (buf[i] == '\0' && got[j] != id) {
				got[queue[tail] = j] = id;
				dist[tail++] = dist[head] + 1;
			} else if (buf[j] == '\0' && got[i] != id) {
				got[queue[tail] = i] = id;
				dist[tail++] = dist[head] + 1;
			}
		}
*/
	}

	return -1;
}

int get()
{
	int i, j, r;
	gets(buf + bufptr);
	for (i = j = r = bufptr; buf[j]; j++)
		if (isalpha(buf[j])) buf[i++] = tolower(buf[j]);
	buf[i++] = '\0';
	bufptr = i;
	return r;
}

int getint()
{
	static char s[2048];
	int x;

	for (;;) {
		if (gets(s) == NULL) abort();
		if (sscanf(s, "%d", &x) == 1) return x;
	}
}

int main()
{
	int w1, w2, i, t;

	memset(got, 0, sizeof(got));
	id = 42;

	for (t = getint(); t-- > 0;) {
		id++;
		bufptr = 1;
		w1 = get();
		w2 = get();
		n = getint();

		for (i = 0; i < n; i++) {
			dict[i] = get();
			len[i] = strlen(buf + dict[i]);
		}

		while (buf[w1] == buf[w2] && buf[w1] != '\0')
			w1++, w2++;

		child[0] = total = 0;
		for (i = 0; i < n; i++)
				insert(buf + dict[i], i+1);

		if (buf[w1] == '\0')
			printf("%d\n", sol(w2));
		else if (buf[w2] == '\0')
			printf("%d\n", sol(w1));
		else
			printf("-1\n");
	}

	return 0;
}
