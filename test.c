#include "test.h"
#define SIZE 10

void
test_trim_whitespace(void)
{
	size_t len = 0, i = 0;
	char dst[SIZE] = {0}, src[SIZE] = {0};
	
	len = SIZE;
	strncpy(src, "  hello  ", 9);
	printf("first loop\n");
	for (i = 0; i < len; i++)
	{
		printf("src[%lu] : %d, dst[%lu] : %d\n", i, (int)src[i], i, (int)dst[i]);
	}
	trim_whitespace(dst, src, len);
	printf("second loop\n");
	for (i = 0; i < len; i++)
	{
		printf("src[%lu] : %d, dst[%lu] : %d\n", i, (int)src[i], i, (int)dst[i]);
	}
}

void
test_execute_script(void)
{
	char dst[256] = {0};
	execute_script("ls", dst, 256);	
	printf("output: %s\n", dst);

	memset(dst, 0, 256);
	execute_script("cat main.c", dst, 256);	
	printf("output: %s\n", dst);
}

void
test_get_net(void)
{
	char dst[BUF_SIZE] = {0};
	get_net(dst, BUF_SIZE);	
}

void
test_get_bat(void)
{
}

void
test_get_cpu(void)
{
	char dst[BUF_SIZE] = {0};
	get_cpu(dst, BUF_SIZE);
}

void
test_get_mem(void)
{
	char dst[BUF_SIZE] = {0};
	get_mem(dst, BUF_SIZE);
}

void
test_get_vol(void)
{
	char dst[BUF_SIZE] = {0};
	get_vol(dst, BUF_SIZE);
}

void
test_get_time(void)
{
	char dst[BUF_SIZE] = {0};
	get_time(dst, BUF_SIZE);
}
