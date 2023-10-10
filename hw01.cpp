#include <stdio.h>
#include <stdint.h>

uint32_t stu_id[33] = { 0x00000001, 0x02000000, 0x00000010, 0x00000020, 0x00008000, 0x00000002, 0x08000000, 0x00000004, 0x00000800, 0x00400000, 0x40000000, 0x00004000, 0x00000080, 0x20000000, 0x00020000, 0x00040000, 0x10000000, 0x01000000, 0x00000100, 0x00001000, 0x00000200, 0x80000000, 0x00000000, 0x00000008, 0x04000000, 0x00010000, 0x00002000, 0x00200000, 0x00000040, 0x00000000, 0x00000000, 0x00100000, 0x00080000 };//The IDs of 33 students.
int stu_score[33] = { 89, 72, 96, 64, 88, 73, 61, 98, 76, 65,
79, 84, 77, 99, 81, 62, 95, 67, 71, 70, 78, 93, 90, 69, 63,
75, 80, 66, 94, 68, 91, 97, 74 };//The scores of 33 students.
//test data x1,x2,x3
int x1 = 0x00000004;
int x2 = 0x00000002;
int x3 = 0x00000001;

uint16_t count_leading_zeros(uint32_t x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);

    /* count ones (population count) */
    x -= ((x >> 1) & 0x55555555 /* Fill this! */);
    x = ((x >> 2) & 0x33333333) + (x & 0x33333333 /* Fill this! */);
    x = ((x >> 4) + x) & 0x0f0f0f0f;
    x += (x >> 8);
    x += (x >> 16);

    return (32 - (x & 0x7f));
}

int get_id(uint32_t id, uint32_t* stu_id, int stu_num) {
    int x = count_leading_zeros(id);
    for (int i = 0; i < stu_num; i++) {
        int y = count_leading_zeros(stu_id[i]);
        if (x == y)return i;
    }
    return 0;
}

int main()
{
    printf("%d", stu_score[get_id(x2, stu_id, 33)]);//output 73
    return 0;
}