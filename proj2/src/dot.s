.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
	addi t0,x0,1
	blt a2,t0,exit1
	blt a3,t0,exit2
	blt a4,t0,exit2
	mv t0,x0
	slli a3,a3,2
	slli a4,a4,2
	beq x0,x0,loop_start
	# Prologue
exit1:
	li a0,36
	j exit
exit2:
	li a0,37
	j exit

loop_start:
	
	lw t1,0(a0)
	lw t2,0(a1)
	mul t1,t1,t2
	add t0,t0,t1
	add a0,a0,a3
	add a1,a1,a4
	addi a2,a2,-1
	bne a2,x0,loop_start

loop_end:
	mv a0,t0

	# Epilogue


	ret
