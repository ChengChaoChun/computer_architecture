.data
stu_id: .word 0x00000001, 0x02000000, 0x00000010, 0x00000020, 0x00008000, 0x00000002, 0x08000000, 0x00000004, 0x00000800, 0x00400000,0x40000000, 0x00004000, 0x00000080, 0x20000000, 0x00020000, 0x00040000, 0x10000000, 0x01000000, 0x00000100, 0x00001000,0x00000200, 0x80000000, 0x00000000, 0x00000008, 0x04000000, 0x00010000, 0x00002000, 0x00200000, 0x00000040, 0x00000000,0x00000000, 0x00100000, 0x00080000

stu_score: .word 89, 72, 96, 64, 88, 73, 61, 98, 76, 65, 79, 84, 77, 99, 81, 62, 95, 67, 71, 70, 78, 93, 90, 69, 63,75, 80, 66, 94, 68, 91, 97, 74
stu_num: .word 33
x1: .word 0x00000004
x2: .word 0x00000002
x3: .word 0x00000001

.text
.global main
main:
    # Call get_stu_id
    jal ra, get_id
get_stu_score:
    la t0, stu_score 
    mv t1, a0
loop:
    beq t1, zero, Exit
    addi t0, t0 ,4
    addi t1, t1, -1
    j loop
Exit:
    lw a0, 0(t0)
    li a7, 93      # Exit syscall number
    ecall
    
get_id:
    # Arguments:
    # a0: uint32_t id, a1: uint32_t* stu_id, a2: int stu_num
    lw a0, x2  #x1,x2,x3
    la a1, stu_id
    lw a2, stu_num
    # int x = count_leading_zeros(fruit_number);  x equal t0
    jal ra, count_leading_zeros
    mv t0, a0 
    # Loop variables t1
    li t1, 0        # i
loop_start:
    # Load stu_id[i] into t2
    lw a0, 0(a1)
    # int y = count_leading_zeros(fruit[i]); y equal t2
    jal ra, count_leading_zeros
    mv t2, a0
    # Check if x == y
    beq t0, t2, found_match
    # Increment i
    addi t1, t1, 1
    #stu_id[i+1]
    addi a1, a1, 4
    # Check if i < stu_num
    blt t1, a2, loop_start
    # If no match found, return 0
    li a0, 0
    j get_stu_score
found_match:
    # Return the index i
    mv a0, t1
    j get_stu_score

count_leading_zeros:
    # Arguments: a0: uint32_t x
    # Returns: a0: uint16_t (result)
    # x |= (x >> 1);
    srli t2, a0, 1
    or a0, a0, t2
    # x |= (x >> 2);
    srli t2, a0, 2
    or a0, a0, t2
    # x |= (x >> 4);
    srli t2, a0, 4
    or a0, a0, t2
    # x |= (x >> 8);
    srli t2, a0, 8
    or a0, a0, t2
    # x |= (x >> 16);
    srli t2, a0, 16
    or a0, a0, t2
    # x -= ((x >> 1) & 0x55555555);
    srli t2, a0, 1
    lui t3, 0x55555
    ori t3, t3, 0x555
    and t3, t2, t3
    sub a0, a0, t3
    # x = ((x >> 2) & 0x33333333) + (x & 0x33333333);
    srli t2, a0, 2
    lui t3, 0x33333
    ori t3, t3, 0x333
    and t2, t2, t3
    and t3, a0, t3
    add a0, t2, t3
    # x = ((x >> 4) + x) & 0x0f0f0f0f;
    srli t2, a0, 4
    add t2, t2, a0
    lui t3, 0x0f0f0
    ori t3, t3, 0x787
    and a0, t2, t3
    # x += (x >> 8);
    srli t2, a0, 8
    add a0, a0, t2
    # x += (x >> 16);
    srli t2, a0, 16
    add a0, a0, t2
    # return (32 - (x & 0x7f));
    li t2, 32
    andi a0, a0, 0x7f
    sub a0, t2, a0
    ret
