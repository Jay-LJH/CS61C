.globl argmax
.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
	bgt a1,zero,start
	li a0,36
	j exit
start:
    mv t0,a1
    lw a3,0(a0)
    mv a4,x0
loop_start:
	lw a2,0(a0)
    beq a2,a3,loop_continue
	blt a2,a3,loop_continue
	mv a3,a2
    sub a4,t0,a1
loop_continue:
	addi a1, a1, -1
	addi a0,a0, 4
	bne a1,x0,loop_start

loop_end:
	# Epilogue
    mv a0,a4
	ret
